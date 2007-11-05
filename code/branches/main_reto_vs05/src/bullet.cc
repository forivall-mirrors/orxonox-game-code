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

#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreVector3.h"

#include "bullet.h"


namespace orxonox {
  using namespace Ogre;

  Bullet::Bullet(SceneNode *node, Entity *entity, Vector3 speed)
    : node_(node), entity_(entity), speed_(speed)
  {
	  node_->attachObject(entity_);
  }


  Bullet::~Bullet()
  {
  }

}
