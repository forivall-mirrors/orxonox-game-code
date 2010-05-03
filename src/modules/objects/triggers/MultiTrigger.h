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
        bool bActive;
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
            'target':                   The target describes the kind of objects that are allowed to trigger this MultiTrigger. The default is 'ControllableEntity'.
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
            
            bool isActive(BaseObject* triggerer = NULL);
            void addTrigger(MultiTrigger* trigger);
            const MultiTrigger* getTrigger(unsigned int index) const;
            
            void addTargets(const std::string& targets);
            void removeTargets(const std::string& targets);
            inline bool isTarget(BaseObject* target)
                { return targetMask_.isIncluded(target->getIdentifier()); }
            
            void setMode(const std::string& modeName);
            const std::string& getModeString(void) const;
            inline void setMode(MultiTriggerMode::Value mode)
                { this->mode_ = mode; }
            inline MultiTriggerMode::Value getMode() const
                { return mode_; }

            inline void setInvert(bool bInvert)
                { this->bInvertMode_ = bInvert; }
            inline bool getInvert() const
                { return this->bInvertMode_; }

            inline void setSwitch(bool bSwitch)
                { this->bSwitch_ = bSwitch; }
            inline bool getSwitch() const
                { return this->bSwitch_; }

            inline void setStayActive(bool bStayActive)
                { this->bStayActive_ = bStayActive; }
            inline bool getStayActive() const
                { return this->bStayActive_; }

            inline void setActivations(int activations)
                { this->remainingActivations_ = activations; }
            inline int getActivations() const
                { return this->remainingActivations_; }
                
            inline void setSimultaniousTriggerers(int triggerers)
                { this->maxNumSimultaniousTriggerers_ = triggerers; }
            inline int getSimultaniousTriggerers(void)
                { return this->maxNumSimultaniousTriggerers_; }

            inline void setDelay(float delay)
                { this->delay_= delay; }
            inline float getDelay() const
                { return this->delay_; }
            
        protected:
            void fire(bool status, BaseObject* originator = NULL);
            
            bool isModeTriggered(BaseObject* triggerer = NULL);
            bool isTriggered(BaseObject* triggerer = NULL);
            
            virtual std::queue<MultiTriggerState*>* letTrigger(void);
            
            inline ClassTreeMask& getTargetMask(void)
                { return this->targetMask_; }
            virtual void notifyMaskUpdate(void) {}
            
        private:
            static const int INF_s;
            static const std::string or_s;
            static const std::string and_s;
            static const std::string xor_s;
            
            bool addState(bool bTriggered, BaseObject* originator = NULL);
            
            bool checkAnd(BaseObject* triggerer);
            bool checkOr(BaseObject* triggerer);
            bool checkXor(BaseObject* triggerer);

            std::set<BaseObject*>& getActive(void)
                { return this->active_; }

            bool bFirstTick_;

            MultiTriggerMode::Value mode_;
            bool bInvertMode_;
            bool bSwitch_;
            bool bStayActive_;
            float delay_;
            int remainingActivations_;
            int maxNumSimultaniousTriggerers_;
            
            std::set<BaseObject*> active_;
            std::set<BaseObject*> triggered_;

            std::set<MultiTrigger*> children_;
            std::deque<std::pair<float, MultiTriggerState*> > stateQueue_;
            
            ClassTreeMask targetMask_;
            
    };

}

#endif // _MultiTrigger_H__
