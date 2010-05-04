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
 *      Benjamin Knecht
 *
*/

/**
    @file MultiTrigger.cc
    @brief Implementation of the MultiTrigger class.
*/

#include "MultiTrigger.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "MultiTriggerContainer.h"

namespace orxonox
{

    // Initialization of some static (magic) variables.
    /*static*/ const int MultiTrigger::INF_s = -1;
    /*static*/ const std::string MultiTrigger::and_s = "and";
    /*static*/ const std::string MultiTrigger::or_s = "or";
    /*static*/ const std::string MultiTrigger::xor_s = "xor";
    
    CreateFactory(MultiTrigger);

    /**
    @brief
        Constructor. Registers the objects and initializes default values.
    @param creator
        The creator.
    */
    MultiTrigger::MultiTrigger(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(MultiTrigger);

        this->bFirstTick_ = true;

        this->delay_ = 0.0f;
        this->bSwitch_ = false;
        this->bStayActive_ = false;
        
        this->remainingActivations_ = INF_s;
        this->maxNumSimultaniousTriggerers_ = INF_s;

        this->bInvertMode_ = false;
        this->mode_ = MultiTriggerMode::EventTriggerAND;
        
        this->targetMask_.exclude(Class(BaseObject));

        this->setSyncMode(0x0);
    }
    
    /**
    @brief
        Destructor. Cleans up the state queue.
    */
    MultiTrigger::~MultiTrigger()
    {
        COUT(4) << "Destorying MultiTrigger &" << this << ". " << this->stateQueue_.size() << " states still in queue. Deleting." << std::endl;
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

        XMLPortParam(MultiTrigger, "delay", setDelay, getDelay, xmlelement, mode);
        XMLPortParam(MultiTrigger, "switch", setSwitch, getSwitch, xmlelement, mode);
        XMLPortParam(MultiTrigger, "stayactive", setStayActive, getStayActive, xmlelement, mode);
        XMLPortParam(MultiTrigger, "activations", setActivations, getActivations, xmlelement, mode);
        XMLPortParam(MultiTrigger, "simultaniousTriggerers", setSimultaniousTriggerers, getSimultaniousTriggerers, xmlelement, mode);
        XMLPortParam(MultiTrigger, "invert", setInvert, getInvert, xmlelement, mode);
        XMLPortParamTemplate(MultiTrigger, "mode", setMode, getModeString, xmlelement, mode, const std::string&);
        XMLPortParamLoadOnly(MultiTrigger, "target", addTargets, xmlelement, mode).defaultValues("ControllableEntity"); //TODO: Remove load only

        //TODO: Maybe nicer with explicit subgroup, e.g. triggers
        XMLPortObject(MultiTrigger, MultiTrigger, "", addTrigger, getTrigger, xmlelement, mode);
        
        COUT(4) << "MultiTrigger '" << this->getName() << "' (&" << this << ") created." << std::endl;
    }
    

    /**
    @brief
        A method that is executed each tick.
    @param dt
        The duration of the last tick.
    */
    void MultiTrigger::tick(float dt)
    {
        // If this is the first tick.
        //TODO: Determine need for this, else kick it out. 
        if(this->bFirstTick_)
        {
            this->bFirstTick_ = false;
            this->fire(false);
        }

        // Check if the object is active (this is NOT MultiTrigger::isActive()!)
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
                    COUT(1) << "In MultiTrigger '" << this->getName() << "' (&" << this << "), Error: State of new states queue was NULL." << std::endl;
                    queue->pop();
                    continue;
                }

                // The new triggered state dependent on the requested state, the mode and the invert-mode.
                bool bTriggered = (state->bTriggered & this->isModeTriggered(state->originator)) ^ this->bInvertMode_;

                // If the 'triggered' state has changed a new state is added to the state queue.
                //TODO: Do something against flooding, when there is delay.
                if(this->delay_ != 0.0f || bTriggered ^ this->isTriggered(state->originator))
                {
                    state->bTriggered = bTriggered;
                    this->addState(state);
                }
                else
                    delete state;
                
                queue->pop();
            }
            delete queue;
        }

        // Go through the state queue and activate all pending states whose remaining time has expired.
        if (this->stateQueue_.size() > 0)
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
                    // If the maximum number of objects simultaniously triggering this MultiTrigger is not exceeded.
                    if(this->maxNumSimultaniousTriggerers_ == INF_s || this->triggered_.size() < (unsigned int)this->maxNumSimultaniousTriggerers_)
                    {
                        bool bStateChanged = false;
                        // If the 'triggered' state is different form what it is now, change it.
                        if(state->bTriggered ^ this->isTriggered(state->originator))
                        {
                            // Add the originator to the objects triggering this MultiTrigger.
                            if(state->bTriggered == true)
                            {
                                this->triggered_.insert(state->originator);
                            }
                            // Remove the originator from the objects triggering this MultiTrigger.
                            else
                            {
                                this->triggered_.erase(state->originator);
                            }
                            
                            bStateChanged = true;
                        }

                        // Get the activity of the new state.
                        bool bActive;
                        // If the MultiTrigger is in switch mode.
                        if(this->bSwitch_ && !state->bTriggered)
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
                                if(this->remainingActivations_ != 0)
                                {
                                    this->active_.insert(state->originator);
                                    if(this->remainingActivations_ != INF_s)
                                        this->remainingActivations_--; // Decrement the remaining activations.
                                }
                                else
                                {
                                    bFire = false;
                                }
                            }
                            // Remove the originator from the objects activating this MultiTrigger.
                            else
                            {
                                if(!this->bStayActive_ || this->remainingActivations_ != 0)
                                {
                                    this->active_.erase(state->originator);
                                }
                                else
                                {
                                    bFire = false;
                                }
                            }

                            // Fire the Event if the activity has changed.
                            if(bFire)
                            {
                                this->fire(bActive, state->originator);
                                bStateChanged = true;
                            }
                        }

                        // Print some debug output if the state has changed.
                        if(bStateChanged)
                            COUT(4) << "MultiTrigger '" << this->getName() << "' (&" << this << ") changed state. originator: " << state->originator->getIdentifier()->getName() << " (&" << state->originator << "), active: " << bActive << ", triggered: " << state->bTriggered << "." << std::endl;

                        // If the MultiTrigger has exceeded its amount of activations and it doesn't stay active, it has to be destroyed,
                        if(this->remainingActivations_ == 0 && !bActive)
                        {
                            this->BaseObject::setActive(false);
                            COUT(4) << "MultiTrigger '" << this->getName() << "' (&" << this << ") ran out of activations. Setting it to inactive." << std::endl;
                        }
                    }
                    
                    // Remove the state from the state queue.
                    this->stateQueue_.pop_front();
                    delete state;
                    size -= 1;
                }
                // If the remaining time has not yet expired. Decrement the remainig time.
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
        Get whether the MultiTrigger is active for a given object.
    @param triggerers
        A pointer to the object.
    @return
        Returns true if the MultiTrigger is active, false if not.
    */
    bool MultiTrigger::isActive(BaseObject* triggerer)
    {
        std::set<BaseObject*>::iterator it = this->active_.find(triggerer);
        if(it == this->active_.end())
            return false;
        return true;
    }

    /**
    @brief
        Set the mode of the MultiTrigger.
    @param modeName
        The name of the mode as a string.
    */
    void MultiTrigger::setMode(const std::string& modeName)
    {
        if (modeName == MultiTrigger::and_s)
            this->setMode(MultiTriggerMode::EventTriggerAND);
        else if (modeName == MultiTrigger::or_s)
            this->setMode(MultiTriggerMode::EventTriggerOR);
        else if (modeName == MultiTrigger::xor_s)
            this->setMode(MultiTriggerMode::EventTriggerXOR);
    }

    /**
    @brief
        Get the mode of the MultiTrigger.
    @return
        Returns the mode as a string.
    */
    const std::string& MultiTrigger::getModeString() const
    {
        if (this->mode_ == MultiTriggerMode::EventTriggerAND)
            return MultiTrigger::and_s;
        else if (this->mode_ == MultiTriggerMode::EventTriggerOR)
            return MultiTrigger::or_s;
        else if (this->mode_ == MultiTriggerMode::EventTriggerXOR)
            return MultiTrigger::xor_s;
        else
            return MultiTrigger::and_s;
    }

    /**
    @brief
        Add some target to the MultiTrigger.
    @param targetStr
        The target as a string.
    */
    void MultiTrigger::addTargets(const std::string& targetStr)
    {
        Identifier* target = ClassByString(targetStr);

        if (target == NULL)
        {
            COUT(1) << "Error: \"" << targetStr << "\" is not a valid class name to include in ClassTreeMask (in " << this->getName() << ", class " << this->getIdentifier()->getName() << ')' << std::endl;
            return;
        }

        this->targetMask_.include(target);

        // A MultiTriggers shouldn't react to itself or other MultiTriggers.
        this->targetMask_.exclude(Class(MultiTrigger), true);

        // We only want WorldEntities
        ClassTreeMask WEMask;
        WEMask.include(Class(WorldEntity));
        this->targetMask_ *= WEMask;

        this->notifyMaskUpdate();
    }

    /**
    @brief
        Remove some target from the MultiTrigger.
    @param targetStr
        The target to be removed as a string.
    */
    void MultiTrigger::removeTargets(const std::string& targetStr)
    {
        Identifier* target = ClassByString(targetStr);
        this->targetMask_.exclude(target);
    }

    /**
    @brief
        Adds a MultiTrigger as a sub-trigger to the trigger.
        Beware: Loops are not prevented and potentially very bad, so just don't create any loops.
    @param trigger
        The trigger to be added.
    */
    void MultiTrigger::addTrigger(MultiTrigger* trigger)
    {
        if (this != trigger && trigger != NULL)
            this->subTriggers_.insert(trigger);
    }

    /**
    @brief
        Get the sub-trigger of this MultiTrigger at the given index.
    @param index
        The index.
    @return
        Returns a pointer ot the MultiTrigger. NULL if no such trigger exists.
    */
    const MultiTrigger* MultiTrigger::getTrigger(unsigned int index) const
    {
        // If the index is greater than the number of sub-triggers.
        if (this->subTriggers_.size() <= index)
            return NULL;

        std::set<MultiTrigger*>::const_iterator it;
        it = this->subTriggers_.begin();

        for (unsigned int i = 0; i != index; ++i)
            ++it;

        return (*it);
    }

    /**
    @brief
        This method is called by the MultiTrigger to get information about new trigger events that need to be looked at.
        This method is the device for the behaviour (the conditions under which the MultiTrigger triggers) of any derived class from MultiTrigger.

        In this implementation it collects all objects triggering all sub-triggers nd the MultiTrigger itself and creates a state for each of them.
    @return
        A pointer to a queue of MultiTriggerState pointers is returned, containing all the neccessary information to decide whether these states should indeed become new states of the MultiTrigger.
        Please be aware that both the queue and the states in the queue need to be deleted one they have been used. This is already done in the tick() method of this class but would have to be done by any method calling this method.
    */
    std::queue<MultiTriggerState*>* MultiTrigger::letTrigger(void)
    {
        // Goes through all sub-triggers and gets the objects triggering them.
        std::set<BaseObject*>* triggerers = new std::set<BaseObject*>();
        std::set<BaseObject*>::iterator objIt;
        for(std::set<MultiTrigger*>::iterator it = this->subTriggers_.begin(); it != this->subTriggers_.end(); it ++)
        {
            std::set<BaseObject*> set = (*it)->getActive();
            for(objIt = set.begin(); objIt != set.end(); objIt++)
            {
                triggerers->insert(*objIt);
            }
        }

        // Goes through all the triggerers of this trigger.
        for(objIt = this->active_.begin(); objIt != this->active_.end(); objIt++)
        {
            triggerers->insert(*objIt);
        }

        // If no objects are triggering this MultiTrigger or the sub-triggers.
        if(triggerers->size() == 0)
            return NULL;

        // Create a state for each object triggering this MultiTrigger or any of the sub-triggers and append it to the queue.
        std::queue<MultiTriggerState*>* queue = new std::queue<MultiTriggerState*>();
        MultiTriggerState* state = NULL;
        for(std::set<BaseObject*>::iterator it = triggerers->begin(); it != triggerers->end(); it++)
        {
            state = new MultiTriggerState;
            state->bTriggered = true;
            state->originator = *it;
            queue->push(state);
        }
        delete triggerers;

        return queue;
    }

    /**
    @brief
        Checks whether the sub-triggers are in such a way that, according to the mode of the MultiTrigger, the MultiTrigger is triggered (only considering the sub-triggers, not the state of MultiTrigger itself), for a given object.
        To make an example: When the mode is 'and', then this would be true or a given object if all the sub-triggers were triggered ofr the given object.
    @param triggerer
        The object.
    @return
        Returns true if the MultiTrigger is triggered concerning it's sub-triggers.
    */
    bool MultiTrigger::isModeTriggered(BaseObject* triggerer)
    {
        if (this->subTriggers_.size() != 0)
        {
            bool returnVal = false;

            switch (this->mode_)
            {
                case MultiTriggerMode::EventTriggerAND:
                    returnVal = checkAnd(triggerer);
                    break;
                case MultiTriggerMode::EventTriggerOR:
                    returnVal = checkOr(triggerer);
                    break;
                case MultiTriggerMode::EventTriggerXOR:
                    returnVal = checkXor(triggerer);
                    break;
                default:
                    returnVal = false;
                    break;
            }

            return returnVal;
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
        Helper method. Creates an event for the given status and originator and fires it.
        Or more precisely creates a MultiTriggerContainer to encompass all neccesary information and creates an Event from that and sends it.
    @param status
        The status of the event to be fired. This is equivalent to the activity of the MultiTrigger.
    @param originator
        The object that triggered the MultiTrigger to fire this Event.
    */
    void MultiTrigger::fire(bool status, BaseObject* originator)
    {
        // If the originator is NULL, a normal event without MultiTriggerContainer is sent.
        if(originator == NULL)
        {
            this->fireEvent(status);
            return;
        }
        
        MultiTriggerContainer* container = new MultiTriggerContainer(this, this, originator);
        this->fireEvent(status, container);
        COUT(4) << "MultiTrigger '" <<  this->getName() << "' (&" << this << "): Fired event. originator: " << originator->getIdentifier()->getName() << " (&" << originator << "), status: " << status << "." << std::endl;
        delete container;
    }

    /**
    @brief
        Helper method. Adds a state to the state queue, where the state will wait to become active.
    @param state
        The state to be added.
    */
    bool MultiTrigger::addState(MultiTriggerState* state)
    {
        assert(state);
        
        // If the originator is no target of this MultiTrigger.
        if(!this->isTarget(state->originator))
            return false;
        
        // Add it ot the state queue.
        this->stateQueue_.push_back(std::pair<float, MultiTriggerState*>(this->delay_, state));

        return true;
    }

    /**
    @brief
        Checks whether the sub-triggers amount to true for the 'and' mode for a given object.
    @param triggerer
        The object.
    @return
        Returns true if they do.
    */
    bool MultiTrigger::checkAnd(BaseObject* triggerer)
    {
        std::set<MultiTrigger*>::iterator it;
        for(it = this->subTriggers_.begin(); it != this->subTriggers_.end(); ++it)
        {
            if (!(*it)->isActive(triggerer))
                return false;
        }
        return true;
    }

    /**
    @brief
        Checks whether the sub-triggers amount to true for the 'or' mode for a given object.
    @param triggerer
        The object.
    @return
        Returns true if they do.
    */
    bool MultiTrigger::checkOr(BaseObject* triggerer)
    {
        std::set<MultiTrigger*>::iterator it;
        for(it = this->subTriggers_.begin(); it != this->subTriggers_.end(); ++it)
        {
            if ((*it)->isActive(triggerer))
                return true;
        }
        return false;
    }

    /**
    @brief
        Checks whether the sub-triggers amount to true for the 'xor' mode for a given object.
    @param triggerer
        The object.
    @return
        Returns true if they do.
    */
    bool MultiTrigger::checkXor(BaseObject* triggerer)
    {
        std::set<MultiTrigger*>::iterator it;
        bool test = false;
        for(it = this->subTriggers_.begin(); it != this->subTriggers_.end(); ++it)
        {
            if (test && (*it)->isActive(triggerer))
                return false;
            if ((*it)->isActive(triggerer))
                test = true;
        }
        return test;
    }

}
