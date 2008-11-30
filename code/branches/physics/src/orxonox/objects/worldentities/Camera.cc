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

#include <string>
#include <cassert>

#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreViewport.h>

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "objects/Scene.h"
#include "CameraManager.h"

namespace orxonox
{
    CreateFactory(Camera);

    Camera::Camera(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Camera);

        assert(this->getScene());
        assert(this->getScene()->getSceneManager());

        this->camera_ = this->getScene()->getSceneManager()->createCamera(getUniqueNumberString());
        this->attachOgreObject(this->camera_);

        this->bHasFocus_ = false;
        this->bDrag_ = false;
        this->nearClipDistance_ = 1;

        this->setObjectMode(0x0);

        this->setConfigValues();
        this->configvaluecallback_changedNearClipDistance();

        this->requestFocus(); // ! HACK ! REMOVE THIS !
    }

    Camera::~Camera()
    {
        if (this->isInitialized())
        {
            this->releaseFocus();
        }
    }

    void Camera::setConfigValues()
    {
        SetConfigValue(nearClipDistance_, 1.0f).callback(this, &Camera::configvaluecallback_changedNearClipDistance);
    }

    void Camera::configvaluecallback_changedNearClipDistance()
    {
        this->camera_->setNearClipDistance(this->nearClipDistance_);
    }

    void Camera::tick(float dt)
    {
/*
        // this stuff here may need some adjustments
        float coeff = (this->bDrag_) ? min(1.0f, 15.0f * dt) : (1.0f);

        Vector3 offset = this->getNode()->getWorldPosition() - this->cameraNode_->getWorldPosition();
        this->cameraNode_->translate(coeff * offset);

        this->cameraNode_->setOrientation(Quaternion::Slerp(coeff, this->cameraNode_->getWorldOrientation(), this->getWorldOrientation(), false));
*/
    }

    void Camera::requestFocus()
    {
        CameraManager::getInstance().requestFocus(this);
    }

    void Camera::releaseFocus()
    {
        CameraManager::getInstance().releaseFocus(this);
    }

    /**
        what to do when camera loses focus (do not request focus in this function!!)
        this is called by the CameraManager singleton class to notify the camera
    */
    void Camera::removeFocus()
    {
        this->bHasFocus_ = false;
    }

    void Camera::setFocus(Ogre::Viewport* viewport)
    {
        this->bHasFocus_ = true;
        viewport->setCamera(this->camera_);
    }
}
