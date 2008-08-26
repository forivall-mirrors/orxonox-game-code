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

#include "OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "core/BaseObject.h"
#include "core/ClassTreeMask.h"
#include "../tools/BillboardSet.h"

namespace orxonox {

  enum TriggerMode
  {
    TM_EventTriggerAND,
    TM_EventTriggerOR,
    TM_EventTriggerXOR,
    TM_EventTriggerNOT
  };

  class _OrxonoxExport Trigger : public WorldEntity
  {
    public:
      Trigger();
      ~Trigger();

      bool isTriggered();
      bool isTriggered(TriggerMode mode);
      void addTrigger(Trigger* trig);
      void addTargets(std::string targets);
      void removeTargets(std::string targets);
      void setVisibility(int bVisible);
      virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
      inline TriggerMode getMode() { return mode_; }
      inline void setMode(TriggerMode mode) { this->mode_ = mode; }
      void tick(float dt);
      inline void reset(float time) { this->actualTime_ = 0; this->triggingTime_ = time; }
      inline void reset() { reset(triggingTime_); }

    private:
      bool checkAnd();
      bool checkOr();
      bool checkXor();
      bool checkNot();

    private:
      std::set<Trigger*> children_;
      TriggerMode mode_;
      float triggingTime_;
      float actualTime_;
      float radius_;
      bool bActive_;
      ClassTreeMask targetMask_;
      BillboardSet debugBillboard_;
  };

}

#endif /* _Trigger_H__ */
