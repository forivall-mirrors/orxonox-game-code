/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Authors:
 *      Martin Polak
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "Munition.h"

#include "core/CoreIncludes.h"
#include "core/command/Executor.h"

namespace orxonox
{
    RegisterClass(Munition);

    Munition::Munition(Context* context) : BaseObject(context)
    {
        RegisterObject(Munition);

        this->maxMunitionPerMagazine_ = 10;
        this->maxMagazines_ = 10;
        this->magazines_ = 10;

        this->bUseSeparateMagazines_ = false;
        this->bStackMunition_ = true;
        this->bAllowMunitionRefilling_ = true;
        this->bAllowMultiMunitionRemovementUnderflow_ = true;

        this->reloadTime_ = 0;
    }

    Munition::~Munition()
    {
        for (std::map<WeaponMode*, Magazine*>::iterator it = this->currentMagazines_.begin(); it != this->currentMagazines_.end(); ++it)
            delete it->second;
    }

    Munition::Magazine* Munition::getMagazine(WeaponMode* user) const
    {
        if (this->bUseSeparateMagazines_)
        {
            // For separated magazines we definitively need a given user
            if (!user)
                return 0;

            // Use the map to get the magazine assigned to the given user
            std::map<WeaponMode*, Magazine*>::const_iterator it = this->currentMagazines_.find(user);
            if (it != this->currentMagazines_.end())
                return it->second;
        }
        else
        {
            // We don't use separate magazines for each user, so just take the first magazine
            if (this->currentMagazines_.size() > 0)
                return this->currentMagazines_.begin()->second;
        }

        return 0;
    }

    unsigned int Munition::getNumMunition(WeaponMode* user) const
    {
        Magazine* magazine = this->getMagazine(user);
        if (magazine)
        {
            if (this->bStackMunition_)
                // With stacked munition every magazine contributes to the total amount
                return this->maxMunitionPerMagazine_ * this->magazines_ + magazine->munition_;
            else
                // Wihtout stacked munition we just consider the current magazine
                return magazine->munition_;
        }
        return 0;
    }

    unsigned int Munition::getNumMunitionInCurrentMagazine(WeaponMode* user) const
    {
        // In contrast to getNumMunition() we really just consider the current magazine, even if we're stacking munition
        Magazine* magazine = this->getMagazine(user);
        if (magazine)
            return magazine->munition_;
        else
            return 0;
    }

    unsigned int Munition::getNumMagazines() const
    {
        if (this->bStackMunition_)
        {
            // If we stack munition and the current magazine is still full, it counts too
            Magazine* magazine = this->getMagazine(0);
            if (magazine && magazine->munition_ == this->maxMunitionPerMagazine_)
                return this->magazines_ + 1;
        }

        return this->magazines_;
    }

    unsigned int Munition::getMaxMunition() const
    {
        if (this->bStackMunition_)
            return this->maxMunitionPerMagazine_ * this->maxMagazines_;
        else
            return this->maxMunitionPerMagazine_;
    }

    bool Munition::canTakeMunition(unsigned int amount, WeaponMode* user) const
    {
        Magazine* magazine = this->getMagazine(user);
        if (magazine && magazine->bLoaded_)
        {
            unsigned int munition = magazine->munition_;

            // If we stack munition, we con't care about the current magazine - we just need enough munition in total
            if (this->bStackMunition_)
                munition += this->maxMunitionPerMagazine_ * this->magazines_;

            if (munition == 0)
                // Absolutely no munition - no chance to take munition
                return false;
            else if (this->bAllowMultiMunitionRemovementUnderflow_)
                // We're not empty AND we allow underflow, so this will always work
                return true;
            else
                // We don't allow underflow, so we have to check the amount
                return (munition >= amount);
        }
        return false;
    }

    bool Munition::takeMunition(unsigned int amount, WeaponMode* user)
    {
        if (!this->canTakeMunition(amount, user))
            return false;

        Magazine* magazine = this->getMagazine(user);
        if (magazine && magazine->bLoaded_)
        {
            if (magazine->munition_ >= amount)
            {
                // Enough munition
                magazine->munition_ -= amount;
                return true;
            }
            else
            {
                // Not enough munition
                if (this->bStackMunition_)
                {
                    // We stack munition, so just take what we can and then load the next magazine
                    amount -= magazine->munition_;
                    magazine->munition_ = 0;

                    if (this->reload(0))
                        // Successfully reloaded, continue recursively
                        return this->takeMunition(amount, 0);
                    else
                        // We don't have more magazines, so let's just hope we allow underflow
                        return this->bAllowMultiMunitionRemovementUnderflow_;
                }
                else
                {
                    // We don't stack, so we can only take munition if this is allowed
                    if (magazine->munition_ > 0 && this->bAllowMultiMunitionRemovementUnderflow_)
                    {
                        magazine->munition_ = 0;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool Munition::canReload() const
    {
        // As long as we have enough magazines (and don't stack munition) we can reload
        return (this->magazines_ > 0 && !this->bStackMunition_);
    }

    bool Munition::needReload(WeaponMode* user) const
    {
        Magazine* magazine = this->getMagazine(user);
        if (magazine)
        {
            if (this->bStackMunition_)
                // With stacked munition, we never have to reload
                return false;
            else
                // We need to reload if an already loaded magazine is empty
                return (magazine->bLoaded_ && magazine->munition_ == 0);
        }
        else
            // No magazine - we definitively need to reload
            return true;
    }

    bool Munition::reload(WeaponMode* user, bool bUseReloadTime)
    {
        // Don't reload if we're already reloading
        Magazine* magazine = this->getMagazine(user);
        if (magazine && !magazine->bLoaded_)
            return false;

        // Check if we actually can reload
        if (this->magazines_ == 0)
            return false;

        // If we use separate magazines for each user, we definitively need a user given
        if (this->bUseSeparateMagazines_ && !user)
            return false;

        // If we don't use separate magazines, set user to 0
        if (!this->bUseSeparateMagazines_)
            user = 0;

        // Remove the current magazine for the given user
        std::map<WeaponMode*, Magazine*>::iterator it = this->currentMagazines_.find(user);
        if (it != this->currentMagazines_.end())
        {
            delete it->second;
            this->currentMagazines_.erase(it);
        }

        // Load a new magazine
        this->currentMagazines_[user] = new Magazine(this, bUseReloadTime);
        this->magazines_--;

        return true;
    }

    bool Munition::canAddMunition(unsigned int amount) const
    {
        // TODO: 'amount' is not used

        if (!this->bAllowMunitionRefilling_)
            return false;

        if (this->bStackMunition_)
        {
            // If we stack munition, we can always add munition until we reach the limit
            return (this->getNumMunition(0) < this->getMaxMunition());
        }
        else
        {
            // Return true if any of the current magazines is not full (loading counts as full although it returns 0 munition)
            for (std::map<WeaponMode*, Magazine*>::const_iterator it = this->currentMagazines_.begin(); it != this->currentMagazines_.end(); ++it)
                if (it->second->munition_ < this->maxMunitionPerMagazine_ && it->second->bLoaded_)
                    return true;
        }

        return false;
    }

    bool Munition::addMunition(unsigned int amount)
    {
        if (!this->canAddMunition(amount))
            return false;

        if (this->bStackMunition_)
        {
            // Stacking munition means, if a magazine gets full, the munition adds to a new magazine
            Magazine* magazine = this->getMagazine(0);
            if (magazine)
            {
                // Add the whole amount
                magazine->munition_ += amount;

                // Add new magazines if the current magazine is overfull
                while (magazine->munition_ > this->maxMunitionPerMagazine_)
                {
                    magazine->munition_ -= this->maxMunitionPerMagazine_;
                    this->magazines_++;
                }

                // If we reached the limit, reduze both magazines and munition to the maximum
                if (this->magazines_ >= this->maxMagazines_)
                {
                    this->magazines_ = this->maxMagazines_ - 1;
                    magazine->munition_ = this->maxMunitionPerMagazine_;
                }

                return true;
            }

            // Something went wrong
            return false;
        }
        else
        {
            // Share the munition equally to the current magazines
            while (amount > 0)
            {
                bool change = false;
                for (std::map<WeaponMode*, Magazine*>::iterator it = this->currentMagazines_.begin(); it != this->currentMagazines_.end(); ++it)
                {
                    // Add munition if the magazine isn't full (but only to loaded magazines)
                    if (amount > 0 && it->second->munition_ < this->maxMunitionPerMagazine_ && it->second->bLoaded_)
                    {
                        it->second->munition_++;
                        amount--;
                        change = true;
                    }
                }

                // If there was no change in a loop, all magazines are full (or locked due to loading)
                if (!change)
                    break;
            }

            return true;
        }
    }

    bool Munition::canAddMagazines(unsigned int amount) const
    {
        // TODO: 'amount' is not used

        if (this->bStackMunition_)
            // If we stack munition, we can always add new magazines because they contribute directly to the munition
            return (this->getNumMunition(0) < this->getMaxMunition());
        else
            // If we don't stack munition, we're more limited
            return ((this->currentMagazines_.size() + this->magazines_) < this->maxMagazines_);
    }

    bool Munition::addMagazines(unsigned int amount)
    {
        if (!this->canAddMagazines(amount))
            return false;

        // Calculate how many magazines are needed
        int needed_magazines = this->maxMagazines_ - this->magazines_ - this->currentMagazines_.size();

        // If zero or less magazines are needed, we definitively don't need more magazines (unless we stack munition - then a magazine contributes directly to the munition)
        if (needed_magazines <= 0 && !this->bStackMunition_)
            return false;

        if (amount <= static_cast<unsigned int>(needed_magazines))
        {
            // We need more magazines than we get, so just add them
            this->magazines_ += amount;
        }
        else
        {
            // We get more magazines than we need, so just add the needed amount
            this->magazines_ += needed_magazines;
            if (this->bStackMunition_)
            {
                // We stack munition, so the additional amount contributes directly to the munition of the current magazine
                Magazine* magazine = this->getMagazine(0);
                if (magazine)
                    magazine->munition_ = this->maxMunitionPerMagazine_;
            }
        }

        return true;
    }

    bool Munition::canRemoveMagazines(unsigned int amount) const
    {
        if (this->bStackMunition_)
        {
            if (this->magazines_ >= amount)
            {
                // We have enough magazines
                return true;
            }
            else if (this->magazines_ == amount - 1)
            {
                // We lack one magazine, check if the current magazine is still full, if yes we're fine
                Magazine* magazine = this->getMagazine(0);
                if (magazine && magazine->munition_ == this->maxMunitionPerMagazine_)
                    return true;
            }
            else
            {
                // We don't have enough magazines
                return false;
            }
        }
        else
        {
            // In case we're not stacking munition, just check the number of magazines
            return (this->magazines_ >= amount);
        }

        return false;
    }

    bool Munition::removeMagazines(unsigned int amount)
    {
        if (!this->canRemoveMagazines(amount))
            return false;

        if (this->magazines_ >= amount)
        {
            // We have enough magazines, just remove the amount
            this->magazines_ -= amount;
        }
        else if (this->bStackMunition_)
        {
            // We don't have enough magazines, but we're stacking munition, so additionally remove the bullets from the current magazine
            this->magazines_ = 0;
            Magazine* magazine = this->getMagazine(0);
            if (magazine)
                magazine->munition_ = 0;
        }

        return true;
    }

    bool Munition::dropMagazine(WeaponMode* user)
    {
        // If we use separate magazines, we need a user
        if (this->bUseSeparateMagazines_ && !user)
            return false;

        // If we don't use separate magazines, set user to 0
        if (!this->bUseSeparateMagazines_)
            user = 0;

        // Remove the current magazine for the given user
        std::map<WeaponMode*, Magazine*>::iterator it = this->currentMagazines_.find(user);
        if (it != this->currentMagazines_.end())
        {
            delete it->second;
            this->currentMagazines_.erase(it);
            return true;
        }

        return false;
    }


    /////////////////////
    // Magazine struct //
    /////////////////////
    Munition::Magazine::Magazine(Munition* munition, bool bUseReloadTime)
    {
        this->munition_ = 0;
        this->bLoaded_ = false;

        if (bUseReloadTime && munition->reloadTime_ > 0 && !munition->bStackMunition_)
        {
            const ExecutorPtr& executor = createExecutor(createFunctor(&Magazine::loaded, this));
            executor->setDefaultValues(munition);

            this->loadTimer_.setTimer(munition->reloadTime_, false, executor);
        }
        else
            this->loaded(munition);
    }

    void Munition::Magazine::loaded(Munition* munition)
    {
        this->bLoaded_ = true;
        this->munition_ = munition->maxMunitionPerMagazine_;
    }
}
