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
    @file MultiTrigger.h
    @brief Definition of the MultiTrigger class.
*/

#ifndef _MultiTrigger_H__
#define _MultiTrigger_H__

#include "objects/ObjectsPrereqs.h"

#include "core/BaseObject.h"
#include "core/ClassTreeMask.h"

#include <set>
#include <deque>

#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{

    //! The different modes the MultiTrigger can be in.
    namespace MultiTriggerMode
    {
        enum Value
        {
            EventTriggerAND,
            EventTriggerOR,
            EventTriggerXOR,
        };
    }

    //! Struct to handle MultiTrigger states internally.
    struct MultiTriggerState
    {
        BaseObject* originator;
        bool bTriggered;
    };

    /**
    @brief
        The MultiTrigger class implements a trigger that has a distinct state for each object triggering it.
        In more detail: A Trigger is an object that can either be active or inactive, whith a specified behaviour how to switch between the two. A MultiTrigger generalizes that behaviour for multiple objects trigggering the trigger. A MultiTrigger can be active or inactive for any object triggering it, with the state for each object being completely independent of the state for other objects. Each time a switch occurs an Event is fired with as the originator a MultiTriggerContainer, containig a pointer to the pointer that caused the Event and a pointer to the object that caused the trigger to change it's activity.

        MultiTriggers also allow for additional complexity which can be added trough the choice of the parameters explained (briefly) below:
        But first you must understand a small implementational detail. There is a distinction between the MultiTrigger being triggered (there is the state 'triggered' for that) and the MultiTrigger being active (for that is the state 'activity'). From the outside only the activity is visible. The state 'triggered' tells us whether the trigger is actually triggered, but it could pretend (for some reason, some of which we will see shortly) to be triggered (or to the outside, active), while it in fact isn't. The standard behaviour is, that the cativity changes, when the MultiTrigger transits from being triggered to not being triggered or the other way around.
        The parameters are:
            'delay':                    The delay is the time that the trigger waits until it reacts (i.e. changes it's state) to the triggering condition being fulfilled.
            'switch':                   Switch is a bool, if true the MultiTrigger is in switch-mode, meaning, that the activity changes only when the trigger is triggered , this means, that now the activity only changes, when the trigger changes from not being triggered to being triggered but not the other way around. The default is false.
            'stayactive':               Stay active is also a bool, if true the MultiTrigger stays active after it has been activated as many times as specified by the parameter activations. The default is -1, which is infinity.
            'activations':              The number of activations until the trigger can't be triggered anymore. The default is -1, which is infinity.
            'invert':                   Invert is a bool, if true the trigger is in invert-mode, meaning, that if the triggering condition is fulfilled the MultiTrigger will have the state not triggered and and if the condition is not fulfilled it will have the state triggered. In short it just inverts the behaviour of the MultiTrigger. The default is false.
            'simultaniousTriggerers':   The number of simultanious triggerers limits the number of object that are allowed to trigger the MultiTrigger at the same time. Or a little more precisely, the number of distinct objects the MultiTrigger has 'triggered' states for, at each point in time.
            'mode':                     The mode describes how the MultiTrigger acts in relation to all the MultiTriggers, that are appended to it. There are 3 modes: 'and', meaning that the MultiTrigger can only be triggered if all the appended MultiTriggers are active. 'or', meaning that the MultiTrigger can only triggered if at least one of the appendend MultiTriggers is active. And 'xor', meaning that the MultiTrigger can only be triggered if one and only one appended MultiTrigger is active. Notice, that I wrote 'can only be active', that implies, that there is an addtitional condition to the activity of the MultiTrigger and that is the fulfillment of the triggering condition (the MultiTrigger itself doesn't have one, but all derived classes should). Also bear in mind, that the activity of a MultiTrigger is still coupled to the object that triggered it. The default is 'and'.
            'broadcast'                 Broadcast is a bool, if true the MutliTrigger is in broadcast-mode, meaining, that all trigger events that are caused by no originator (originator is NULL) are broadcasted as having come from every possible originator, or more precisely as having come from all objects that are specified targets of this MultiTrigger.
            'target':                   The target describes the kind of objects that are allowed to trigger this MultiTrigger. The default is 'Pawn'.
            Also there is the possibility of appending MultiTriggers to the MultiTrigger just by adding them as subobjects in the XML description of your MultiTrigger.

    @author
        Damian 'Mozork' Frick
    */
    class _ObjectsExport MultiTrigger : public StaticEntity, public Tickable
    {
        public:
            MultiTrigger(BaseObject* creator); //!< Constructor. Registers the objects and initializes default values.
            ~MultiTrigger(); //!< Destructor.
            
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a MultiTrigger object through XML.
            virtual void tick(float dt); //!< A method that is executed each tick.
            
            bool isActive(BaseObject* triggerer = NULL); //!< Get whether the MultiTrigger is active for a given object.

            /**
            @brief Set the delay of the MultiTrigger.
            @param delay The delay to be set.
            */
            inline void setDelay(float delay)
                { if(delay > 0) this->delay_= delay; }
            /**
            @brief Get the delay of the MultiTrigger.
            @return The delay.
            */
            inline float getDelay(void) const
                { return this->delay_; }

            /**
            @brief Set switch-mode of the MultiTrigger.
            @param bSwitch If true the MultiTrigger is set to switched.
            */
            inline void setSwitch(bool bSwitch)
                { this->bSwitch_ = bSwitch; }
            /**
            @brief Get the switch-mode of the MultiTrigger.
            @return Returns true if the MultiTriger is in switch-mode.
            */
            inline bool getSwitch(void) const
                { return this->bSwitch_; }

            /**
            @brief Set the stay-active-mode of the MultiTrigger.
            @param bStayActive If true the MultiTrigger is set to stay active.
            */
            inline void setStayActive(bool bStayActive)
                { this->bStayActive_ = bStayActive; }
            /**
            @brief Get the stay-active-mode of the MultiTrigger.
            @return Returns true if the MultiTrigger stays active.
            */
            inline bool getStayActive(void) const
                { return this->bStayActive_; }

            /**
            @brief Set the number of activations the MultiTrigger can go through.
            @param activations The number of activations. -1 denotes infinitely many activations.
            */
            inline void setActivations(int activations)
                { if(activations >= 0 || activations == INF_s) this->remainingActivations_ = activations; }
            /**
            @brief Get the number of remaining activations of the MultiTrigger.
            @return The number of activations. -1 denotes infinity.
            */
            inline int getActivations(void) const
                { return this->remainingActivations_; }

            /**
            @brief Set the number of objects that are allowed to simultaniously trigger this MultiTrigger.
            @param triggerers The number of objects. -1 denotes infinitely many.
            */
            inline void setSimultaniousTriggerers(int triggerers)
                { if(triggerers >= 0 || triggerers == INF_s) this->maxNumSimultaniousTriggerers_ = triggerers; }
            /**
            @brief Get the number of objects that are allowed to simultaniously trigger this MultiTriggger.
            @return Returns the number of objects. -1 denotes infinity.
            */
            inline int getSimultaniousTriggerers(void)
                { return this->maxNumSimultaniousTriggerers_; }

            /**
            @brief Set the invert-mode of the MultiTrigger.
            @param bInvert If true the MultiTrigger is set to invert.
            */
            inline void setInvert(bool bInvert)
                { this->bInvertMode_ = bInvert; }
            /**
            @brief Get the invert-mode of the MultiTrigger.
            @return Returns true if the MultiTrigger is set to invert.
            */
            inline bool getInvert(void) const
                { return this->bInvertMode_; }

            void setMode(const std::string& modeName); //!< Set the mode of the MultiTrigger.
            /**
            @brief Set the mode of the MultiTrigger.
            @param mode The mode of the MultiTrigger.
            */
            inline void setMode(MultiTriggerMode::Value mode) //!< Get the mode of the MultiTrigger.
                { this->mode_ = mode; }
            const std::string& getModeString(void) const;
            /**
            @brief Get the mode of the MultiTrigger.
            @return Returns and Enum for the mode of the MultiTrigger.
            */
            inline MultiTriggerMode::Value getMode() const
                { return mode_; }

            /**
            @brief Set the broadcast-mode of the MultiTrigger.
            @param bBroadcast If true the MultiTrigger is set to broadcast;
            */
            inline void setBroadcast(bool bBroadcast)
                { this->bBroadcast_ = bBroadcast; }
            /**
            @brief Get the broadcast-mode of the MultiTrigger.
            @return Returns true if the MultiTrigger is set to broadcast.
            */
            inline bool getBroadcast(void)
                { return this->bBroadcast_; }

            /**
            @brief Get whether the input object is a target of the MultiTrigger.
            @param target A pointer to the object.
            @return Returns true if the input object is a target, false if not.
            */
            inline bool isTarget(BaseObject* target)
                { if(target == NULL) return true; else return targetMask_.isIncluded(target->getIdentifier()); }
            void addTargets(const std::string& targets); //!< Add some target to the MultiTrigger.
            void removeTargets(const std::string& targets); //!< Remove some target from the MultiTrigger.
            
            void addTrigger(MultiTrigger* trigger); //!< Adds a MultiTrigger as a sub-trigger to the trigger.
            const MultiTrigger* getTrigger(unsigned int index) const; //!< Get the sub-trigger of this MultiTrigger at the given index.
            
        protected:
            virtual std::queue<MultiTriggerState*>* letTrigger(void); //!< This method is called by the MultiTrigger to get information about new trigger events that need to be looked at.

            void changeTriggered(BaseObject* originator = NULL); //!< This method can be called by any class inheriting from MultiTrigger to change it's triggered status for a specified originator.
            
            bool isModeTriggered(BaseObject* triggerer = NULL); //!< Checks whetherx the MultiTrigger is triggered concerning it's sub-triggers.
            bool isTriggered(BaseObject* triggerer = NULL); //!< Get whether the MultiTrigger is triggered for a given object.

            void fire(bool status, BaseObject* originator = NULL);  //!< Helper method. Creates an Event for the given status and originator and fires it.
            void broadcast(bool status); //!< Helper method. Broadcasts an Event for every object that is a target.

            /**
            @brief Adds the parent of a MultiTrigger.
            @param parent A pointer to the parent MultiTrigger.
            */
            inline void addParentTrigger(MultiTrigger* parent)
                { this->parentTrigger_ = parent; }
            
            /**
            @brief Get the target mask used to identify the targets of this MultiTrigger.
            @return Returns the target mask.
            */
            inline ClassTreeMask& getTargetMask(void)
                { return this->targetMask_; }
            /**
            @brief Is called, when the target mask changes.
            */
            //TODO: Check if something mus be done here.
            virtual void notifyMaskUpdate(void) {}
            
        private:
            static const int INF_s; //!< Magic number for infinity.
            //! Magic strings for the mode.
            static const std::string and_s;
            static const std::string or_s;
            static const std::string xor_s;

            void subTrigggerActivityChanged(BaseObject* originator); //!< This method is called by any sub-trigger to advertise changes in it's state to it's parent-trigger.
            
            bool addState(MultiTriggerState* state); //!< Helper method. Adds a state to the state queue, where the state will wait to become active.
            
            bool checkAnd(BaseObject* triggerer); //!< Checks whether the sub-triggers amount to true for the 'and' mode for a given object.
            bool checkOr(BaseObject* triggerer); //!< Checks whether the sub-triggers amount to true for the 'or' mode for a given object.
            bool checkXor(BaseObject* triggerer); //!< Checks whether the sub-triggers amount to true for the 'xor' mode for a given object.

            /**
            @brief Get the objects for which this MultiTrigger is active.
            @return Returns a set with all the object this MultiTrigger is active for.
            */
            std::set<BaseObject*>& getActive(void)
                { return this->active_; }

            bool bFirstTick_; //!< Bool to distinguish the first tick form all the following.

            float delay_; //!< The delay that is imposed on all new trigger events.
            bool bSwitch_; //!< Bool for the switch-mode, if true the MultiTrigger behaves like a switch.
            bool bStayActive_; //!< Bool for the stay-active-mode, if true the MultiTrigger stays active after its last activation.;
            
            int remainingActivations_; //!< The remaining activations of this MultiTrigger.
            int maxNumSimultaniousTriggerers_; //!< The maximum number of objects simultaniously trigggering this MultiTrigger.

            bool bInvertMode_; //!< Bool for the invert-mode, if true the MultiTrigger is inverted.
            MultiTriggerMode::Value mode_; //!< The mode of the MultiTrigger.

            bool bBroadcast_; //!< Bool for the broadcast-mode, if true all triggers go to all possible targets.

            MultiTrigger* parentTrigger_;
            std::set<MultiTrigger*> subTriggers_; //!< The sub-triggers of this MultiTrigger.
            
            std::set<BaseObject*> active_; //!< The set of all objects the MultiTrigger is active for.
            std::set<BaseObject*> triggered_; //!< The set of all objects the MultiTrigger is triggered for.

            std::deque< std::pair<float, MultiTriggerState*> > stateQueue_; //!< The queue of states waiting to become active.
            
            ClassTreeMask targetMask_; //!< The target mask, masking all objects that can trigger this MultiTrigger.
            
    };

}

#endif // _MultiTrigger_H__
