/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>

#include <string>

#include "../Orxonox.h"
#include "../GraphicsEngine.h"
#include "util/tinyxml/tinyxml.h"
#include "util/Tokenizer.h"
#include "util/String2Number.h"
#include "core/CoreIncludes.h"
#include "SpaceshipSteeringObject.h"

#include "../SpaceshipSteering.h"

namespace orxonox
{
    CreateFactory(SpaceshipSteeringObject);

    SpaceshipSteeringObject::SpaceshipSteeringObject()
    {
        RegisterObject(SpaceshipSteeringObject);
    }

    SpaceshipSteeringObject::~SpaceshipSteeringObject()
    {
    }

    void SpaceshipSteeringObject::loadParams(TiXmlElement* xmlElem)
    {
      SpaceshipSteering* steering = orxonox::Orxonox::getSingleton()->getSteeringPointer();

      if (xmlElem->Attribute("node") && xmlElem->Attribute("forward") && xmlElem->Attribute("rotateupdown") && xmlElem->Attribute("rotaterightleft") && xmlElem->Attribute("looprightleft"))
      {
        std::string nodeStr = xmlElem->Attribute("node");
        std::string forwardStr = xmlElem->Attribute("forward");
        std::string rotateupdownStr = xmlElem->Attribute("rotateupdown");
        std::string rotaterightleftStr = xmlElem->Attribute("rotaterightleft");
        std::string looprightleftStr = xmlElem->Attribute("looprightleft");

        float forward, rotateupdown, rotaterightleft, looprightleft;
        String2Number<float>(forward, forwardStr);
        String2Number<float>(rotateupdown, rotateupdownStr);
        String2Number<float>(rotaterightleft, rotaterightleftStr);
        String2Number<float>(looprightleft, looprightleftStr);

        steering = new SpaceshipSteering(forward, rotateupdown, rotaterightleft, looprightleft);

        Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();
        Ogre::SceneNode* sceneNode = (Ogre::SceneNode*)mgr->getRootSceneNode()->getChild(nodeStr);

        steering->addNode(sceneNode);




        COUT(4) << "Loader: Initialized spaceship steering at node " << nodeStr << " values "<< forward << " " << rotateupdown << " "<< rotaterightleft << " "<< looprightleft << " "<< std::endl << std::endl;
      }
   }
}
