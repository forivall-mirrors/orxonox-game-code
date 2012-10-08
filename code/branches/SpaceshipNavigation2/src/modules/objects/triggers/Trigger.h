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
    @file Trigger.h
    @brief Definition of the Trigger class.
    @ingroup NormalTrigger
*/

#ifndef _Trigger_H__
#define _Trigger_H__

#include "objects/ObjectsPrereqs.h"

#include <set>
#include <queue>

#include "tools/BillboardSet.h"

#include "TriggerBase.h"

namespace orxonox
{

    /**
    @brief
         A Trigger is an object that can either be <em>active</em> or <em>inactive</em>, with a specified behavior how to switch between the two. Each time a switch occurs an @ref orxonox::Event "Event" is fired with as the originator the Trigger that caused the @ref orxonox::Event "Event".
         
        Triggers also allow for additional complexity which can be added through the choice of the parameters explained below:
        But first it is imperative to understand a small implementation detail. There is a distinction between the Trigger being triggered (there is the state <em>triggered</em> for that) and the Trigger being active (for that is the state <em>activity</em>). From the outside only the <em>activity</em> is visible. The state <em>triggered</em> tells us whether the trigger is actually triggered, but it could pretend (for some reason, some of which we will see shortly) to be <em>active</em>, while it in fact isn't. The standard behavior is, that the <em>activity</em> changes, when the Trigger transits from being <em>triggered</em> to being <em>not triggered</em> or the other way around.
        The parameters are:
        - @b delay The delay is the time that the trigger waits until it reacts (i.e. changes it's state) to the triggering condition being fulfilled.
        - @b switch Switch is a boolean, if true the Trigger is in <em>switch-mode</em>, meaning, that the <em>activity</em> changes only when the trigger is triggered, this means, that now the <em>activity</em> only changes, when the trigger changes from not being triggered to being triggered but not the other way around. The default is <code>false</code>.
        - @b stayactive Stay active is also a boolean, if true the Trigger stays active after it has been activated as many times as specified by the parameter <em>activations</em>. The default is <code>false</code>.
        - @b activations The number of times the Trigger can be activated until the trigger can't be triggered anymore. The default is <code>-1</code>, which denotes infinity.
        - @b invert Invert is a boolean, if true the Trigger is in <em>invert-mode</em>, meaning, that if the triggering condition is fulfilled the Trigger will have the state <em>not triggered</em> and and if the condition is not fulfilled it will have the state <em>triggered</em>. In short it just inverts the behavior of the Trigger. The default is <code>false</code>.
        - @b mode The mode describes how the Trigger acts in relation to all the triggers, that are appended to it. There are 3 modes: <em>and</em>, meaning that the Trigger can only be triggered if all the appended triggers are active. <em>or</em>, meaning that the Trigger can only triggered if at least one of the appended triggers is active. And <em>xor</em>, meaning that the Trigger can only be triggered if one and only one appended trigger is active. Note, that I wrote <em>can only be active</em>, that implies, that there is an additional condition to the <em>activity</em> of the Trigger and that is the fulfillment of the triggering condition (the Trigger itself doesn't have one, but all derived classes should). Also bear in mind, that the <em>activity</em> of a Trigger is still coupled to the object that triggered it. The default is <em>and</em>.
        - Also there is the possibility of appending triggers (as long as they inherit from TriggerBase) to the Trigger just by adding them as children in the XML description of your Trigger.

        An example of a Trigger created through XML would look like this:
        @code
        <Trigger position="0,0,0" delay="1.3" switch="true" stayactive="true" activations="7" invert="true" mode="xor" broadcast="false" target="Pawn">
            <TriggerBase />
            ...
            <TriggerBase />
        </Trigger>
        @endcode

    @author
        Benjamin Knecht

    @ingroup NormalTrigger
    */
    class _ObjectsExport Trigger : public TriggerBase
    {
        public:
            Trigger(BaseObject* creator); // Constructor. Registers and initializes the object.
            virtual ~Trigger();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); // Method for creating a Trigger object through XML.
            virtual void tick(float dt);

            /**
            @brief Check whether the Trigger is active.
            @return Returns if the Trigger is active.
            */
            inline bool isActive(void) const
                { return this->bActive_; }

            void delayChanged(void); // React to a change in delay.

            static void debugFlares(bool bVisible); // Set the visibility of all debug billboards of all Triggers.
            virtual void changedVisibility(); // React to a change of visibility of the trigger by adjusting the visibility of the debug billboards.

        protected:
            /**
            @brief Check whether the Trigger is triggered according to its mode.
            @return Returns true if the Trigger is triggered, false if not.
            */
            inline bool isTriggered()
                { return this->isTriggered(this->mode_); }
            virtual bool isTriggered(TriggerMode::Value mode); // Check whether the Trigger should be triggered, given only its sub-triggers, given a specific mode.
            virtual void triggered(bool bIsTriggered); // Fires an event with the input triggered state.

        private:
            bool switchState(); // Switch (toggle) the activity (to the outside the triggered state) of the trigger.
            void storeState(); // Stores the state in the queue.
            
            bool checkAnd(); // Check whether all the sub-triggers of this Trigger are active.
            bool checkOr(); // Check whether at least one of the sub-triggers of this Trigger is active.
            bool checkXor(); // Check whether exactly one of the sub-triggers of this Trigger is active.
            
            void setBillboardColour(const ColourValue& colour); // Set the colour of the debug billboard.

            bool bActive_; //!< Whether the trigger is active (to the outside triggered).
            bool bTriggered_; //!< Whether the trigger is triggered (to the inside).

            char latestState_; //!< Temporarily stores a state consisting of whether the trigger is triggeres at the first bit (least significant bit) and its activity at the second bit.
            float remainingTime_; //!< The time until the next state (in the queue) takes effect.
            float timeSinceLastEvent_; //!< The time since the last event came in.

            BillboardSet debugBillboard_; //!< A set of debug billboards to visualize the state of the trigger.

            std::queue<std::pair<float, char> > stateChanges_; //!< A queue of state changes (in the same format as latestState_) paired with the time they will take effect since the last state change took effect.
    };

}

#endif /* _Trigger_H__ */
