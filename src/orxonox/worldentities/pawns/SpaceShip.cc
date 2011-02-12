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

namespace orxonox
{
    const float orientationGain = 100;
    CreateFactory(SpaceShip);

    SpaceShip::SpaceShip(BaseObject* creator) : Pawn(creator)
    {
        RegisterObject(SpaceShip);

        this->primaryThrust_  = 100;
        this->auxilaryThrust_ =  30;
        this->rotationThrust_ =  10;

        this->localLinearAcceleration_.setValue(0, 0, 0);
        this->localAngularAcceleration_.setValue(0, 0, 0);
        this->bBoost_ = false;
        this->bPermanentBoost_ = false;
        this->steering_ = Vector3::ZERO;
        this->engine_ = 0;

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

        this->setConfigValues();
        this->registerVariables();
    }

    SpaceShip::~SpaceShip()
    {
        if (this->isInitialized() && this->engine_)
            this->engine_->destroy();
    }

    void SpaceShip::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceShip, XMLPort, xmlelement, mode);

        XMLPortParam(SpaceShip, "engine",            setEngineTemplate,    getEngineTemplate,    xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "primaryThrust",  primaryThrust_,  xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "auxilaryThrust", auxilaryThrust_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "rotationThrust", rotationThrust_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "boostPower", initialBoostPower_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "boostPowerRate", boostPowerRate_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "boostRate", boostRate_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "boostCooldownDuration", boostCooldownDuration_, xmlelement, mode);
    }

    void SpaceShip::registerVariables()
    {
        registerVariable(this->primaryThrust_,  VariableDirection::ToClient);
        registerVariable(this->auxilaryThrust_, VariableDirection::ToClient);
        registerVariable(this->rotationThrust_, VariableDirection::ToClient);
    }

    void SpaceShip::setConfigValues()
    {
        SetConfigValue(bInvertYAxis_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = ship down).");
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
/*
            this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() * getMass() * this->auxilaryThrust_);
            this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() * getMass() * this->auxilaryThrust_);
            if (this->localLinearAcceleration_.z() > 0)
                this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() * getMass() * this->auxilaryThrust_);
            else
                this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() * getMass() * this->primaryThrust_);
            this->physicalBody_->applyCentralForce(physicalBody_->getWorldTransform().getBasis() * this->localLinearAcceleration_);
            this->localLinearAcceleration_.setValue(0, 0, 0);
*/
            if (!this->isInMouseLook())
            {
                this->localAngularAcceleration_ *= this->getLocalInertia() * this->rotationThrust_;
                this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * this->localAngularAcceleration_);
            }

            this->localAngularAcceleration_.setValue(0, 0, 0);

            if(!this->bBoostCooldown_ && this->boostPower_ < this->initialBoostPower_)
            {
                this->boostPower_ += this->boostPowerRate_*dt;
            }
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

    // TODO: something seems to call this function every tick, could probably handled a little more efficiently!
    void SpaceShip::setBoost(bool bBoost)
    {
        if(bBoost == this->bBoost_)
            return;

        if(bBoost)
            this->boost();
        else
        {
            this->bBoost_ = false;
        }
    }

    void SpaceShip::fire()
    {
    }

    void SpaceShip::boost()
    {
        if(!this->bBoostCooldown_)
            this->bBoost_ = true;
    }

    void SpaceShip::loadEngineTemplate()
    {
        if (!this->enginetemplate_.empty())
        {
            Template* temp = Template::getTemplate(this->enginetemplate_);

            if (temp)
            {
                Identifier* identifier = temp->getBaseclassIdentifier();

                if (identifier)
                {
                    BaseObject* object = identifier->fabricate(this);
                    this->engine_ = orxonox_cast<Engine*>(object);

                    if (this->engine_)
                    {
                        this->engine_->addTemplate(temp);
                        this->engine_->addToSpaceShip(this);
                    }
                    else
                    {
                        object->destroy();
                    }
                }
            }
        }
    }

    void SpaceShip::setEngine(Engine* engine)
    {
        this->engine_ = engine;
        if (engine && engine->getShip() != this)
            engine->addToSpaceShip(this);
    }

    std::vector<PickupCarrier*>* SpaceShip::getCarrierChildren(void) const
    {
        std::vector<PickupCarrier*>* list = new std::vector<PickupCarrier*>();
        list->push_back(this->engine_);
        return list;
    }
}
