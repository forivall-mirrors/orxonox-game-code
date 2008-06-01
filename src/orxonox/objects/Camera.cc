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
 *      Benjamin Knecht
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Camera.h"
#include "CameraHandler.h"

#include <string>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

#include "tinyxml/tinyxml.h"
#include "util/SubString.h"
#include "util/Convert.h"
#include "util/Math.h"
#include "core/Debug.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"

namespace orxonox
{

  Camera::Camera(Ogre::SceneNode* node)
  {
    this->bHasFocus_ = false;
    this->cameraNode_ = GraphicsEngine::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode(node->getName() + "Camera");
    if( node != NULL )
      this->setPositionNode(node);
  }

  Camera::~Camera()
  {
    CameraHandler::getInstance()->releaseFocus(this);
    GraphicsEngine::getSingleton().getSceneManager()->getRootSceneNode()->removeAndDestroyChild(cameraNode_->getName());
  }

  void Camera::setPositionNode(Ogre::SceneNode* node)
  {
    this->positionNode_ = node;
    // set camera to node values according to camera mode
  }

  void Camera::setTargetNode(Ogre::SceneNode* obj)
  {
    this->targetNode_ = obj;
  }

  void Camera::tick(float dt)
  {
    if (this->positionNode_ != NULL)
    {
      // this stuff here may need some adjustments
      Vector3 offset = this->positionNode_->getWorldPosition() - this->cameraNode_->getWorldPosition();
      float coeff = 15.0f * dt;
      if (coeff > 1.0f)
        coeff = 1.0f;

      this->cameraNode_->translate(coeff * offset);

      this->cameraNode_->setOrientation(Quaternion::Slerp(7.0f * dt, this->positionNode_->getWorldOrientation(), this->cameraNode_->getWorldOrientation(), false));
    }
  }

  /**
    don't move anything before here! here the Ogre camera is set to values of this camera
    always call update after changes
  */
  void Camera::update()
  {
    if(this->positionNode_ != NULL)
    {
      this->cameraNode_->setPosition(this->positionNode_->getWorldPosition());
      this->cameraNode_->setOrientation(this->positionNode_->getWorldOrientation());
    }
  }

  /**
    what to do when camera loses focus (do not request focus in this function!!)
    this is called by the CameraHandler singleton class to notify the camera
  */
  void Camera::removeFocus()
  {
    this->bHasFocus_ = false;
    this->cameraNode_->detachObject(this->cam_);
  }

  void Camera::setFocus(Ogre::Camera* ogreCam)
  {
    this->bHasFocus_ = true;
    this->cam_ = ogreCam;
    this->cam_->setOrientation(this->cameraNode_->getWorldOrientation());
    this->cameraNode_->attachObject(this->cam_);
  }
}
