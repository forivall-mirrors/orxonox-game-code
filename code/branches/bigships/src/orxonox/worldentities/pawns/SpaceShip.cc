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
#include "items/Engine.h"

// New as of Booster integration
#include "Scene.h"
#include "tools/Shader.h"

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
        //this->engine_ = 0;

        this->boostPower_ = 10.0f;
        this->initialBoostPower_ = 10.0f;
        this->boostRate_ = 5.0;
        this->boostPowerRate_ = 1.0;
        this->boostCooldownDuration_ = 5.0;
        this->bBoostCooldown_ = false;

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

		XMLPortObject(SpaceShip, Engine, "engines", addEngine, getEngine, xmlelement, mode);
    }

    void SpaceShip::registerVariables()
    {
        registerVariable(this->primaryThrust_,  VariableDirection::ToClient);
        registerVariable(this->auxilaryThrust_, VariableDirection::ToClient);
        registerVariable(this->rotationThrust_, VariableDirection::ToClient);
        registerVariable(this->boostPower_, VariableDirection::ToClient);
        registerVariable(this->boostPowerRate_, VariableDirection::ToClient);
        registerVariable(this->boostRate_, VariableDirection::ToClient);
        registerVariable(this->boostCooldownDuration_, VariableDirection::ToClient);
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
                    this->bBoost_ = false;
                    this->bBoostCooldown_ = true;
                    this->timer_.setTimer(this->boostCooldownDuration_, false, createExecutor(createFunctor(&SpaceShip::boostCooledDown, this)));
                }
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

    void SpaceShip::boostCooledDown(void)
    {
        this->bBoostCooldown_ = false;
    }

    void SpaceShip::moveFrontBack(const Vector2& value)
    {
        this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() - value.x);
        this->steering_.z = -value.x;
    }

    void SpaceShip::moveRightLeft(const Vector2& value)
    {
        this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() + value.x);
        this->steering_.x = value.x;
    }

    void SpaceShip::moveUpDown(const Vector2& value)
    {
        this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() + value.x);
        this->steering_.y = value.x;
    }

    void SpaceShip::rotateYaw(const Vector2& value)
    {
        this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() + value.x);

        Pawn::rotateYaw(value);
    }

    void SpaceShip::rotatePitch(const Vector2& value)
    {
        this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x);

        Pawn::rotatePitch(value);
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
            this->bBoost_ = true;
        if(!bBoost)
            this->bBoost_ = false;
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
		for(unsigned int i=0; i<this->engineList_.size(); i++)
			this->engineList_[i]->~Engine();
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
