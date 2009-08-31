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

#ifndef _Trigger_H__
#define _Trigger_H__

#include "objects/ObjectsPrereqs.h"

#include <set>
#include <queue>

#include "tools/BillboardSet.h"
#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
  namespace TriggerMode
  {
    enum Value
    {
      EventTriggerAND,
      EventTriggerOR,
      EventTriggerXOR,
    };
  }

  class _ObjectsExport Trigger : public StaticEntity, public Tickable
  {
    public:
      Trigger(BaseObject* creator);
      virtual ~Trigger();

      virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
      virtual void tick(float dt);

      inline bool isActive() const
        { return bActive_; }

      void addTrigger(Trigger* trigger);
      const Trigger* getTrigger(unsigned int index) const;

      void setMode(const std::string& modeName);
      inline void setMode(TriggerMode::Value mode)
        { this->mode_ = mode; }
      inline TriggerMode::Value getMode() const
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

      inline void setVisible(bool visibility)
        { this->debugBillboard_.setVisible(visibility); }

      void setDelay(float delay);
      inline float getDelay() const
        { return this->delay_; }

      bool switchState();

      static void debugFlares(bool bVisible);
      virtual void changedVisibility();

    protected:
      inline bool isTriggered() { return this->isTriggered(this->mode_); }
      virtual bool isTriggered(TriggerMode::Value mode);
      virtual void triggered(bool bIsTriggered);

    private:
      bool checkAnd();
      bool checkOr();
      bool checkXor();
      void setBillboardColour(const ColourValue& colour);
      void storeState();
      std::string getModeString() const;

      bool bActive_;
      bool bTriggered_;
      bool bFirstTick_;

      TriggerMode::Value mode_;
      bool bInvertMode_;
      bool bSwitch_;
      bool bStayActive_;
      float delay_;
      int remainingActivations_;

      char latestState_;
      float remainingTime_;
      float timeSinceLastEvent_;

//      bool bUpdating_;
      BillboardSet debugBillboard_;

      std::set<Trigger*> children_;
      std::queue<std::pair<float, char> > stateChanges_;
  };

}

#endif /* _Trigger_H__ */
