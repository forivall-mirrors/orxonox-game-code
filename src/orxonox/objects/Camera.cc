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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Camera.h"

#include <string>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

#include "util/tinyxml/tinyxml.h"
#include "util/Substring.h"
#include "util/Convert.h"
#include "util/Math.h"
#include "core/Debug.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    CreateFactory(Camera);

    Camera::Camera()
    {
        RegisterObject(Camera);
    }

    Camera::~Camera()
    {
    }

    void Camera::loadParams(TiXmlElement* xmlElem)
    {
      Ogre::SceneManager* mgr = GraphicsEngine::getSingleton().getSceneManager();

      if (xmlElem->Attribute("name") && xmlElem->Attribute("pos") && xmlElem->Attribute("lookat") && xmlElem->Attribute("node"))
      {
        //    <Camera name="Camera" pos="0,0,-250" lookat="0,0,0" />

        std::string name = xmlElem->Attribute("name");
        std::string pos = xmlElem->Attribute("pos");
        std::string lookat = xmlElem->Attribute("lookat");

        Ogre::Camera *cam = mgr->createCamera(name);

        float x, y, z;
        SubString posVec(xmlElem->Attribute("pos"), ',');
        convertValue<std::string, float>(&x, posVec[0]);
        convertValue<std::string, float>(&y, posVec[1]);
        convertValue<std::string, float>(&z, posVec[2]);

        cam->setPosition(Vector3(x,y,z));

        posVec = SubString(xmlElem->Attribute("lookat"), ',');
        convertValue<std::string, float>(&x, posVec[0]);
        convertValue<std::string, float>(&y, posVec[1]);
        convertValue<std::string, float>(&z, posVec[2]);

        cam->lookAt(Vector3(x,y,z));

        std::string node = xmlElem->Attribute("node");

        Ogre::SceneNode* sceneNode = (Ogre::SceneNode*)mgr->getRootSceneNode()->createChildSceneNode(node); //getChild(node);
        sceneNode->attachObject((Ogre::MovableObject*)cam);

        // FIXME: unused var
        //Ogre::Viewport* vp = 
        GraphicsEngine::getSingleton().getRenderWindow()->addViewport(cam);


        COUT(4) << "Loader: Created camera "<< name  << std::endl << std::endl;
      }
   }
}
