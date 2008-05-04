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
#include "util/SubString.h"
#include "util/Convert.h"
#include "util/Math.h"
#include "core/Debug.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"

namespace orxonox
{
  //CreateFactory(Camera);

  Camera::Camera(Ogre::SceneNode* node)
  {
    //RegisterObject(Camera);
    this->bHasFocus_ = false;
    if( node != NULL )
      this->setCameraNode(node);

  }

  Camera::~Camera()
  {
  }

  /*void Camera::loadParams(TiXmlElement* xmlElem)
  {
    Ogre::SceneManager* mgr = GraphicsEngine::getSingleton().getSceneManager();

    if (xmlElem->Attribute("name") && xmlElem->Attribute("pos") && xmlElem->Attribute("lookat") && xmlElem->Attribute("node"))
    {
      //    <Camera name="Camera" pos="0,0,-250" lookat="0,0,0" />

      std::string name = xmlElem->Attribute("name");
      std::string pos = xmlElem->Attribute("pos");
      std::string lookat = xmlElem->Attribute("lookat");

      this->cam_ = mgr->createCamera(name);

      float x, y, z;
      SubString posVec(xmlElem->Attribute("pos"), ',');
      convertValue<std::string, float>(&x, posVec[0]);
      convertValue<std::string, float>(&y, posVec[1]);
      convertValue<std::string, float>(&z, posVec[2]);

      setPosition(Vector3(x,y,z));

      //std::string target = xmlElem->Attribute("lookat");
      posVec.split(xmlElem->Attribute("lookat"), ',');
      convertValue<std::string, float>(&x, posVec[0]);
      convertValue<std::string, float>(&y, posVec[1]);
      convertValue<std::string, float>(&z, posVec[2]);

      cam_->lookAt(Vector3(x,y,z));

      /*std::string node = xmlElem->Attribute("node");

      Ogre::SceneNode* sceneNode = (Ogre::SceneNode*)mgr->getRootSceneNode()->createChildSceneNode(node); //getChild(node);
      sceneNode->attachObject((Ogre::MovableObject*)cam_);
      */

      // FIXME: unused var
      //Ogre::Viewport* vp =
      //GraphicsEngine::getSingleton().getRenderWindow()->addViewport(cam_);
    /*

      COUT(4) << "Loader: Created camera "<< name  << std::endl << std::endl;
    }
  }*/

  void Camera::setCameraNode(Ogre::SceneNode* node)
  {
    this->positionNode_ = node;
    // set camera to node values according to camera mode
  }

  void Camera::setTargetNode(Ogre::SceneNode* obj)
  {
    this->targetNode_ = obj;
  }

  /**
    don't move anything before here! here the Ogre camera is set to values of this camera
    always call update after changes
  */
  void Camera::update()
  {
    COUT(0) << "p " << this->positionNode_->getPosition() << std::endl;
    COUT(0) << "t " << this->targetNode_->getPosition() << std::endl;
    if(this->positionNode_ != NULL)
      //this->cam_->setPosition(this->positionNode_->getPosition());
    if(this->targetNode_ != NULL)
      this->cam_->lookAt(this->targetNode_->getPosition());
  }

  /**
    what to do when camera loses focus (do not request focus in this function!!)
    this is called by the CameraHandler singleton class to notify the camera
  */
  void Camera::removeFocus()
  {
    this->positionNode_->detachObject(cam_);
    this->bHasFocus_ = false;
  }

  void Camera::setFocus(Ogre::Camera* ogreCam)
  {
    this->bHasFocus_ = true;
    this->cam_ = ogreCam;
    this->positionNode_->attachObject(cam_);
  }
}
