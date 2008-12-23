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

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "objects/Scene.h"
#include "CameraManager.h"

namespace orxonox
{
    CreateFactory(Camera);

    Camera::Camera(BaseObject* creator) : PositionableEntity(creator)
    {
        RegisterObject(Camera);

        if (!this->getScene())
            ThrowException(AbortLoading, "Can't create Camera, no scene.");
        if (!this->getScene()->getSceneManager())
            ThrowException(AbortLoading, "Can't create Camera, no scene-manager given.");

        this->camera_ = this->getScene()->getSceneManager()->createCamera(getUniqueNumberString());
        this->cameraNode_ = this->getNode()->createChildSceneNode();
        this->cameraNode_->attachObject(this->camera_);

        this->bHasFocus_ = false;
        this->bDrag_ = false;
        this->nearClipDistance_ = 1;

        this->setObjectMode(0x0);

        this->setConfigValues();
        this->configvaluecallback_changedNearClipDistance();
    }

    Camera::~Camera()
    {
        if (this->isInitialized())
        {
            this->releaseFocus();

            this->cameraNode_->detachAllObjects();
            this->getScene()->getSceneManager()->destroyCamera(this->camera_);

            if (this->bDrag_)
                this->detachNode(this->cameraNode_);

            if (this->getScene()->getSceneManager())
                this->getScene()->getSceneManager()->destroySceneNode(this->cameraNode_->getName());
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
        SUPER(Camera, tick, dt);

        if (this->bDrag_)
        {
            // this stuff here may need some adjustments
            float coeff = min(1.0f, 15.0f * dt);

            Vector3 offset = this->getNode()->getWorldPosition() - this->cameraNode_->getWorldPosition();
            this->cameraNode_->translate(coeff * offset);

            this->cameraNode_->setOrientation(Quaternion::Slerp(coeff, this->cameraNode_->getWorldOrientation(), this->getWorldOrientation(), false));
            //this->cameraNode_->setOrientation(this->getWorldOrientation());
        }
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

    void Camera::setFocus()
    {
        this->bHasFocus_ = true;
        CameraManager::getInstance().useCamera(this->camera_);
    }

    void Camera::setDrag(bool bDrag)
    {
        if (bDrag != this->bDrag_)
        {
            this->bDrag_ = bDrag;

            if (!bDrag)
            {
                this->attachNode(this->cameraNode_);
                this->cameraNode_->setPosition(Vector3::ZERO);
                this->cameraNode_->setOrientation(Quaternion::IDENTITY);
            }
            else
            {
                this->detachNode(this->cameraNode_);
                this->cameraNode_->setPosition(this->getWorldPosition());
                this->cameraNode_->setOrientation(this->getWorldOrientation());
            }
        }
    }
}
