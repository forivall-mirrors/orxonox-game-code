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
#include "util/Debug.h" // TODO: Needed?
#include "util/Math.h"

#include "graphics/Camera.h"
#include "items/Engine.h"

#include "CameraManager.h"
#include "Scene.h"

namespace orxonox
{
    const float orientationGain = 100;
    CreateFactory(SpaceShip);

    SpaceShip::SpaceShip(BaseObject* creator) : Pawn(creator), boostBlur_(NULL)
    {
        RegisterObject(SpaceShip);

        this->primaryThrust_  = 100;
        this->auxilaryThrust_ =  30;
        this->rotationThrust_ =  10;

        this->localLinearAcceleration_.setValue(0, 0, 0);
        this->localAngularAcceleration_.setValue(0, 0, 0);
        this->bBoost_ = false;
        this->steering_ = Vector3::ZERO;

        this->boostPower_ = 10.0f;
        this->initialBoostPower_ = 10.0f;
        this->boostRate_ = 5.0;
        this->boostPowerRate_ = 1.0;
        this->boostCooldownDuration_ = 5.0;
        this->bBoostCooldown_ = false;

        this->lift_ = 1.0f;                         // factor of the lift, standard is 1
        this->stallSpeed_ = 220.0f;                 // max speed where lift is added

        this->bInvertYAxis_ = false;

        this->setDestroyWhenPlayerLeft(true);

        // SpaceShip is always a physical object per default
        // Be aware of this call: The collision type legality check will not reach derived classes!
        this->setCollisionType(WorldEntity::Dynamic);
        // Get notification about collisions
        this->enableCollisionCallback();

        this->engineTicksNotDone = 0;
        this->setConfigValues();
        this->registerVariables();
        
        this->cameraOriginalPosition_ = Vector3::UNIT_SCALE;
        this->cameraOriginalOrientation_ = Quaternion::IDENTITY;

        this->shakeFrequency_ = 15;
        this->shakeAmplitude_ = 5;
        this->shakeDt_ = 0;
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

        //XMLPortParam(SpaceShip, "engine",            setEngineTemplate,    getEngineTemplate,    xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "primaryThrust",  primaryThrust_,  xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "auxilaryThrust", auxilaryThrust_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "rotationThrust", rotationThrust_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "boostPower", initialBoostPower_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "boostPowerRate", boostPowerRate_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "boostRate", boostRate_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "boostCooldownDuration", boostCooldownDuration_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "shakeFrequency", shakeFrequency_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "shakeAmplitude", shakeAmplitude_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "lift", lift_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "stallSpeed", stallSpeed_, xmlelement, mode);

        XMLPortObject(SpaceShip, Engine, "engines", addEngine, getEngine, xmlelement, mode);
    }

    void SpaceShip::registerVariables()
    {
        registerVariable(this->primaryThrust_,  VariableDirection::ToClient);
        registerVariable(this->auxilaryThrust_, VariableDirection::ToClient);
        registerVariable(this->rotationThrust_, VariableDirection::ToClient);
        // TODO: Synchronization of boost needed?
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
            CCOUT(1) << "Error: Cannot tell a SpaceShip not to be dynamic! Ignoring." << std::endl;
            assert(false); // Only in debug mode
            return false;
        }
        else
            return true;
    }

    void SpaceShip::tick(float dt)
    {
        SUPER(SpaceShip, tick, dt);

        if (this->hasLocalController())
        {
            // Handle mouse look
            if (!this->isInMouseLook())
            {
                this->localAngularAcceleration_ *= this->getLocalInertia() * this->rotationThrust_;
                this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * this->localAngularAcceleration_);
            }
            this->localAngularAcceleration_.setValue(0, 0, 0);

            // Charge boostPower
            if(!this->bBoostCooldown_ && this->boostPower_ < this->initialBoostPower_)
            {
                this->boostPower_ += this->boostPowerRate_*dt;
            }
            // Use boostPower
            if(this->bBoost_)
            {
                this->boostPower_ -=this->boostRate_*dt;
                if(this->boostPower_ <= 0.0f)
                {
                    this->boost(false);
                    this->bBoostCooldown_ = true;
                    this->timer_.setTimer(this->boostCooldownDuration_, false, createExecutor(createFunctor(&SpaceShip::boostCooledDown, this)));
                }

                this->shakeCamera(dt);
            }

            // Enable Blur depending on settings
            if (this->bEnableMotionBlur_ && !this->boostBlur_ && this->hasLocalController() && this->hasHumanController())
            {
                this->boostBlur_ = new Shader(this->getScene()->getSceneManager());
                this->boostBlur_->setCompositorName("Radial Blur");
            }

            if (this->boostBlur_) // && this->maxSpeedFront_ != 0 && this->boostFactor_ != 1)
            {
                // TODO: this->maxSpeedFront_ gets fastest engine
                float blur = this->blurStrength_ * clamp((-this->getLocalVelocity().z - 0.0f /*this->maxSpeedFront_*/) / ((150.0f /*boostFactor_*/ - 1) * 1.5f /*this->maxSpeedFront_*/), 0.0f, 1.0f);

                // Show and hide blur effect depending on state of booster
                if(this->bBoost_)
                    this->boostBlur_->setVisible(blur > 0);
                else
                    this->boostBlur_->setVisible(false);

                this->boostBlur_->setParameter(0, 0, "sampleStrength", blur);
            }
        }
    }

    void SpaceShip::moveFrontBack(const Vector2& value)
    {
        this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() - value.x);
        this->steering_.z -= value.x;
    }

    void SpaceShip::moveRightLeft(const Vector2& value)
    {
        this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() + value.x);
        this->steering_.x += value.x;
    }

    void SpaceShip::moveUpDown(const Vector2& value)
    {
        this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() + value.x);
        this->steering_.y += value.x;
    }

    void SpaceShip::rotateYaw(const Vector2& value)
    {
        this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() + value.x);

        Pawn::rotateYaw(value);

        //This function call adds a lift to the ship when it is rotating to make it's movement more "realistic" and enhance the feeling.
        if (abs(this-> getLocalVelocity().z) < stallSpeed_)  {this->moveRightLeft(-lift_ / 5 * value * sqrt(abs(this-> getLocalVelocity().z)));}
    }

    void SpaceShip::rotatePitch(const Vector2& value)
    {
        this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x*0.8f);

        Pawn::rotatePitch(value);

        //This function call adds a lift to the ship when it is pitching to make it's movement more "realistic" and enhance the feeling.
        if (abs(this-> getLocalVelocity().z) < stallSpeed_)  {this->moveUpDown(lift_ / 5 * value * sqrt(abs(this-> getLocalVelocity().z)));}
    }

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
        if(bBoost && !this->bBoostCooldown_)
        {
            this->bBoost_ = true;
            Camera* camera = CameraManager::getInstance().getActiveCamera();
            this->cameraOriginalPosition_ = camera->getPosition();
            this->cameraOriginalOrientation_ = camera->getOrientation();
        }
        if(!bBoost)
        {
            this->bBoost_ = false;
            this->resetCamera();
        }
    }

    void SpaceShip::boostCooledDown(void)
    {
        this->bBoostCooldown_ = false;
    }
    
    void SpaceShip::shakeCamera(float dt)
    {
        //make sure the ship is only shaking if it's moving
        if (this->getVelocity().squaredLength() > 80.0f)
        {
            this->shakeDt_ += dt;
    
            float frequency = this->shakeFrequency_ * (this->getVelocity().squaredLength());
    
            if (this->shakeDt_ >= 1.0f/frequency)
            {
                this->shakeDt_ -= 1.0f/frequency;
            }
    
            Degree angle = Degree(sin(this->shakeDt_ *2.0f* math::pi * frequency) * this->shakeAmplitude_);
    
            //COUT(0) << "Angle: " << angle << std::endl;
            Camera* camera = this->getCamera();

            //Shaking Camera effect
            if (camera != 0)
            {
                camera->setOrientation(Vector3::UNIT_X, angle);
            }
        }
    }

    void SpaceShip::resetCamera()
    {
        Camera *camera = this->getCamera();

        if (camera == 0)
        {
            COUT(2) << "Failed to reset camera!";
            return;
        }
    
        this->shakeDt_ = 0;
        camera->setPosition(this->cameraOriginalPosition_);
        camera->setOrientation(this->cameraOriginalOrientation_);
    }

    void SpaceShip::backupCamera()
    {
        Camera* camera = CameraManager::getInstance().getActiveCamera();
        if(camera != NULL)
        {
            this->cameraOriginalPosition_ = camera->getPosition();
            this->cameraOriginalOrientation_ = camera->getOrientation();
        }
    }

    void SpaceShip::addEngine(orxonox::Engine* engine)
    {
        //COUT(0)<<"Adding an Engine: " << engine << endl;
        this->engineList_.push_back(engine);
        engine->addToSpaceShip(this);
        this->resetEngineTicks();
    }

    bool SpaceShip::hasEngine(Engine* engine)
    {
        for(unsigned int i=0; i<this->engineList_.size(); i++)
        {
            if(this->engineList_[i]==engine)
                return true;
        }
        return false;
    }

    Engine* SpaceShip::getEngine(unsigned int i)
    {
        if(this->engineList_.size()>=i)
            return 0;
        else
            return this->engineList_[i];
    }

    void SpaceShip::removeAllEngines()
    {
        while(this->engineList_.size())
            this->engineList_.back()->destroy();
    }
    
    void SpaceShip::removeEngine(Engine* engine)
    {
        for(std::vector<Engine*>::iterator it=this->engineList_.begin(); it!=this->engineList_.end(); ++it)
        {
            if(*it==engine)
            {
                this->engineList_.erase(it);
                return;
            }
        }
    }

    void SpaceShip::setSpeedFactor(float factor)
    {
        for(unsigned int i=0; i<this->engineList_.size(); i++)
            this->engineList_[i]->setSpeedFactor(factor);
    }
    float SpaceShip::getSpeedFactor() // Calculate mean SpeedFactor.
    {
        float ret = 0; unsigned int i = 0;
        for(; i<this->engineList_.size(); i++)
            ret += this->engineList_[i]->getSpeedFactor();
        ret /= (float)i;
        return ret;
    }
    float SpaceShip::getMaxSpeedFront()
    {
        float ret=0;
        for(unsigned int i=0; i<this->engineList_.size(); i++)
        {
            if(this->engineList_[i]->getMaxSpeedFront() > ret)
                ret = this->engineList_[i]->getMaxSpeedFront();
        }
        return ret;
    }

    float SpaceShip::getBoostFactor()
    {
        float ret = 0; unsigned int i=0;
        for(; i<this->engineList_.size(); i++)
            ret += this->engineList_[i]->getBoostFactor();
        ret /= (float)i;
        return ret;
    }

    std::vector<PickupCarrier*>* SpaceShip::getCarrierChildren(void) const
    {
        std::vector<PickupCarrier*>* list = new std::vector<PickupCarrier*>();
        for(unsigned int i=0; i<this->engineList_.size(); i++)
            list->push_back(this->engineList_[i]);
        return list;
    }
    
    void SpaceShip::changedEnableMotionBlur()
    {
        if (!this->bEnableMotionBlur_)
        {
            this->boostBlur_->destroy();
            this->boostBlur_ = 0;
        }
    }

}
