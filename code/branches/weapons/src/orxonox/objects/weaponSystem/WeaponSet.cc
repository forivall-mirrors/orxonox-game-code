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
 *   Author:
 *      Martin Polak
 *   Co-authors:
 *      ... *
 */

#include "OrxonoxStableHeaders.h"
#include "WeaponSet.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "WeaponSystem.h"
#include "WeaponPack.h"

namespace orxonox
{
    CreateFactory(WeaponSet);

    WeaponSet::WeaponSet(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(WeaponSet);

        this->weaponSystem_ = 0;
        this->desiredFiremode_ = WeaponSystem::FIRE_MODE_UNASSIGNED;

COUT(0) << "+WeaponSet" << std::endl;
    }

    WeaponSet::~WeaponSet()
    {
COUT(0) << "~WeaponSet" << std::endl;

        if (this->isInitialized() && this->weaponSystem_)
            this->weaponSystem_->removeWeaponSet(this);
    }

    void WeaponSet::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WeaponSet, XMLPort, xmlelement, mode);

        XMLPortParam(WeaponSet, "firemode", setDesiredFiremode, getDesiredFiremode, xmlelement, mode);
    }
/*
    void WeaponSet::attachWeaponPack(WeaponPack *wPack)
    {
        if ( this->weaponSystem_->getWeaponSlotSize()>0 && wPack->getSize()>0 && ( wPack->getSize() <= this->weaponSystem_->getWeaponSlotSize() ) )
        {
            this->attachedWeaponPack_ = wPack;
            int wPackWeapon = 0;    //WeaponCounter for Attaching

            //should be possible to choose which slot to use
            //attach every weapon of the weaponPack to a weaponSlot
            for (  int i=0; i < wPack->getSize() ; i++  )
            {
                //at the moment this function only works for one weaponPack in the entire WeaponSystem...
                //it also takes the first free weaponSlot...
                if ( this->weaponSystem_->getWeaponSlot(i)->getAttachedWeapon() == 0 && this->weaponSystem_->getWeaponSlot(i) != 0) //if slot not full
                {
                    this->setWeaponSlots_.push_back( this->weaponSystem_->getWeaponSlot(i) );
                    this->weaponSystem_->getWeaponSlot(i)->attachWeapon( wPack->getWeaponPointer(wPackWeapon) );
                    this->weaponSystem_->getPawn()->attach( wPack->getWeaponPointer(wPackWeapon) );
                    wPackWeapon++;
                }
                else
                {
                    for (int k=0; k < this->weaponSystem_->getWeaponSlotSize(); k++)
                    {
                        if ( this->weaponSystem_->getWeaponSlot(k)->getAttachedWeapon() == 0 )
                        {
                            this->setWeaponSlots_.push_back( this->weaponSystem_->getWeaponSlot(k) );
                            this->weaponSystem_->getWeaponSlot(k)->attachWeapon( wPack->getWeaponPointer(wPackWeapon) );
                            this->weaponSystem_->getPawn()->attach( wPack->getWeaponPointer(wPackWeapon) );
                            wPackWeapon++;
                        }
                    }
                }
            }
        }
    }
*/

    void WeaponSet::fire()
    {
        // fire all WeaponPacks with their defined weaponmode
        for (std::map<WeaponPack*, unsigned int>::iterator it = this->weaponpacks_.begin(); it != this->weaponpacks_.end(); ++it)
            if (it->second != WeaponSystem::WEAPON_MODE_UNASSIGNED)
                it->first->fire(it->second);
    }

    void WeaponSet::setWeaponmodeLink(WeaponPack* weaponpack, unsigned int weaponmode)
    {
        this->weaponpacks_[weaponpack] = weaponmode;
    }

    void WeaponSet::removeWeaponmodeLink(WeaponPack* weaponpack)
    {
        this->weaponpacks_.erase(weaponpack);
    }

    unsigned int WeaponSet::getWeaponmodeLink(WeaponPack* weaponpack)
    {
        std::map<WeaponPack*, unsigned int>::iterator it = this->weaponpacks_.find(weaponpack);
        if (it != this->weaponpacks_.end())
            return it->second;
        else
            return WeaponSystem::WEAPON_MODE_UNASSIGNED;
    }
}
