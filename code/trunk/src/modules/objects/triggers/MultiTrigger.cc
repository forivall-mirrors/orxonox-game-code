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

#include "MultiTrigger.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "MultiTriggerContainer.h"

namespace orxonox
{
    
    /*static*/ const int MultiTrigger::INF_s = -1;
    /*static*/ const std::string MultiTrigger::or_s = "or";
    /*static*/ const std::string MultiTrigger::and_s = "and";
    /*static*/ const std::string MultiTrigger::xor_s = "xor";
    
    CreateFactory(MultiTrigger);
    
    //TODO: Clean up.
    MultiTrigger::MultiTrigger(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(MultiTrigger);
        
        this->mode_ = MultiTriggerMode::EventTriggerAND;

        this->bFirstTick_ = true;

        this->bInvertMode_ = false;
        this->bSwitch_ = false;
        this->bStayActive_ = false;
        this->delay_ = 0.0f;
        this->remainingActivations_ = INF_s;
        this->maxNumSimultaniousTriggerers_ = INF_s;
        
        this->targetMask_.exclude(Class(BaseObject));

        this->setSyncMode(0x0);
        
    }
    
    //TODO: Document
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
    
    //TODO: Document.
    void MultiTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MultiTrigger, XMLPort, xmlelement, mode);

        XMLPortParam(MultiTrigger, "delay", setDelay, getDelay, xmlelement, mode).defaultValues(0.0f);
        XMLPortParam(MultiTrigger, "switch", setSwitch, getSwitch, xmlelement, mode).defaultValues(false);
        XMLPortParam(MultiTrigger, "stayactive", setStayActive, getStayActive, xmlelement, mode).defaultValues(false);
        XMLPortParam(MultiTrigger, "activations", setActivations, getActivations, xmlelement, mode).defaultValues(INF_s);
        XMLPortParam(MultiTrigger, "invert", setInvert, getInvert, xmlelement, mode).defaultValues(false);
        XMLPortParam(MultiTrigger, "simultaniousTriggerers", setSimultaniousTriggerers, getSimultaniousTriggerers, xmlelement, mode).defaultValues(INF_s);
        XMLPortParamTemplate(MultiTrigger, "mode", setMode, getModeString, xmlelement, mode, const std::string&).defaultValues(MultiTrigger::and_s);
        XMLPortParamLoadOnly(MultiTrigger, "target", addTargets, xmlelement, mode).defaultValues("ControllableEntity"); //TODO: Remove load only

        XMLPortObject(MultiTrigger, MultiTrigger, "", addTrigger, getTrigger, xmlelement, mode);
        
        COUT(4) << "MultiTrigger '" << this->getName() << "' (&" << this << ") created." << std::endl;
    }
    
    //TODO: Document
    void MultiTrigger::tick(float dt)
    {
        if(this->bFirstTick_) // If this is the first tick.
        {
            this->bFirstTick_ = false;
            this->fire(false); //TODO: Does this work? Resp. Is it correct?
        }

        // Check if the object is active (this is NOT Trigger::isActive()!)
        if (!this->BaseObject::isActive())
            return;

        SUPER(MultiTrigger, tick, dt);
        
        std::queue<MultiTriggerState*>* queue  = this->letTrigger();
        
        if(queue != NULL)
        {
            while(queue->size() > 0)
            {
                //TODO: Be more efficient, Don't delete a state and create a new one immediately after that. Reuse!
                MultiTriggerState* state = queue->front();
                if(state == NULL)
                    break;

                bool bTriggered = (state->bTriggered & this->isModeTriggered(state->originator)) ^ this->bInvertMode_;
                if(bTriggered ^ this->isTriggered(state->originator))
                    this->addState(bTriggered, state->originator);
                queue->pop();
                delete state;
            }
            delete queue;
        }        

        if (this->stateQueue_.size() > 0)
        {
            MultiTriggerState* state;
            float timeRemaining;
            for(int size = this->stateQueue_.size(); size >= 1; size--)
            {
                timeRemaining = this->stateQueue_.front().first;
                state = this->stateQueue_.front().second;
                if(timeRemaining <= dt)
                {
                    if(this->maxNumSimultaniousTriggerers_ == INF_s || this->triggered_.size() < (unsigned int)this->maxNumSimultaniousTriggerers_)
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
                        
                        // Add the originator to the objects activating this MultiTrigger.
                        if(state->bActive == true)
                        {
                            this->active_.insert(state->originator);
                        }
                        // Remove the originator from the objects activating this MultiTrigger.
                        else
                        {
                            this->active_.erase(state->originator);
                        }
                        
                        // Fire the event.
                        this->fire(state->bActive, state->originator);
                    }
                    
                    // Remove the state from the state queue.
                    this->stateQueue_.pop_front();
                    COUT(4) << "MultiTrigger '" << this->getName() << "' &" << this << ": State processed, removing from state queue. originator: " << state->originator->getIdentifier()->getName() << " (&" << state->originator << "), active: " << state->bActive << "|" << this->isActive(state->originator) << ", triggered: " << state->bTriggered << "|" << this->isTriggered(state->originator) << "." << std::endl;
                    delete state;
                    size -= 1;
                }
                else
                {
                    this->stateQueue_.push_back(std::pair<float, MultiTriggerState*>(timeRemaining-dt, state));
                    this->stateQueue_.pop_front();
                    COUT(4) << "MultiTrigger '" << this->getName() << "' &" << this << ": State processed, decreasing time remaining. originator: " << state->originator->getIdentifier()->getName() << " (&" << state->originator << "), active: " << state->bActive << ", triggered: " << state->bTriggered << ", time remaining: " << timeRemaining-dt << "." << std::endl;
                }
            }
        }
    }

    //TODO: Document
    bool MultiTrigger::isModeTriggered(BaseObject* triggerer)
    {
        if (this->children_.size() != 0)
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
    
    //TODO: Document
    std::queue<MultiTriggerState*>* MultiTrigger::letTrigger(void)
    {
        // Goes through all trigger children and gets the objects triggering them.
        std::set<BaseObject*>* triggerers = new std::set<BaseObject*>();
        std::set<BaseObject*>::iterator objIt;
        for(std::set<MultiTrigger*>::iterator it = this->children_.begin(); it != this->children_.end(); it ++)
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

        if(triggerers->size() == 0)
            return NULL;
        
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
    
    //TODO: Document
    bool MultiTrigger::isTriggered(BaseObject* triggerer)
    {
        std::set<BaseObject*>::iterator it = this->triggered_.find(triggerer);
        if(it == this->triggered_.end())
            return false;
        return true;
    }

    //TODO: Document
    bool MultiTrigger::checkAnd(BaseObject* triggerer)
    {
        std::set<MultiTrigger*>::iterator it;
        for(it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            if (!(*it)->isActive(triggerer))
                return false;
        }
        return true;
    }

    //TODO: Document
    bool MultiTrigger::checkOr(BaseObject* triggerer)
    {
        std::set<MultiTrigger*>::iterator it;
        for(it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            if ((*it)->isActive(triggerer))
                return true;
        }
        return false;
    }

    //TODO: Document
    bool MultiTrigger::checkXor(BaseObject* triggerer)
    {
        std::set<MultiTrigger*>::iterator it;
        bool test = false;
        for(it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            if (test && (*it)->isActive(triggerer))
                return false;
            if ((*it)->isActive(triggerer))
                test = true;
        }
        return test;
    }
    
    //TODO: Document
    bool MultiTrigger::addState(bool bTriggered, BaseObject* originator)
    {
        if(!this->isTarget(originator))
            return false;
        
        // If the state doesn't change.
        if(this->isTriggered() && bTriggered)
            return false;
        
        bool bActive = !this->isActive(originator);
        
        // If the MultiTrigger is in switch mode.
        if(this->bSwitch_ && !bTriggered)
        {
            bActive = this->isActive(originator);
        }
        // If the state changes to active.
        else if(this->remainingActivations_ != INF_s && bActive)
        {
            if(this->remainingActivations_ == 0)
                return false;
            this->remainingActivations_--;
        }
        else
        {
            // If the MultiTrigger should stay active if there are no more remaining activations.
            //TODO: Find out how this applies to infinitely many activations.
            if(this->bStayActive_ && this->remainingActivations_ == 0)
                return false;
        }
        
        COUT(4) << "MultiTrigger &" << this << ": State added to state queue. originator: " << originator->getIdentifier()->getName() << " (&" << originator << "), active: " << bActive << "|" << this->isActive(originator) << ", triggered: " << bTriggered << "|" << this->isTriggered(originator) << ", remaining activations: " << this->remainingActivations_ << "." << std::endl;
        
        // Create state.
        MultiTriggerState* state = new MultiTriggerState;
        state->bActive = bActive;
        state->bTriggered = bTriggered;
        state->originator = originator;
        this->stateQueue_.push_back(std::pair<float, MultiTriggerState*>(this->delay_, state));
        
        return true;
    }

    //TODO: Document
    void MultiTrigger::setMode(const std::string& modeName)
    {
        if (modeName == MultiTrigger::and_s)
            this->setMode(MultiTriggerMode::EventTriggerAND);
        else if (modeName == MultiTrigger::or_s)
            this->setMode(MultiTriggerMode::EventTriggerOR);
        else if (modeName == MultiTrigger::xor_s)
            this->setMode(MultiTriggerMode::EventTriggerXOR);
    }

    //TODO: Document
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
    
    //TODO: Document
    bool MultiTrigger::isActive(BaseObject* triggerer)
    {
        std::set<BaseObject*>::iterator it = this->active_.find(triggerer);
        if(it == this->active_.end())
            return false;
        return true;
    }

    //TODO: Document
    void MultiTrigger::addTrigger(MultiTrigger* trigger)
    {
        if (this != trigger && trigger != NULL)
            this->children_.insert(trigger);
    }

    //TODO: Document
    const MultiTrigger* MultiTrigger::getTrigger(unsigned int index) const
    {
        if (this->children_.size() <= index)
            return NULL;

        std::set<MultiTrigger*>::const_iterator it;
        it = this->children_.begin();

        for (unsigned int i = 0; i != index; ++i)
            ++it;

        return (*it);
    }
    
    //TODO: Document
    void MultiTrigger::fire(bool status, BaseObject* originator)
    {
        if(originator == NULL)
        {
            this->fireEvent(status);
            return;
        }
        MultiTriggerContainer* container = new MultiTriggerContainer(this, this, originator);
        this->fireEvent(status, container);
        COUT(4) << "MultiTrigger &" << this << ": Fired event. originator: " << originator->getIdentifier()->getName() << " (&" << originator << "), status: " << status << "." << std::endl;
        delete container;
    }
    
    //TODO: Document
    void MultiTrigger::addTargets(const std::string& targets)
    {
        Identifier* target = ClassByString(targets);

        if (target == NULL)
        {
            COUT(1) << "Error: \"" << targets << "\" is not a valid class name to include in ClassTreeMask (in " << this->getName() << ", class " << this->getIdentifier()->getName() << ')' << std::endl;
            return;
        }

        this->targetMask_.include(target);

        // trigger shouldn't react on itself or other triggers
        this->targetMask_.exclude(Class(MultiTrigger), true);

        // we only want WorldEntities
        ClassTreeMask WEMask;
        WEMask.include(Class(WorldEntity));
        this->targetMask_ *= WEMask;

        this->notifyMaskUpdate();
    }

    //TODO: Document
    void MultiTrigger::removeTargets(const std::string& targets)
    {
        Identifier* target = ClassByString(targets);
        this->targetMask_.exclude(target);
    }
    
}
