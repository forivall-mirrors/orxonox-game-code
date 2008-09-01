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

#include <set>
#include <queue>

#include "OrxonoxPrereqs.h"

#include "WorldEntity.h"

#include "../tools/BillboardSet.h"

namespace orxonox {

  enum TriggerMode
  {
    TM_EventTriggerAND,
    TM_EventTriggerOR,
    TM_EventTriggerXOR,
  };

  class _OrxonoxExport Trigger : public WorldEntity
  {
    public:
      Trigger();
      ~Trigger();

      inline bool isTriggered() { return this->isTriggered(this->mode_); }
      virtual bool isTriggered(TriggerMode mode);
      void addTrigger(Trigger* trig);
      void setVisibility(bool bVisible);
      void setDelay(float delay);
      bool switchState();
      virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
      inline TriggerMode getMode() { return mode_; }
      inline void setMode(TriggerMode mode) { this->mode_ = mode; }
      void tick(float dt);

    private:
      void init();
      bool checkAnd();
      bool checkOr();
      bool checkXor();
      void setBillboardColour(ColourValue colour);

    private:
      std::set<Trigger*> children_;
      std::queue<std::pair<float,char> > stateChanges_;
      float remainingTime_;
      float timeSinceLastEvent_;
      TriggerMode mode_;
      bool bActive_;
      bool bInvertMode_;
      bool bTriggered_;
      bool bUpdating_;
      BillboardSet debugBillboard_;
      float delay_;
      char latestState_;
  };

}

#endif /* _Trigger_H__ */
