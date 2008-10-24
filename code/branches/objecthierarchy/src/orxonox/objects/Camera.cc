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
#include "util/Math.h"
#include "util/Debug.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"

namespace orxonox
{
  CreateUnloadableFactory(Camera);

  Camera::Camera()
  {
    RegisterObject(Camera);

    this->bHasFocus_ = false;
    this->bDrag_ = false;
    this->cameraNode_ = GraphicsEngine::getInstance().getLevelSceneManager()->getRootSceneNode()->createChildSceneNode();
    this->setObjectMode(0x0);
  }

  Camera::~Camera()
  {
    if (this->isInitialized())
    {
      CameraHandler::getInstance()->releaseFocus(this);
      GraphicsEngine::getInstance().getLevelSceneManager()->getRootSceneNode()->removeAndDestroyChild(this->cameraNode_->getName());
    }
  }

  void Camera::tick(float dt)
  {
      // this stuff here may need some adjustments
      float coeff = (this->bDrag_) ? min(1.0f, 15.0f * dt) : (1.0f);

      Vector3 offset = this->getNode()->getWorldPosition() - this->cameraNode_->getWorldPosition();
      this->cameraNode_->translate(coeff * offset);

      this->cameraNode_->setOrientation(Quaternion::Slerp(coeff, this->cameraNode_->getWorldOrientation(), this->getWorldOrientation(), false));
  }

  /**
    don't move anything before here! here the Ogre camera is set to values of this camera
    always call update after changes
  */
  void Camera::update()
  {
      this->cameraNode_->setPosition(this->getWorldPosition());
      this->cameraNode_->setOrientation(this->getWorldOrientation());
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

  void Camera::requestFocus()
  {
    CameraHandler::getInstance()->requestFocus(this);
  }
}
