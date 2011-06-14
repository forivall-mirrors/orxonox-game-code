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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
*/

/**
    @file MultiTrigger.cc
    @brief Implementation of the MultiTrigger class.
    @ingroup MultiTrigger
*/

#include "MultiTrigger.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "MultiTriggerContainer.h"

namespace orxonox
{

    CreateFactory(MultiTrigger);

    /**
    @brief
        Constructor. Registers the objects and initializes default values.
    @param creator
        The creator.
    */
    MultiTrigger::MultiTrigger(BaseObject* creator) : TriggerBase(creator)
    {
        RegisterObject(MultiTrigger);

        this->maxNumSimultaneousTriggerers_ = INF_s;

        this->bBroadcast_ = false;

        this->targetMask_.exclude(Class(BaseObject));

        this->bMultiTrigger_ = true;

        this->setSyncMode(ObjectDirection::None);
    }

    /**
    @brief
        Destructor. Cleans up the state queue.
    */
    MultiTrigger::~MultiTrigger()
    {
        COUT(4) << "Destroying MultiTrigger &" << this << ". " << this->stateQueue_.size() << " states still in queue. Deleting." << std::endl;
        while(this->stateQueue_.size() > 0)
        {
            MultiTriggerState* state = this->stateQueue_.front().second;
            this->stateQueue_.pop_front();
            delete state;
        }
    }

    /**
    @brief
        Method for creating a MultiTrigger object through XML.
        For a detailed description of the parameters please see the class description in the header file.
    */
    void MultiTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MultiTrigger, XMLPort, xmlelement, mode);

        XMLPortParam(MultiTrigger, "simultaneousTriggerers", setSimultaneousTriggerers, getSimultaneousTriggerers, xmlelement, mode);
        XMLPortParam(MultiTrigger, "broadcast", setBroadcast, getBroadcast, xmlelement, mode);
        XMLPortParamLoadOnly(MultiTrigger, "target", addTarget, xmlelement, mode).defaultValues("Pawn"); //TODO: Remove load only

        COUT(4) << "MultiTrigger '" << this->getName() << "' (&" << this << ") created." << std::endl;
    }


    /**
    @brief
        A method that is executed each tick.
        Most of the magic of MultiTriggers happens here.
    @param dt
        The duration of the last tick.
    */
    //TODO: Segment into some helper methods?
    void MultiTrigger::tick(float dt)
    {
        // If this is the first tick.
        if(this->bFirstTick_)
        {
            this->bFirstTick_ = false;
            // Fire for all objects that are targets.
            this->broadcast(false);
        }

        // Check if the object is active (this is NOT MultiTrigger::isActive()!), it is whether the MultiTrigger actually does anything, ever.
        if (!this->BaseObject::isActive())
            return;

        SUPER(MultiTrigger, tick, dt);

        // Let the MultiTrigger return the states that trigger and process the new states if there are any.
        std::queue<MultiTriggerState*>* queue  = this->letTrigger();
        if(queue != NULL)
        {
            while(queue->size() > 0)
            {
                MultiTriggerState* state = queue->front();
                // If the state is NULL. (This really shouldn't happen)
                if(state == NULL)
                {
                    COUT(1) << "In MultiTrigger '" << this->getName() << "' (&" << this << "), Error: State of new states queue was NULL. State ignored." << std::endl;
                    queue->pop();
                    continue;
                }

                // The new triggered state dependent on the requested state, the mode and the invert-mode.
                bool bTriggered = (state->bTriggered & this->isModeTriggered(state->originator)) ^ this->getInvert();

                // If the 'triggered' state has changed or the MultiTrigger has delay and thus we don't know whether this state will actually change the 'triggered' state, a new state is added to the state queue.
                if(this->getDelay() > 0.0f || bTriggered ^ this->isTriggered(state->originator))
                {
                    state->bTriggered = bTriggered;
                    this->addState(state);
                }
                // Else the change is irrelevant.
                else
                    delete state;

                queue->pop();
            }
            delete queue;
        }

        // Go through the state queue and activate all pending states whose remaining time has expired.
        if(this->stateQueue_.size() > 0)
        {
            MultiTriggerState* state;
            float timeRemaining;

            // Go through all pending states.
            for(int size = this->stateQueue_.size(); size >= 1; size--)
            {
                timeRemaining = this->stateQueue_.front().first;
                state = this->stateQueue_.front().second;

                // If the remaining time has expired, the state has to be set as the state of the MultiTrigger.
                if(timeRemaining <= dt)
                {
                    // If the maximum number of objects simultaneously triggering this MultiTrigger is not exceeded.
                    if(this->getSimultaneousTriggerers() == TriggerBase::INF_s || this->triggered_.size() < (unsigned int)this->getSimultaneousTriggerers())
                    {
                        bool bStateChanged = false;
                        // If the 'triggered' state is different from what it is now, change it.
                        if(state->bTriggered ^ this->isTriggered(state->originator))
                        {
                            // Add the originator to the objects triggering this MultiTrigger.
                            if(state->bTriggered == true)
                                this->triggered_.insert(state->originator);
                            // Remove the originator from the objects triggering this MultiTrigger.
                            else
                                this->triggered_.erase(state->originator);

                            bStateChanged = true;
                        }

                        // Get the activity of the new state.
                        bool bActive;
                        // If the MultiTrigger is in switch mode the 'active'-state only changes of the state changed to triggered.
                        if(this->getSwitch() && !state->bTriggered)
                            bActive = this->isActive(state->originator);
                        else
                            bActive = !this->isActive(state->originator);

                        // If the activity is different from what it is now, change it and fire an Event.
                        if(bActive ^ this->isActive(state->originator))
                        {
                            bool bFire = true;

                            // Add the originator to the objects activating this MultiTrigger.
                            if(bActive == true)
                            {
                                // If the MultiTrigger has not exceeded its remaining activations.
                                if(this->hasRemainingActivations())
                                {
                                    this->active_.insert(state->originator);
                                    if(this->remainingActivations_ != INF_s)
                                        this->remainingActivations_--; // Decrement the remaining activations.
                                }
                                else
                                    bFire = false;
                            }
                            // Remove the originator from the objects activating this MultiTrigger.
                            else
                            {
                                // If the MultiTrigger doesn't stay active or hasn't' exceeded its remaining activations.
                                if(!this->getStayActive() || this->hasRemainingActivations())
                                    this->active_.erase(state->originator);
                                else
                                    bFire = false;
                            }

                            // Fire the Event if the activity has changed.
                            if(bFire)
                            {
                                // If the MultiTrigger is set to broadcast and has no originator a boradcast is fired.
                                if(this->getBroadcast() && state->originator == NULL)
                                    this->broadcast(bActive);
                                // Else a normal event is fired.
                                else
                                    this->fire(bActive, state->originator);

                                bStateChanged = true;
                            }
                        }

                        if(bStateChanged)
                        {
                            // Print some debug output if the state has changed.
                            if(state->originator != NULL)
                                COUT(4) << "MultiTrigger '" << this->getName() << "' (&" << this << ") changed state. originator: " << state->originator->getIdentifier()->getName() << " (&" << state->originator << "), active: " << bActive << ", triggered: " << state->bTriggered << "." << std::endl;
                            else
                                COUT(4) << "MultiTrigger '" << this->getName() << "' (&" << this << ") changed state. originator: NULL, active: " << bActive << ", triggered: " << state->bTriggered << "." << std::endl;

                            // If the MultiTrigger has a parent trigger, that is itself a MultiTrigger, it needs to call a method to notify him, that its activity has changed.
                            if(this->parent_ != NULL && this->parent_->isMultiTrigger())
                                static_cast<MultiTrigger*>(this->parent_)->childActivityChanged(state->originator);
                        }

                        // If the MultiTrigger has exceeded its amount of activations and it doesn't stay active, it has to be deactivated.
                        if(this->remainingActivations_ == 0 && !bActive)
                        {
                            this->BaseObject::setActive(false);
                            COUT(4) << "MultiTrigger '" << this->getName() << "' (&" << this << ") ran out of activations. Setting it to inactive." << std::endl;
                        }
                    }

                    // Remove the state from the state queue.
                    this->stateQueue_.pop_front();
                    delete state;
                }
                // If the remaining time has not yet expired. Decrement the remainig time and put the state at the end of the queue.
                else
                {
                    this->stateQueue_.push_back(std::pair<float, MultiTriggerState*>(timeRemaining-dt, state));
                    this->stateQueue_.pop_front();
                }
            }
        }
    }

    /**
    @brief
        Check whether the MultiTrigger is active for a given object.
    @param triggerer
        A pointer to the object.
    @return
        Returns true if the MultiTrigger is active, false if not.
    */
    bool MultiTrigger::isActive(BaseObject* triggerer) const
    {
        std::set<BaseObject*>::const_iterator it = this->active_.find(triggerer);
        if(it == this->active_.end())
            return false;
        return true;
    }

    /**
    @brief
        Add some target to the MultiTrigger.
    @param targetStr
        The target class name as a string.
    */
    void MultiTrigger::addTarget(const std::string& targetStr)
    {
        Identifier* target = ClassByString(targetStr);

        // If the target is not a valid class name display an error.
        if (target == NULL)
        {
            COUT(1) << "Error: '" << targetStr << "' is not a valid class name to include in ClassTreeMask (in " << this->getName() << ", class " << this->getIdentifier()->getName() << ")" << std::endl;
            return;
        }

        this->targetMask_.include(target);

        // A MultiTrigger shouldn't react to itself or other triggers.
        this->targetMask_.exclude(Class(TriggerBase), true);

        // We only want WorldEntities
        ClassTreeMask WEMask;
        WEMask.include(Class(WorldEntity));
        this->targetMask_ *= WEMask;
    }

    /**
    @brief
        Remove some target from the MultiTrigger.
    @param targetStr
        The target to be removed as a string.
    */
    void MultiTrigger::removeTarget(const std::string& targetStr)
    {
        Identifier* target = ClassByString(targetStr);

        // If the target is not a valid class name display an error.
        if (target == NULL)
        {
            COUT(1) << "Error: '" << targetStr << "' is not a valid class name to include in ClassTreeMask (in " << this->getName() << ", class " << this->getIdentifier()->getName() << ")" << std::endl;
            return;
        }

        this->targetMask_.exclude(target);
    }

    /**
    @brief
        This method is called by the MultiTrigger to get information about new trigger events that need to be looked at.
        This method is the device for the behavior (the conditions under which the MultiTrigger triggers) of any derived class of MultiTrigger.
    @return
        Returns a pointer to a queue of MultiTriggerState pointers, containing all the necessary information to decide whether these states should indeed become new states of the MultiTrigger.
        Please be aware that both the queue and the states in the queue need to be deleted once they have been used. This is already done in the tick() method of this class but would have to be done by any method calling this method.
    */
    std::queue<MultiTriggerState*>* MultiTrigger::letTrigger(void)
    {
        return NULL;
    }

    /**
    @brief
        This method can be called by any class inheriting from MultiTrigger to change it's triggered status for a specified originator.

        Compared to the letTrigger mode, which just polls and lets the pollee send it's state changed back, the changeTriggered method lets the trigger advertise its state changes just as they happen so it's much like the interrupt version of letTrigger.
    @param originator
        The originator which triggered the state change.
    */
    void MultiTrigger::changeTriggered(BaseObject* originator)
    {
        MultiTriggerState* state = new MultiTriggerState;
        state->bTriggered = (!this->isTriggered(originator) & this->isModeTriggered(originator)) ^ this->getInvert();
        state->originator = originator;
        this->addState(state);
    }

    /**
    @brief
        This method is called by any child to advertise changes in its state to its parent.
    @param originator
        The object that caused the change in activity.
    */
    void MultiTrigger::childActivityChanged(BaseObject* originator)
    {
        MultiTriggerState* state = new MultiTriggerState;
        state->bTriggered = (this->isTriggered(originator) & this->isModeTriggered(originator)) ^ this->getInvert();
        state->originator = originator;
        this->addState(state);
    }

    /**
    @brief
        Checks whether the children are in such a way that, according to the mode of the MultiTrigger, the MultiTrigger is triggered (only considering the children, not the state of MultiTrigger itself), for a given object.
        To make an example: When the mode is <em>and</em>, then this would be true or a given object if all the children were triggered for the given object.
    @param triggerer
        The object.
    @return
        Returns true if the MultiTrigger is triggered concerning it's children.
    */
    bool MultiTrigger::isModeTriggered(BaseObject* triggerer)
    {
        if(this->children_.size() != 0)
        {
            bool triggered = false;

            switch(this->mode_)
            {
                case TriggerMode::EventTriggerAND:
                    triggered = checkAnd(triggerer);
                    break;
                case TriggerMode::EventTriggerOR:
                    triggered = checkOr(triggerer);
                    break;
                case TriggerMode::EventTriggerXOR:
                    triggered = checkXor(triggerer);
                    break;
                default: // This will never happen.
                    triggered = false;
                    break;
            }

            return triggered;
        }

        return true;
    }

    /**
    @brief
        Get whether the MultiTrigger is triggered for a given object.
    @param triggerer
        The object.
    @return
        Returns true if the MultiTrigger is triggered for the given object.
    */
    bool MultiTrigger::isTriggered(BaseObject* triggerer)
    {
        std::set<BaseObject*>::iterator it = this->triggered_.find(triggerer);
        if(it == this->triggered_.end())
            return false;
        return true;
    }

    /**
    @brief
        Helper method. Creates an Event for the given status and originator and fires it.
        Or more precisely creates a MultiTriggerContainer to encompass all neccesary information, creates an Event from that and sends it.
    @param status
        The status of the Event to be fired. This is equivalent to the activity of the MultiTrigger.
    @param originator
        The object that triggered the MultiTrigger to fire this Event.
    */
    void MultiTrigger::fire(bool status, BaseObject* originator)
    {
        // If the originator is NULL, a normal event without MultiTriggerContainer is sent.
        if(originator == NULL)
        {
            this->fireEvent(status);
            COUT(4) << "MultiTrigger '" <<  this->getName() << "' (&" << this << "): Fired event. status: " << status << "." << std::endl;
            return;
        }

        MultiTriggerContainer* container = new MultiTriggerContainer(this, this, originator);
        this->fireEvent(status, container);
        COUT(4) << "MultiTrigger '" <<  this->getName() << "' (&" << this << "): Fired event. originator: " << originator->getIdentifier()->getName() << " (&" << originator << "), status: " << status << "." << std::endl;
        delete container;
    }

    /**
    @brief
        Helper method. Broadcasts an Event for every object that is a target.
    @param status
        The status of the Events to be fired. This is equivalent to the activity of the MultiTrigger.
    */
    void MultiTrigger::broadcast(bool status)
    {
        for(ClassTreeMaskObjectIterator it = this->getTargetMask().begin(); it != this->getTargetMask().end(); ++it)
            this->fire(status, static_cast<BaseObject*>(*it));
    }

    /**
    @brief
        Helper method. Adds a state to the state queue, where the state will wait to become active.
    @param state
        The state to be added.
    @return
        Returns true if the state has been added, false if not. If the state has not been added this method destroys it.
    */
    bool MultiTrigger::addState(MultiTriggerState* state)
    {
        assert(state); // The state really shouldn't be NULL.

        // If the originator is no target of this MultiTrigger.
        if(!this->isTarget(state->originator))
        {
            delete state;
            return false;
        }

        // Add it ot the state queue with the delay specified for the MultiTrigger.
        this->stateQueue_.push_back(std::pair<float, MultiTriggerState*>(this->getDelay(), state));

        return true;
    }

    /**
    @brief
        Checks whether the children amount to true for the <em>and</em> mode for a given object.
    @param triggerer
        The object.
    @return
        Returns true if all the sub-triggers are active.
    */
    bool MultiTrigger::checkAnd(BaseObject* triggerer)
    {
        for(std::set<TriggerBase*>::iterator it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            TriggerBase* trigger = *it;
            if(trigger->isMultiTrigger())
            {
                if(!static_cast<MultiTrigger*>(trigger)->isActive(triggerer))
                    return false;
            }
            else
            {
                if(!trigger->isActive())
                    return false;
            }
        }
        return true;
    }

    /**
    @brief
        Checks whether the children amount to true for the <em>or</em> mode for a given object.
    @param triggerer
        The object.
    @return
        Returns true if at least one sub-trigger is active.
    */
    bool MultiTrigger::checkOr(BaseObject* triggerer)
    {
        for(std::set<TriggerBase*>::iterator it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            TriggerBase* trigger = *it;
            if(trigger->isMultiTrigger())
            {
                if(static_cast<MultiTrigger*>(trigger)->isActive(triggerer))
                    return true;
            }
            else
            {
                if(trigger->isActive())
                    return true;
            }
        }
        return false;
    }

    /**
    @brief
        Checks whether the children amount to true for the <em>xor</em> mode for a given object.
    @param triggerer
        The object.
    @return
        Returns true if exactly one sub-trigger is active.
    */
    bool MultiTrigger::checkXor(BaseObject* triggerer)
    {
        bool triggered = false;
        for(std::set<TriggerBase*>::iterator it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            TriggerBase* trigger = *it;
            if(triggered)
            {
                if(trigger->isMultiTrigger())
                {
                    if(static_cast<MultiTrigger*>(trigger)->isActive(triggerer))
                        return false;
                }
                else
                {
                    if(trigger->isActive())
                        return false;
                }
            }

            if(trigger->isMultiTrigger())
            {
                if(static_cast<MultiTrigger*>(trigger)->isActive(triggerer))
                    triggered = true;
            }
            else
            {
                if(trigger->isActive())
                    triggered = true;
            }
        }
        return triggered;
    }

}
