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

#include "SpaceShip.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Template.h"
#include "core/XMLPort.h"
#include "tools/Shader.h"
#include "util/Math.h"

#include "graphics/Camera.h"
#include "items/Engine.h"

#include "CameraManager.h"
#include "Scene.h"

namespace orxonox
{
    CreateFactory(SpaceShip);

    SpaceShip::SpaceShip(BaseObject* creator) : Pawn(creator), boostBlur_(NULL)
    {
        RegisterObject(SpaceShip);

        this->bInvertYAxis_ = false;

        this->steering_ = Vector3::ZERO;

        this->rotationThrust_ =  10;
        this->localAngularAcceleration_.setValue(0, 0, 0);

        this->bBoost_ = false;
        this->bBoostCooldown_ = false;
        this->initialBoostPower_ = 10.0f;
        this->boostPower_ = 10.0f;
        this->boostPowerRate_ = 1.0f;
        this->boostRate_ = 5.0f;
        this->boostCooldownDuration_ = 5.0f;

        this->shakeFrequency_ = 15.0f;
        this->shakeAmplitude_ = 5.0f;
        this->shakeDt_ = 0.0f;
        this->cameraOriginalPosition_ = Vector3::UNIT_SCALE;
        this->cameraOriginalOrientation_ = Quaternion::IDENTITY;

        this->lift_ = 1.0f;
        this->stallSpeed_ = 220.0f;

        this->setDestroyWhenPlayerLeft(true);

        // SpaceShip is always a physical object per default
        // Be aware of this call: The collision type legality check will not reach derived classes!
        this->setCollisionType(WorldEntity::Dynamic);
        // Get notification about collisions
        this->enableCollisionCallback();

        this->setConfigValues();
        this->registerVariables();

    }

    SpaceShip::~SpaceShip()
    {
        if (this->isInitialized())
        {
            this->removeAllEngines();

            if (this->boostBlur_)
                this->boostBlur_->destroy();
        }
    }

    void SpaceShip::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceShip, XMLPort, xmlelement, mode);

        XMLPortParamVariable(SpaceShip, "rotationThrust", rotationThrust_, xmlelement, mode);
        XMLPortParam(SpaceShip, "boostPower", setInitialBoostPower, getInitialBoostPower, xmlelement, mode);
        XMLPortParam(SpaceShip, "boostPowerRate", setBoostPowerRate, getBoostPowerRate, xmlelement, mode);
        XMLPortParam(SpaceShip, "boostRate", setBoostRate, getBoostRate, xmlelement, mode);
        XMLPortParam(SpaceShip, "boostCooldownDuration", setBoostCooldownDuration, getBoostCooldownDuration, xmlelement, mode);
        XMLPortParam(SpaceShip, "shakeFrequency", setShakeFrequency, getShakeFrequency, xmlelement, mode);
        XMLPortParam(SpaceShip, "shakeAmplitude", setShakeAmplitude, getShakeAmplitude, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "lift", lift_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "stallSpeed", stallSpeed_, xmlelement, mode);

        XMLPortObject(SpaceShip, Engine, "engines", addEngine, getEngine, xmlelement, mode);
    }

    void SpaceShip::registerVariables()
    {
        registerVariable(this->rotationThrust_, VariableDirection::ToClient);
        registerVariable(this->boostPower_, VariableDirection::ToClient);
        registerVariable(this->boostPowerRate_, VariableDirection::ToClient);
        registerVariable(this->boostRate_, VariableDirection::ToClient);
        registerVariable(this->boostCooldownDuration_, VariableDirection::ToClient);
        registerVariable(this->shakeFrequency_, VariableDirection::ToClient);
        registerVariable(this->shakeAmplitude_, VariableDirection::ToClient);
        registerVariable(this->lift_, VariableDirection::ToClient);
        registerVariable(this->stallSpeed_, VariableDirection::ToClient);
    }

    void SpaceShip::setConfigValues()
    {
        SetConfigValue(bInvertYAxis_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = ship down).");
        
        SetConfigValueExternal(bEnableMotionBlur_, "GraphicsSettings", "enableMotionBlur", true)
            .description("Enable or disable the motion blur effect when moving very fast")
            .callback(this, &SpaceShip::changedEnableMotionBlur);
        SetConfigValueExternal(blurStrength_, "GraphicsSettings", "blurStrength", 3.0f)
            .description("Defines the strength of the motion blur effect");
    }

    bool SpaceShip::isCollisionTypeLegal(WorldEntity::CollisionType type) const
    {
        if (type != WorldEntity::Dynamic)
        {
            orxout(internal_warning) << "Cannot tell a SpaceShip not to be dynamic! Ignoring." << endl;
            assert(false); // Only in debug mode
            return false;
        }
        else
            return true;
    }

    void SpaceShip::tick(float dt)
    {
        SUPER(SpaceShip, tick, dt);

        // Run the engines
        for(std::vector<Engine*>::iterator it = this->engineList_.begin(); it != this->engineList_.end(); it++)
            (*it)->run(dt);

        if (this->hasLocalController())
        {
            // If not in mouse look apply the angular movement to the ship.
            if (!this->isInMouseLook())
            {
                this->localAngularAcceleration_ *= this->getLocalInertia() * this->rotationThrust_;
                this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * this->localAngularAcceleration_);
            }
            this->localAngularAcceleration_.setValue(0, 0, 0);

            // If not in boost cooldown, the boost power is recharged up to the initial boost power.
            if(!this->isBoostCoolingDown() && this->boostPower_ < this->initialBoostPower_)
            {
                this->boostPower_ += this->boostPowerRate_*dt;
            }
            // If boosting
            if(this->isBoosting())
            {
                // Discount the used power
                this->boostPower_ -= this->boostRate_*dt;
                // If the power has been used up boosting is stopped and boost cooldown is initiated.
                if(this->boostPower_ <= 0.0f)
                {
                    this->boost(false);
                    this->bBoostCooldown_ = true;
                    this->timer_.setTimer(this->boostCooldownDuration_, false, createExecutor(createFunctor(&SpaceShip::boostCooledDown, this)));
                }

                // Shake the camera because of the boosting.
                this->shakeCamera(dt);
            }

            // Enable Blur depending on settings
            if(this->bEnableMotionBlur_)
            {
                if (this->boostBlur_ == NULL && this->hasLocalController() && this->hasHumanController())
                {
                    this->boostBlur_ = new Shader(this->getScene()->getSceneManager());
                    this->boostBlur_->setCompositorName("Radial Blur");
                }
                if (this->boostBlur_)
                {
                    float blur = this->blurStrength_ * clamp(-this->getLocalVelocity().z/(this->getMaxSpeedFront()*this->getBoostFactor()), 0.0f, 1.0f);

                    // Show and hide blur effect depending on state of booster
                    if(this->isBoosting())
                        this->boostBlur_->setVisible(blur > 0.0f);
                    else
                        this->boostBlur_->setVisible(false);

                    this->boostBlur_->setParameter(0, 0, "sampleStrength", blur);
                }
            }
        }

        this->steering_ = Vector3::ZERO;
    }

    /**
    @brief
        Rotate in yaw direction.
        Due to added lift, can also lead to an additional right-left motion.
    @param value
        A vector whose first component specifies the magnitude of the rotation. Positive means yaw left, negative means yaw right.
    */
    void SpaceShip::rotateYaw(const Vector2& value)
    {
        this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() + value.x);

        Pawn::rotateYaw(value);

        // This function call adds a lift to the ship when it is rotating to make it's movement more "realistic" and enhance the feeling.
        if (this->getLocalVelocity().z < 0 && abs(this->getLocalVelocity().z) < stallSpeed_)
            this->moveRightLeft(-lift_ / 5.0f * value * sqrt(abs(this->getLocalVelocity().z)));
    }

    /**
    @brief
        Rotate in pitch direction.
        Due to added left, can also lead to an additional up-down motion.
    @param value
        A vector whose first component specifies the magnitude of the rotation. Positive means pitch up, negative means pitch down.
    */
    void SpaceShip::rotatePitch(const Vector2& value)
    {
        Vector2 pitch = value;
        if(this->bInvertYAxis_)
            pitch.x = -pitch.x;

        this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + pitch.x*0.8f);

        Pawn::rotatePitch(pitch);

        // This function call adds a lift to the ship when it is pitching to make it's movement more "realistic" and enhance the feeling.
        if (this->getLocalVelocity().z < 0 && abs(this->getLocalVelocity().z) < stallSpeed_)
            this->moveUpDown(lift_ / 5.0f * pitch * sqrt(abs(this->getLocalVelocity().z)));
    }

    /**
    @brief
        Rotate in roll direction.
    @param value
        A vector whose first component specifies the magnitude of the rotation. Positive means roll left, negative means roll right.
    */
    void SpaceShip::rotateRoll(const Vector2& value)
    {
        this->localAngularAcceleration_.setZ(this->localAngularAcceleration_.z() + value.x);

        Pawn::rotateRoll(value);
    }

    void SpaceShip::fire()
    {
    }

    /**
    @brief
        Starts or stops boosting.
    @param bBoost
        Whether to start or stop boosting.
    */
    void SpaceShip::boost(bool bBoost)
    {
        // Can only boost if not cooling down.
        if(bBoost && !this->isBoostCoolingDown())
        {
            this->bBoost_ = true;
            this->backupCamera();
        }
        // Stop boosting.
        if(!bBoost)
        {
            this->bBoost_ = false;
            this->resetCamera();
        }
    }

    void SpaceShip::addEngine(orxonox::Engine* engine)
    {
        OrxAssert(engine != NULL, "The engine cannot be NULL.");
        this->engineList_.push_back(engine);
        engine->addToSpaceShip(this);
    }

    /**
    @brief
        Check whether the SpaceShip has a particular Engine.
    @param engine
        A pointer to the Engine to be checked.
    */
    bool SpaceShip::hasEngine(Engine* engine) const
    {
        for(unsigned int i = 0; i < this->engineList_.size(); i++)
        {
            if(this->engineList_[i] == engine)
                return true;
        }
        return false;
    }

    /**
    @brief
        Get the i-th Engine of the SpaceShip.
    @return
        Returns a pointer to the i-the Engine. NULL if there is no Engine with that index.
    */
    Engine* SpaceShip::getEngine(unsigned int i)
    {
        if(this->engineList_.size() >= i)
            return NULL;
        else
            return this->engineList_[i];
    }

    /**
    @brief
        Remove and destroy all Engines of the SpaceShip.
    */
    void SpaceShip::removeAllEngines()
    {
        while(this->engineList_.size())
            this->engineList_.back()->destroy();
    }

    /**
    @brief
        Remove a particular Engine from the SpaceShip.
    @param engine
        A pointer to the Engine to be removed.
    */
    void SpaceShip::removeEngine(Engine* engine)
    {
        for(std::vector<Engine*>::iterator it = this->engineList_.begin(); it != this->engineList_.end(); ++it)
        {
            if(*it == engine)
            {
                this->engineList_.erase(it);
                return;
            }
        }
    }

    /**
    @brief
        Add to the speed factor for all engines of the SpaceShip.
    @param factor
        The factor to be added.
    */
    void SpaceShip::addSpeedFactor(float factor)
    {
        for(unsigned int i=0; i<this->engineList_.size(); i++)
            this->engineList_[i]->addSpeedMultiply(factor);
    }

    /**
    @brief
        Add to the speed factor for all engines of the SpaceShip.
    @param speed
        The speed to be added.
    */
    void SpaceShip::addSpeed(float speed)
    {
        for(unsigned int i=0; i<this->engineList_.size(); i++)
            this->engineList_[i]->addSpeedAdd(speed);
    }

    /**
    @brief
        Get the mean speed factor over all engines of the SpaceShip.
    @return
        Returns the mean speed factor over all engines of the SpaceShip.
    */
    float SpaceShip::getSpeedFactor() const
    {
        float sum = 0;
        unsigned int i = 0;
        for(; i<this->engineList_.size(); i++)
            sum += this->engineList_[i]->getSpeedMultiply();
        return sum/float(i);
    }

    /**
    @brief
        Get the largest maximal forward speed over all engines of the SpaceShip.
    @return
        Returns the largest forward speed over all engines of the SpaceShip.
    */
    float SpaceShip::getMaxSpeedFront() const
    {
        float speed=0;
        for(unsigned int i=0; i<this->engineList_.size(); i++)
        {
            if(this->engineList_[i]->getMaxSpeedFront() > speed)
                speed = this->engineList_[i]->getMaxSpeedFront();
        }
        return speed;
    }

    /**
    @brief
        Get the mean boost factor over all engines of the SpaceShip.
    @return
        Returns the mean boost factor over all engines of the SpaceShip.
    */
    float SpaceShip::getBoostFactor() const
    {
        float sum = 0;
        unsigned int i=0;
        for(; i<this->engineList_.size(); i++)
            sum += this->engineList_[i]->getBoostFactor();
        return sum/float(i);
    }

    /**
    @brief
        Is called when the enableMotionBlur config value has changed.
    */
    void SpaceShip::changedEnableMotionBlur()
    {
        if (!this->bEnableMotionBlur_ && this->boostBlur_ != NULL)
        {
            this->boostBlur_->destroy();
            this->boostBlur_ = NULL;
        }
    }

    /**
    @brief
        Shake the camera for a given time interval.
    @param dt
        The time interval in seconds.
    */
    void SpaceShip::shakeCamera(float dt)
    {
        // Make sure the ship is only shaking if it's moving forward and above the maximal forward speed.
        if (-this->getLocalVelocity().z > this->getMaxSpeedFront())
        {
            this->shakeDt_ += dt;

            float frequency = this->shakeFrequency_ * (square(abs(this->getLocalVelocity().z)));

            if (this->shakeDt_ >= 1.0f/frequency)
                this->shakeDt_ -= 1.0f/frequency;

            Degree angle = Degree(sin(this->shakeDt_ * math::twoPi * frequency) * this->shakeAmplitude_);

            Camera* camera = this->getCamera();
            //Shaking Camera effect
            if (camera != 0)
                camera->setOrientation(Vector3::UNIT_X, angle);

        }
        // If the camera is no shaking, reset it.
        else
            this->resetCamera();
    }

    /**
    @brief
        Save the original position and orientation of the camera.
    */
    void SpaceShip::backupCamera()
    {
        Camera* camera = CameraManager::getInstance().getActiveCamera();
        if(camera != NULL)
        {
            this->cameraOriginalPosition_ = camera->getPosition();
            this->cameraOriginalOrientation_ = camera->getOrientation();
        }
    }

    /**
    @brief
        Reset the camera to its original position.
    */
    void SpaceShip::resetCamera()
    {
	if(this->hasLocalController() && this->hasHumanController())
	{
            Camera *camera = this->getCamera();
            if (camera == 0)
            {
                orxout(internal_warning) << "Failed to reset camera!" << endl;
                return;
            }
            this->shakeDt_ = 0.0f;
            camera->setPosition(this->cameraOriginalPosition_);
            camera->setOrientation(this->cameraOriginalOrientation_);
	}
    }

}
