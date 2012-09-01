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

#include "Camera.h"

#include <algorithm>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include "util/Exception.h"
#include "util/StringUtils.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/GameMode.h"
#include "core/GUIManager.h"
#include "Scene.h"
#include "CameraManager.h"
#include "sound/SoundManager.h"

namespace orxonox
{
    CreateFactory(Camera);

    Camera::Camera(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Camera);

        if (!GameMode::showsGraphics())
            ThrowException(AbortLoading, "Can't create Camera, no graphics.");
        if (!this->getScene())
            ThrowException(AbortLoading, "Can't create Camera, no scene.");
        if (!this->getScene()->getSceneManager())
            ThrowException(AbortLoading, "Can't create Camera, no scene-manager given.");
        if (!this->getScene()->getRootSceneNode())
            ThrowException(AbortLoading, "Can't create Camera, no root-scene-node given.");

        this->camera_ = this->getScene()->getSceneManager()->createCamera(getUniqueNumberString());
        static_cast<Ogre::MovableObject*>(this->camera_)->setUserAny(Ogre::Any(static_cast<OrxonoxClass*>(this)));
        this->cameraNode_ = this->getScene()->getRootSceneNode()->createChildSceneNode();
        this->attachNode(this->cameraNode_);
        this->cameraNode_->attachObject(this->camera_);

        this->bHasFocus_ = false;
        this->bDrag_ = false;
        this->lastDtLagged_ = false;

        this->setSyncMode(ObjectDirection::None);

        this->setConfigValues();

        this->configvaluecallback_changedFovAndAspectRatio();
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
        SetConfigValue(fov_, 80.0f)
            .description("Horizontal field of view in degrees")
            .callback(this, &Camera::configvaluecallback_changedFovAndAspectRatio);
        SetConfigValue(aspectRatio_, 1.0f)
            .description("Aspect ratio of pixels (width / height)")
            .callback(this, &Camera::configvaluecallback_changedFovAndAspectRatio);
        SetConfigValue(nearClipDistance_, 1.0f)
            .description("Distance from the camera where close objects will be clipped")
            .callback(this, &Camera::configvaluecallback_changedNearClipDistance);
    }

    /**
        @brief Update FOV and the aspect ratio of the camera after the config values or the window's size have changed.
    */
    void Camera::configvaluecallback_changedFovAndAspectRatio()
    {
        // the aspect ratio of the window (width / height) has to be multiplied with the pixels aspect ratio (this->aspectRatio_)
        float aspectRatio = this->aspectRatio_ * this->getWindowWidth() / this->getWindowHeight();
        this->camera_->setAspectRatio(aspectRatio);

        // Since we use horizontal FOV, we have to calculate FOVy by dividing by the aspect ratio and using some tangents
        Radian fovy(2 * atan(tan(Degree(this->fov_).valueRadians() / 2) / aspectRatio));
        this->camera_->setFOVy(fovy);
    }

    void Camera::configvaluecallback_changedNearClipDistance()
    {
        this->camera_->setNearClipDistance(this->nearClipDistance_);
    }

    /**
        @brief Inherited from WindowEventListener.
    */
    void Camera::windowResized(unsigned int newWidth, unsigned int newHeight)
    {
        this->configvaluecallback_changedFovAndAspectRatio();
    }

    void Camera::tick(float dt)
    {
        SUPER(Camera, tick, dt);

        if (this->bDrag_ && this->getTimeFactor() != 0)
        {
            // this stuff here may need some adjustments
            float poscoeff = 15.0f * dt / this->getTimeFactor();
            float anglecoeff = 7.0f * dt / this->getTimeFactor();
            // Only clamp if fps rate is actually falling. Occasional high dts should
            // not be clamped to reducing lagging effects.
            if (poscoeff > 1.0f)
            {
                if (this->lastDtLagged_)
                    poscoeff = 1.0f;
                else
                    this->lastDtLagged_ = true;
            }
            else
                this->lastDtLagged_ = false;

            if (anglecoeff > 1.0f)
            {
                if (this->lastDtLagged_)
                    anglecoeff = 1.0f;
                else
                    this->lastDtLagged_ = true;
            }
            else
                this->lastDtLagged_ = false;

            Vector3 offset = this->getWorldPosition() - this->cameraNode_->_getDerivedPosition();
            this->cameraNode_->translate(poscoeff * offset);

            this->cameraNode_->setOrientation(Quaternion::Slerp(anglecoeff, this->cameraNode_->_getDerivedOrientation(), this->getWorldOrientation(), true));
        }

        // Update sound listener transformation
        if (GameMode::playsSound() && this->bHasFocus_)
        {
            SoundManager::getInstance().setListenerPosition(this->getWorldPosition());
            SoundManager::getInstance().setListenerOrientation(this->getWorldOrientation());
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
