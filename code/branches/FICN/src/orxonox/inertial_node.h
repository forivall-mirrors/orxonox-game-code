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


#ifndef INERTIAL_NODE_H
#define INERTIAL_NODE_H

#include "OgrePrerequisites.h"

#include "orxonox_prerequisites.h"


namespace orxonox {

  class InertialNode
  {
  public:
    InertialNode(Ogre::SceneNode*, Ogre::Vector3);
	  ~InertialNode();

    InertialNode* createChildNode();

    InertialNode* getParentNode();

    Ogre::SceneNode* getSceneNode();

    Ogre::Vector3 getSpeed();

    Ogre::Vector3 getWorldSpeed();

    void addSpeed(Ogre::Vector3);

  protected:
    InertialNode(InertialNode*, Ogre::Vector3 speed = Ogre::Vector3::ZERO);
    void addChild(InertialNode*);

  public:

  protected:
    InertialNode **childList_;
    int childListSize_;
    int childListIndex_;

    Ogre::SceneNode *node_;
	  Ogre::Vector3 speed_;

    InertialNode* parentNode_;

  };

}

#endif /* INERTIAL_NODE_H */
