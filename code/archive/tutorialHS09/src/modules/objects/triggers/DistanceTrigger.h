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

#ifndef _DistanceTrigger_H__
#define _DistanceTrigger_H__

#include "objects/ObjectsPrereqs.h"

#include <set>
#include "core/ClassTreeMask.h"
#include "Trigger.h"
#include "interfaces/PlayerTrigger.h"

namespace orxonox
{
  class _ObjectsExport DistanceTrigger : public Trigger, public PlayerTrigger
  {
    public:
      DistanceTrigger(BaseObject* creator);
      virtual ~DistanceTrigger();

      virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

      void addTarget(Ogre::Node* targetNode);
      void addTargets(const std::string& targets);
      void removeTarget(Ogre::Node* targetNode);
      void removeTargets(const std::string& targets);

      inline void setDistance(float distance)
        { this->distance_ = distance; }
      inline float getDistance() const
        { return this->distance_; }

      bool checkDistance();

    protected:
      virtual bool isTriggered(TriggerMode::Value mode);
      virtual void notifyMaskUpdate() {}

      ClassTreeMask targetMask_;

    private:
      std::set<Ogre::Node*> targetSet_;
      float distance_;

  };
}

#endif /* _DistanceTrigger_H__ */
