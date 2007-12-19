/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */


#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <OgrePrerequisites.h>

#include "orxonox/OrxonoxPrereqs.h"

#include "network/Synchronisable.h"
#include "tinyxml/tinyxml.h"
//#include "../core/CoreIncludes.h"
#include "../BaseObject.h"
#include "../Tickable.h"

namespace orxonox {
  class BulletManager : public BaseObject, public network::Synchronisable, public Tickable
  {
  public:
    BulletManager();
	  virtual ~BulletManager();

    void addBullet(Bullet*);

    int getAmmunitionID(const Ogre::String&);

    int getNumberOfAmmos();

    virtual void tick(float dt);

    inline virtual void loadParams(TiXmlElement* xmlElem) { BaseObject::loadParams(xmlElem); };

  protected:
    inline bool create() { return true; }

    // Bullet array
	  Bullet **bullets_;
	  int bulletsSize_;
	  int bulletsIndex_;
  };
}

#endif /* BULLET_MANAGER_H */
