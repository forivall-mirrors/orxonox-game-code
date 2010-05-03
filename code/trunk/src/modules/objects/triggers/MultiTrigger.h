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
    
    namespace MultiTriggerMode
    {
        enum Value
        {
            EventTriggerAND,
            EventTriggerOR,
            EventTriggerXOR,
        };
    }
    
    struct MultiTriggerState
    {
        BaseObject* originator;
        bool bActive;
        bool bTriggered;
    };

    /**
    @brief
        The MultiTrigger class implements a trigger that has a distinct state for each object triggering it.
    */
    class _ObjectsExport MultiTrigger : public StaticEntity, public Tickable
    {
        public:
            MultiTrigger(BaseObject* creator);
            ~MultiTrigger();
            
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            
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
