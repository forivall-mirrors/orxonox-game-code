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
    
  @author
    Benjamin Knecht

  @ingroup NormalTrigger
  */
  class _ObjectsExport Trigger : public TriggerBase
  {
    public:
      Trigger(BaseObject* creator);
      virtual ~Trigger();

      virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
      virtual void tick(float dt);

      inline bool isActive(void) const
        { return this->bActive_; }

      inline void setVisible(bool visibility)
        { this->debugBillboard_.setVisible(visibility); }

      void delayChanged(void);

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

      bool bActive_;
      bool bTriggered_;

      char latestState_;
      float remainingTime_;
      float timeSinceLastEvent_;

//      bool bUpdating_;
      BillboardSet debugBillboard_;

      std::queue<std::pair<float, char> > stateChanges_;
  };

}

#endif /* _Trigger_H__ */
