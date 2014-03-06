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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Trigger.cc
    @brief Implementation of the Trigger class.
    @ingroup NormalTrigger
*/

#include "Trigger.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "core/command/ConsoleCommand.h"

#include "Scene.h"

namespace orxonox
{

    SetConsoleCommand("Trigger", "debugFlares", &Trigger::debugFlares).defaultValues(false);

    RegisterClass(Trigger);

    /**
    @brief
        Constructor. Registers and initializes the object.
    @param creator
        The creator of the Trigger.
    */
    Trigger::Trigger(Context* context) : TriggerBase(context)
    {
        RegisterObject(Trigger);

        this->bActive_ = false;
        this->bTriggered_ = false;
        this->latestState_ = 0x0;

        this->remainingTime_ = 0.0f;
        this->timeSinceLastEvent_ = 0.0f;

        // Set the debug billboard.
        if (this->getScene() && GameMode::showsGraphics())
        {
            this->debugBillboard_.setBillboardSet(this->getScene()->getSceneManager(), "Examples/Flare", ColourValue(1.0, 0.0, 0.0), 1);
            this->debugBillboard_.setVisible(false);

            if (this->debugBillboard_.getBillboardSet())
                this->attachOgreObject(this->debugBillboard_.getBillboardSet());
        }

        this->setSyncMode(ObjectDirection::None);
    }

    /**
    @brief
        Destructor.
    */
    Trigger::~Trigger()
    {

    }

    /**
    @brief
        Method for creating a Trigger object through XML.
    */
    void Trigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Trigger, XMLPort, xmlelement, mode);
    }

    /**
    @brief

    @param dt
        The time elapsed since last tick.
    */
    void Trigger::tick(float dt)
    {
        // If this is the first tick, announce, that the trigger is not triggered.
        // This is needed, e.g. for an inverted trigger, that needs to announce at the beginning, that it is active.
        if (this->bFirstTick_)
        {
            this->bFirstTick_ = false;
            this->triggered(false);
        }

        // Check if the object is active (this is NOT Trigger::isActive()!)
        // If the object is not active we do nothing.
        if (!this->BaseObject::isActive())
            return;

        SUPER(Trigger, tick, dt);

        // Apply the invert operation.
        bool newTriggered = this->isTriggered() ^ this->getInvert();

        // Check if new triggering event is really new. (i.e. if the previous triggering state is not the same as the current)
        if ((this->latestState_ & 0x1) != newTriggered)
        {
            // Create new state
            if (newTriggered)
            {
                this->latestState_ |= 1; // Set triggered bit to 1.
                this->switchState(); // Toggle the activity of the trigger.
            }
            else
            {
                this->latestState_ &= 0xFE; // Set triggered bit to 0.
                // If this trigger is not in switched-mode (i.e. it switches its activity only if it changes from not triggered to triggered and not the other way around), the activity of the trigger is toggled.
                if (!this->getSwitch())
                    this->switchState();
            }
        }

        // If there is time remaining, i.e. there are states in the queue waiting to take effect.
        if (this->remainingTime_ > 0.0)
        {
            // Discount the last tick time from the time remaining.
            this->remainingTime_ -= dt;
            // Increase the time since the last event (the time since the last state took effect), but only when actually waiting for a state in the queue
            if (this->timeSinceLastEvent_ >= 0.0)
                this->timeSinceLastEvent_ += dt;
        }

        // If the remaining time has run out and there are states in the queue waiting to take effect.
        while (this->remainingTime_ <= 0.0 && this->stateChanges_.size() > 0)
        {
            // Get the state to take effect and apply it.
            char newState = this->stateChanges_.front().second;
            this->bTriggered_ = (newState & 0x1);
            this->bActive_ = newState & 0x2;

            // Fire a triggered (or un-triggered, depending on the activity) event.
            this->triggered(this->bActive_);

            // Remove the state that was just applied from the queue.
            this->stateChanges_.pop();

            // If there are still states in the queue, set the remaining time to the time of the next state to take effect.
            if (this->stateChanges_.size() != 0)
                this->remainingTime_ = this->stateChanges_.front().first;
            // Else the time since the last event is set to the delay.
            else
                this->timeSinceLastEvent_ = this->getDelay();
        }

        // Set the color of the debug billboard according to the current state of the trigger.
        if (this->bTriggered_ && this->bActive_)
            this->setBillboardColour(ColourValue(0.5, 1.0, 0.0));
        else if (!this->bTriggered_ && this->bActive_)
            this->setBillboardColour(ColourValue(0.0, 1.0, 0.0));
        else if (this->bTriggered_ && !this->bActive_)
            this->setBillboardColour(ColourValue(1.0, 0.5, 0.0));
        else
            this->setBillboardColour(ColourValue(1.0, 0.0, 0.0));
    }

    /**
    @brief
        Fires an event with the input triggered state.
    @param bIsTriggered
        The triggered state.
    */
    void Trigger::triggered(bool bIsTriggered)
    {
        this->fireEvent(bIsTriggered);
    }

    /**
    @brief
        Check whether the Trigger should be triggered, given only its sub-triggers, given a specific mode.
    @param mode
        The Trigger mode. Specifies how the sub-triggers are combined and how they affect the Trigger.
    @return
        Returns true if the Trigger should be triggered and false if not.
    */
    bool Trigger::isTriggered(TriggerMode::Value mode)
    {
        // If the trigger has sub-triggers.
        if (this->children_.size() > 0)
        {
            switch (mode)
            {
                case TriggerMode::EventTriggerAND:
                    return checkAnd();
                case TriggerMode::EventTriggerOR:
                    return checkOr();
                case TriggerMode::EventTriggerXOR:
                    return checkXor();
                default:
                    return false;
            }
        }
        // If the trigger has no sub-triggers, whether it is triggered should only depend on itself and nothing else, thus this returns true.
        return true;
    }

    /**
    @brief
        Check whether all the sub-triggers of this Trigger are active.
        This is in fact the conjunction (logical AND) of the activity of all its sub-triggers.
    @return
        Returns true if all the sub-triggers of this Trigger are active, false if at least one of them is not active.
    */
    bool Trigger::checkAnd()
    {
        // Iterate over all sub-triggers.
        for (std::set<TriggerBase*>::iterator it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            if (!(*it)->isActive())
                return false;
        }
        return true;
    }

    /**
    @brief
        Check whether at least one of the sub-triggers of this Trigger is active.
        This is in fact the disjunction (logical OR) of the activity of all its sub-triggers.
    @return
        Returns true if at least one of the sub-triggers of this Trigger is active, false if none of them is active.
    */
    bool Trigger::checkOr()
    {
        // Iterate over all sub-triggers.
        for (std::set<TriggerBase*>::iterator it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            if ((*it)->isActive())
                return true;
        }
        return false;
    }

    /**
    @brief
        Check whether exactly one of the sub-triggers of this Trigger is active.
        This is in fact the logical XOR of the activity of all its sub-triggers.
    @return
        Returns true if exactly one of the sub-triggers of this Trigger is active, false if none of them or two or more of them are active.
    */
    bool Trigger::checkXor()
    {
        bool test = false;
        for (std::set<TriggerBase*>::iterator it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            if (test && (*it)->isActive())
                return false;
            if ((*it)->isActive())
                test = true;
        }
        return test;
    }

    /**
    @brief
        Switch (toggle) the activity of the Trigger.
    @return
        Returns true if the activity of the Trigger has changed. False if not.
    */
    bool Trigger::switchState()
    {
        // If the previous state was active and there are no remaining activations, but the trigger stays active.
        // or if the previous state was inactive and there are no remaining activations.
        // the activity is not switched.
        if (( (this->latestState_ & 0x2) && this->getStayActive() && (this->remainingActivations_ <= 0))
           || (!(this->latestState_ & 0x2)                        && (this->remainingActivations_ == 0)))
            return false;
        // Else the activity is switched.
        else
        {
            this->latestState_ ^= 0x2; // Toggle activity bit.

            // If the activity has switched to active, decrease the remaining activations.
            if (this->latestState_ & 0x2 && this->remainingActivations_ > 0)
                this->remainingActivations_--;

            // Store the new state in the queue.
            this->storeState();

            return true;
        }
    }

    /**
    @brief
        Stores the state in the queue.
        The queue is a list of states that are waiting to take effect paired with the time it has to wait after its preceding state takes effect.
    */
    void Trigger::storeState()
    {
        // Put the state change into the queue.
        this->stateChanges_.push(std::pair<float, char>(this->timeSinceLastEvent_, this->latestState_));
        // Reset time since last event
        this->timeSinceLastEvent_ = 0.0;

        // If there is just one state in the queue. (i.e. the one that we just added), The remaining time is set to the time it takes for the next state to take effect.
        if (this->stateChanges_.size() == 1)
            this->remainingTime_ = this->stateChanges_.front().first;
    }

    /**
    @brief
        React to a change in delay.
        Only newly arriving states are affected by a change in delay.
    */
    void Trigger::delayChanged(void)
    {
        this->timeSinceLastEvent_ = this->getDelay();
    }

    /**
    @brief
        Set the visibility of all debug billboards of all Triggers.
    @param bVisible
        The visibility the billboards are set to.
    */
    void Trigger::debugFlares(bool bVisible)
    {
        // Iterate over all Triggers.
        for (ObjectList<Trigger>::iterator it = ObjectList<Trigger>::begin(); it != ObjectList<Trigger>::end(); ++it)
            it->setVisible(bVisible);
    }

    /**
    @brief
        Set the colour of the debug billboard.
    @param colour
        The colour the billboard is set to.
    */
    void Trigger::setBillboardColour(const ColourValue& colour)
    {
        this->debugBillboard_.setColour(colour);
    }

    /**
    @brief
        React to a change of visibility of the trigger by adjusting the visibility of the debug billboards.
    */
    void Trigger::changedVisibility()
    {
        SUPER(Trigger, changedVisibility);

        this->debugBillboard_.setVisible(this->isVisible());
    }
}
