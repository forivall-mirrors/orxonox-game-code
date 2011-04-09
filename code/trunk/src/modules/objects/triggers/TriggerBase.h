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
    @file TriggerBase.h
    @brief Definition of the TriggerBase class.
    @ingroup Triggers
*/

#ifndef _TriggerBase_H__
#define _TriggerBase_H__

#include "objects/ObjectsPrereqs.h"

#include <set>
#include <string>

#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox {

    /**
    @brief
        The different modes the trigger can be in.

    @ingroup Triggers
    */
    namespace TriggerMode
    {
        enum Value
        {
            EventTriggerAND, //!< The <em>and</em> mode. The trigger can only trigger if all the children are active. 
            EventTriggerOR, //!< The <em>or</em> mode. The trigger can only trigger if at least one child is active.
            EventTriggerXOR, //!< The <em>xor</em> mode. The trigger can only trigger if exactly one child is active. 
        };
    }

    /**
    @brief
        The TriggerBase class is a base class for the two types of triggers, the (normal) @ref orxonox::Trigger "Triggers" and the @ref orxonox::MultiTrigger "MultiTriggers", it encompasses the shared data and functionality between these two types of triggers, but is in itself not a trigger that has any meaningful behavior and thus should not be instantiated.

    @author
        Damian 'Mozork' Frick

    @ingroup Triggers
    */
    class _ObjectsExport TriggerBase : public StaticEntity, public Tickable
    {
        public:
            TriggerBase(BaseObject* creator);
            virtual ~TriggerBase();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);  //!< Method for creating a TriggerBase object through XML.
            virtual void tick(float dt); //!< A method that is executed each tick.

            /**
            @brief Check whether the trigger is active. (i.e. triggered to the outside)
            @return Returns if the trigger is active.
            */
            virtual bool isActive(void) const
                { return false; }

            /**
            @brief Set the delay of the trigger.
            @param delay The delay to be set.
            */
            inline void setDelay(float delay)
                { if(delay > 0.0f) this->delay_= delay; this->delayChanged(); }
            /**
            @brief Get the delay of the trigger.
            @return The delay.
            */
            inline float getDelay(void) const
                { return this->delay_; }
            virtual void delayChanged(void) {}

            /**
            @brief Set switch-mode of the trigger.
            @param bSwitch If true the trigger is set to switched.
            */
            inline void setSwitch(bool bSwitch)
                { this->bSwitch_ = bSwitch; }
            /**
            @brief Get the switch-mode of the trigger.
            @return Returns true if the trigger is in switch-mode.
            */
            inline bool getSwitch(void) const
                { return this->bSwitch_; }

            /**
            @brief Set the stay-active-mode of the trigger.
            @param bStayActive If true the trigger is set to stay active.
            */
            inline void setStayActive(bool bStayActive)
                { this->bStayActive_ = bStayActive; }
            /**
            @brief Get the stay-active-mode of the trigger.
            @return Returns true if the trigger stays active.
            */
            inline bool getStayActive(void) const
                { return this->bStayActive_; }

            /**
            @brief Get the number of remaining activations of the trigger.
            @return The number of activations. -1 denotes infinity.
            */
            inline int getActivations(void) const
                { return this->remainingActivations_; }

            /**
            @brief Set the invert-mode of triggerhe trigger.
            @param bInvert If true the t is set to invert.
            */
            inline void setInvert(bool bInvert)
                { this->bInvertMode_ = bInvert; }
            /**
            @brief Get the invert-mode of the trigger.
            @return Returns true if the trigger is set to invert.
            */
            inline bool getInvert(void) const
                { return this->bInvertMode_; }

            void setMode(const std::string& modeName); //!< Set the mode of the trigger.
            /**
            @brief Set the mode of the trigger.
            @param mode The mode of the trigger.
            */
            inline void setMode(TriggerMode::Value mode) //!< Get the mode of the trigger.
                { this->mode_ = mode; }
            const std::string& getModeString(void) const;
            /**
            @brief Get the mode of the trigger.
            @return Returns and Enum for the mode of the trigger.
            */
            inline TriggerMode::Value getMode(void) const
                { return mode_; }

            void addTrigger(TriggerBase* trigger);
            const TriggerBase* getTrigger(unsigned int index) const;

            /**
            @brief Check whether this trigger is a @ref orxonox::MultiTrigger "MultiTrigger".
                This is done for performance reasons.
            */
            inline bool isMultiTrigger(void)
                { return this->bMultiTrigger_; }

        protected:
            static const int INF_s; //!< Magic number for infinity.
            //! Magic strings for the mode.
            static const std::string and_s;
            static const std::string or_s;
            static const std::string xor_s;

            /**
            @brief Set the number of activations the trigger can go through.
            @param activations The number of activations. -1 denotes infinitely many activations.
            */
            inline void setActivations(int activations)
                { if(activations >= 0 || activations == INF_s) this->remainingActivations_ = activations; }
                
            inline bool hasRemainingActivations(void)
                { return this->remainingActivations_ == INF_s || this->remainingActivations_  > 0; }

            /**
            @brief Adds the parent of a MultiTrigger.
            @param parent A pointer to the parent MultiTrigger.
            */
            inline void addParentTrigger(TriggerBase* parent)
                { this->parent_ = parent; }

            bool bFirstTick_; //!< Bool to check whether this is the first tick.

            float delay_; //!< The delay that is imposed on all new trigger events.
            bool bSwitch_; //!< Bool for the switch-mode, if true the trigger behaves like a switch.
            bool bStayActive_; //!< Bool for the stay-active-mode, if true the trigger stays active after its last activation.;

            int remainingActivations_; //!< The remaining activations of this trigger.

            bool bInvertMode_; //!< Bool for the invert-mode, if true the trigger is inverted.
            TriggerMode::Value mode_; //!< The mode of the trigger.

            TriggerBase* parent_; //!< The parent of this trigger.
            std::set<TriggerBase*> children_; //!< The children of this trigger.

            bool bMultiTrigger_; //!< Specifies whether this trigger is a MultiTrigger.

    };
}

#endif /* _Trigger_H__ */
