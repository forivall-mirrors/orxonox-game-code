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
    @file MultiTrigger.h
    @brief Definition of the MultiTrigger class.
    @ingroup MultiTrigger
*/

#ifndef _MultiTrigger_H__
#define _MultiTrigger_H__

#include "objects/ObjectsPrereqs.h"

#include "core/BaseObject.h"
#include "core/ClassTreeMask.h"

#include <set>
#include <deque>

#include "TriggerBase.h"

namespace orxonox
{

    /**
    @brief
    Struct to handle @ref orxonox::MultiTrigger "MultiTrigger" states internally.

    @ingroup MultiTrigger
    */
    struct MultiTriggerState
    {
        BaseObject* originator;
        bool bTriggered;
    };

    /**
    @brief
        The MultiTrigger class implements a trigger that has a distinct state for each object triggering it.
        In more detail: A Trigger is an object that can either be <em>active</em> or <em>inactive</em>, with a specified behavior how to switch between the two. A MultiTrigger generalizes that behavior for multiple objects triggering the trigger. A MultiTrigger can be <em>active</em> or <em>inactive</em> for any object triggering it, with the state for each object being completely independent of the state for other objects. Each time a switch occurs an @ref orxonox::Event "Event" is fired with as the originator a @ref orxonox::MultiTriggerContainer "MultiTriggerContainer", containing a pointer to the MultiTrigger that caused the @ref orxonox::Event "Event" and a pointer to the object that caused the trigger to change it's activity.

        MultiTriggers also allow for additional complexity which can be added through the choice of the parameters explained (briefly) below:
        But first you must understand a small implementation detail. There is a distinction between the MultiTrigger being triggered (there is the state <em>triggered</em> for that) and the MultiTrigger being active (for that is the state <em>activity</em>). From the outside only the <em>activity</em> is visible. The state <em>triggered</em> tells us whether the trigger is actually triggered, but it could pretend (for some reason, some of which we will see shortly) to be <em>active</em>, while it in fact isn't. The standard behavior is, that the <em>activity</em> changes, when the MultiTrigger transits from being <em>triggered</em> to being <em>not triggered</em> or the other way around.
        The parameters are:
        - @b delay The delay is the time that the trigger waits until it reacts (i.e. changes it's state) to the triggering condition being fulfilled.
        - @b switch Switch is a boolean, if true the MultiTrigger is in <em>switch-mode</em>, meaning, that the <em>activity</em> changes only when the trigger is triggered, this means, that now the <em>activity</em> only changes, when the trigger changes from not being triggered to being triggered but not the other way around. The default is <code>false</code>.
        - @b stayactive Stay active is also a boolean, if true the MultiTrigger stays active after it has been activated as many times as specified by the parameter <em>activations</em>. The default is <code>false</code>.
        - @b activations The number of times the MultiTrigger can be activated until the trigger can't be triggered anymore. The default is <code>-1</code>, which denotes infinity.
        - @b invert Invert is a boolean, if true the MultiTrigger is in <em>invert-mode</em>, meaning, that if the triggering condition is fulfilled the MultiTrigger will have the state <em>not triggered</em> and and if the condition is not fulfilled it will have the state <em>triggered</em>. In short it just inverts the behavior of the MultiTrigger. The default is <code>false</code>.
        - @b simultaneousTriggerers The number of simultaneous triggerers limits the number of objects that are allowed to trigger the MultiTrigger at the same time. Or more precisely, the number of distinct objects the MultiTrigger has <em>triggered</em> states for, at each point in time. The default is <code>-1</code>, which denotes infinity.
        - @b mode The mode describes how the MultiTrigger acts in relation to all the triggers, that are appended to it. There are 3 modes: <em>and</em>, meaning that the MultiTrigger can only be triggered if all the appended triggers are active. <em>or</em>, meaning that the MultiTrigger can only triggered if at least one of the appended triggers is active. And <em>xor</em>, meaning that the MultiTrigger can only be triggered if one and only one appended trigger is active. Note, that I wrote <em>can only be active</em>, that implies, that there is an additional condition to the <em>activity</em> of the MultiTrigger and that is the fulfillment of the triggering condition (the MultiTrigger itself doesn't have one, but all derived classes should). Also bear in mind, that the <em>activity</em> of a MultiTrigger is still coupled to the object that triggered it. The default is <em>and</em>.
        - @b broadcast Broadcast is a boolean, if true the MutliTrigger is in <em>broadcast-mode</em>, meaning, that all trigger events that are caused by no originator (originator is NULL) are broadcast as having come from every possible originator, or more precisely as having come from all objects that are specified targets of this MultiTrigger. The default is false.
        - @b target The target describes the kind of objects that are allowed to trigger this MultiTrigger. The default is @ref orxonox::Pawn "Pawn".
        - Also there is the possibility of appending triggers (as long as they inherit from TriggerBase) to the MultiTrigger just by adding them as children in the XML description of your MultiTrigger.

        An example of a MultiTrigger created through XML would look like this:
        @code
        <MultiTrigger position="0,0,0" delay="1.3" switch="true" stayactive="true" activations="7" invert="true" simultaneousTriggerers="2" mode="xor" broadcast="false" target="Pawn">
            <TriggerBase />
            ...
            <TriggerBase />
        </MultiTrigger>
        @endcode

    @author
        Damian 'Mozork' Frick

        Many concepts and loads of inspiration from the @ref orxonox::Trigger "Trigger" class by Benjamin Knecht.

    @ingroup MultiTrigger
    */
    class _ObjectsExport MultiTrigger : public TriggerBase
    {
        public:
            MultiTrigger(Context* context); //!< Constructor. Registers the objects and initializes default values.
            virtual ~MultiTrigger(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a MultiTrigger object through XML.
            virtual void tick(float dt); //!< A method that is executed each tick.

            /**
            @brief Check whether the MultiTrigger is active.
            @return Returns if the MultiTrigger is active.
            */
            inline bool isActive(void) const
                { return this->isActive(NULL); }
            bool isActive(BaseObject* triggerer = NULL) const; //!< Check whether the MultiTrigger is active for a given object.

            /**
            @brief Set the number of objects that are allowed to simultaneously trigger this MultiTrigger.
            @param triggerers The number of objects. -1 denotes infinitely many.
            */
            inline void setSimultaneousTriggerers(int triggerers)
                { if(triggerers >= 0 || triggerers == TriggerBase::INF_s) this->maxNumSimultaneousTriggerers_ = triggerers; }
            /**
            @brief Get the number of objects that are allowed to simultaneously trigger this MultiTriggger.
            @return Returns the number of objects. -1 denotes infinity.
            */
            inline int getSimultaneousTriggerers(void)
                { return this->maxNumSimultaneousTriggerers_; }

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
                
            void addTarget(const std::string& targets); //!< Add some target to the MultiTrigger.

        protected:
            virtual std::queue<MultiTriggerState*>* letTrigger(void); //!< This method is called by the MultiTrigger to get information about new trigger events that need to be looked at.

            void changeTriggered(BaseObject* originator = NULL); //!< This method can be called by any class inheriting from MultiTrigger to change it's triggered status for a specified originator.

            bool isModeTriggered(BaseObject* triggerer = NULL); //!< Checks whether the MultiTrigger is triggered concerning it's children.
            bool isTriggered(BaseObject* triggerer = NULL); //!< Get whether the MultiTrigger is triggered for a given object.

            virtual void fire(bool status, BaseObject* originator = NULL);  //!< Helper method. Creates an Event for the given status and originator and fires it.
            void broadcast(bool status); //!< Helper method. Broadcasts an Event for every object that is a target.

            void removeTarget(const std::string& target); //!< Remove some target from the MultiTrigger.

            /**
            @brief Get the target mask used to identify the targets of this MultiTrigger.
            @return Returns the target mask.
            */
            inline ClassTreeMask& getTargetMask(void)
                { return this->targetMask_; }

        private:
            void childActivityChanged(BaseObject* originator); //!< This method is called by any child to advertise changes in it's state to it's parent.

            bool addState(MultiTriggerState* state); //!< Helper method. Adds a state to the state queue, where the state will wait to become active.

            bool checkAnd(BaseObject* triggerer); //!< Checks whether the children amount to true for the <em>and</em> mode for a given object.
            bool checkOr(BaseObject* triggerer); //!< Checks whether the children amount to true for the <em>or</em> mode for a given object.
            bool checkXor(BaseObject* triggerer); //!< Checks whether the children amount to true for the <em>xor</em> mode for a given object.

            /**
            @brief Get the objects for which this MultiTrigger is active.
            @return Returns a set with all the object this MultiTrigger is active for.
            */
            std::set<BaseObject*>& getActive(void)
                { return this->active_; }

            int maxNumSimultaneousTriggerers_; //!< The maximum number of objects simultaneously trigggering this MultiTrigger.

            bool bBroadcast_; //!< Bool for the broadcast-mode, if true all triggers go to all possible targets.

            std::set<BaseObject*> active_; //!< The set of all objects the MultiTrigger is active for.
            std::set<BaseObject*> triggered_; //!< The set of all objects the MultiTrigger is triggered for.

            std::deque< std::pair<float, MultiTriggerState*> > stateQueue_; //!< The queue of states waiting to become active.

            ClassTreeMask targetMask_; //!< The target mask, masking all objects that can trigger this MultiTrigger.

    };

}

#endif // _MultiTrigger_H__
