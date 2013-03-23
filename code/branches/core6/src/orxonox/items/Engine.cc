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

#include "Engine.h"

#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/Template.h"
#include "core/XMLPort.h"
#include "util/Math.h"

#include "Scene.h"
#include "worldentities/pawns/SpaceShip.h"

namespace orxonox
{
    CreateFactory(Engine);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    Engine::Engine(BaseObject* creator) : Item(creator)
    {
        RegisterObject(Engine);

        this->ship_ = 0;
        this->shipID_ = OBJECTID_UNKNOWN;
        this->relativePosition_ = Vector3::ZERO;

        this->boostFactor_ = 1.5f;

        this->maxSpeedFront_ = 0.0f;
        this->maxSpeedBack_ = 0.0f;
        this->maxSpeedLeftRight_ = 0.0f;
        this->maxSpeedUpDown_ = 0.0f;

        this->accelerationFront_ = 0.0f;
        this->accelerationBrake_ = 0.0f;
        this->accelerationBack_ = 0.0f;
        this->accelerationLeftRight_ = 0.0f;
        this->accelerationUpDown_ = 0.0f;

        this->speedAdd_ = 0.0f;
        this->speedMultiply_ = 1.0f;

        this->setConfigValues();
        this->registerVariables();
    }

    /**
    @brief
        Destructor. Destroys the object and removes it from the SpaceShip.
    */
    Engine::~Engine()
    {
        if (this->isInitialized())
        {
            // Remove the engine from the ShapeShip.
            if (this->ship_ && this->ship_->hasEngine(this))
                this->ship_->removeEngine(this);
        }
    }

    void Engine::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Engine, XMLPort, xmlelement, mode);

        XMLPortParam(Engine, "boostfactor", setBoostFactor, getBoostFactor, xmlelement, mode);

        XMLPortParam(Engine, "speedfront",     setMaxSpeedFront,     setMaxSpeedFront,     xmlelement, mode);
        XMLPortParam(Engine, "speedback",      setMaxSpeedBack,      setMaxSpeedBack,      xmlelement, mode);
        XMLPortParam(Engine, "speedleftright", setMaxSpeedLeftRight, setMaxSpeedLeftRight, xmlelement, mode);
        XMLPortParam(Engine, "speedupdown",    setMaxSpeedUpDown,    setMaxSpeedUpDown,    xmlelement, mode);

        XMLPortParam(Engine, "accelerationfront",     setAccelerationFront,     setAccelerationFront,     xmlelement, mode);
        XMLPortParam(Engine, "accelerationbrake",     setAccelerationBrake,     setAccelerationBrake,     xmlelement, mode);
        XMLPortParam(Engine, "accelerationback",      setAccelerationBack,      setAccelerationBack,      xmlelement, mode);
        XMLPortParam(Engine, "accelerationleftright", setAccelerationLeftRight, setAccelerationLeftRight, xmlelement, mode);
        XMLPortParam(Engine, "accelerationupdown",    setAccelerationUpDown,    setAccelerationUpDown,    xmlelement, mode);

        XMLPortParam(Engine, "position", setRelativePosition, getRelativePosition, xmlelement, mode);
        XMLPortParam(Engine, "template", setEngineTemplate, getEngineTemplate, xmlelement, mode);
    }

    void Engine::setConfigValues()
    {
    }

    void Engine::registerVariables()
    {
        registerVariable(this->shipID_, VariableDirection::ToClient, new NetworkCallback<Engine>(this, &Engine::networkcallback_shipID));

        registerVariable(this->boostFactor_, VariableDirection::ToClient);

        registerVariable(this->maxSpeedFront_,     VariableDirection::ToClient);
        registerVariable(this->maxSpeedBack_,      VariableDirection::ToClient);
        registerVariable(this->maxSpeedLeftRight_, VariableDirection::ToClient);
        registerVariable(this->maxSpeedUpDown_,    VariableDirection::ToClient);

        registerVariable(this->accelerationFront_,     VariableDirection::ToClient);
        registerVariable(this->accelerationBrake_,     VariableDirection::ToClient);
        registerVariable(this->accelerationBack_,      VariableDirection::ToClient);
        registerVariable(this->accelerationLeftRight_, VariableDirection::ToClient);
        registerVariable(this->accelerationUpDown_,    VariableDirection::ToClient);

        registerVariable(this->speedAdd_, VariableDirection::ToClient);
        registerVariable(this->speedMultiply_, VariableDirection::ToClient);
    }

    void Engine::networkcallback_shipID()
    {
        this->ship_ = 0;

        if (this->shipID_ != OBJECTID_UNKNOWN)
        {
            Synchronisable* object = Synchronisable::getSynchronisable(this->shipID_);
            if (object)
                this->addToSpaceShip(orxonox_cast<SpaceShip*>(object));
        }
    }

    /**
    @brief
        Run the engine for a given time interval.
        Is called each tick by SpaceShip.
    @param dt
        The time since last tick.
    */
    void Engine::run(float dt)
    {
        if (this->ship_ == NULL)
        {
            if (this->shipID_ != 0)
            {
                this->networkcallback_shipID();

                if (this->ship_ == NULL)
                    return;
            }
            else
                return;
        }

        if (!this->isActive())
            return;

        // Get the desired steering direction and amount, clipped to length 1 at maximum.
        Vector3 steering = (this->getSteering().length() > 1.0f ? this->getSteering().normalisedCopy() : this->getSteering());

        // Get the ships velocity.
        Vector3 velocity = this->ship_->getLocalVelocity();
        Vector3 acceleration = Vector3::ZERO;

        // If there is forward steering action.
        if (steering.z < 0)
        {
            if (this->maxSpeedFront_ != 0)
            {
                float boostfactor = (this->ship_->isBoosting() ? this->boostFactor_ : 1.0f); // Boost factor is 1.0 if not boosting.
                // Boosting can lead to velocities larger the maximal forward velocity.
                acceleration.z = steering.z * this->accelerationFront_ * boostfactor * clamp((this->maxSpeedFront_ - -velocity.z/boostfactor) / this->maxSpeedFront_, 0.0f, 1.0f);
            }
        }
        // If there is backward steering action.
        else if (steering.z > 0)
        {
            // Either breaking
            if (velocity.z < 0)
                acceleration.z = steering.z * this->accelerationBrake_;
            // or backward flight.
            else if (this->maxSpeedBack_ != 0)
                acceleration.z = steering.z * this->accelerationBack_ * clamp((this->maxSpeedBack_ - velocity.z) / this->maxSpeedBack_, 0.0f, 1.0f);
        }
        // If there is left-right steering action.
        if (this->maxSpeedLeftRight_ != 0)
        {
            if (steering.x < 0)
                acceleration.x = steering.x * this->accelerationLeftRight_ * clamp((this->maxSpeedLeftRight_ - -velocity.x) / this->maxSpeedLeftRight_, 0.0f, 1.0f);
            else if (steering.x > 0)
                acceleration.x = steering.x * this->accelerationLeftRight_ * clamp((this->maxSpeedLeftRight_ - velocity.x) / this->maxSpeedLeftRight_, 0.0f, 1.0f);
        }
        // If there is up-down steering action.
        if (this->maxSpeedUpDown_ != 0)
        {
            if (steering.y < 0)
                acceleration.y = steering.y * this->accelerationUpDown_ * clamp((this->maxSpeedUpDown_ - -velocity.y) / this->maxSpeedUpDown_, 0.0f, 1.0f);
            else if (steering.y > 0)
                acceleration.y = steering.y * this->accelerationUpDown_ * clamp((this->maxSpeedUpDown_ - velocity.y) / this->maxSpeedUpDown_, 0.0f, 1.0f);
        }

        // NOTE: Bullet always uses global coordinates.
        this->ship_->addAcceleration(this->ship_->getOrientation() * (acceleration*this->getSpeedMultiply()+Vector3(0,0,-this->getSpeedAdd())), this->ship_->getOrientation() * this->relativePosition_);
    }

    /**
    @brief
        Adds the Engine to the input SpaceShip.
    @param ship
        A pointer to the SpaceShip to which the engine is added.
    */
    void Engine::addToSpaceShip(SpaceShip* ship)
    {
        this->ship_ = ship;

        if (ship)
        {
            this->shipID_ = ship->getObjectID();
            if (!ship->hasEngine(this))
                ship->addEngine(this);
        }
    }

    /**
    @brief
        Get the direction and magnitude of the steering imposed upon the Engine.
    @return
        Returns the direction and magnitude of the steering imposed upon the Engine.
        Is the zero vector if the Engine doesn't belong to a ship.
    */
    const Vector3& Engine::getSteering() const
    {
        if (this->ship_)
            return this->ship_->getSteeringDirection();
        else
            return Vector3::ZERO;
    }

    /**
    @brief
        Load the engine template.
        Causes all parameters specified by the template to be applied to the Engine.
    */
    void Engine::loadEngineTemplate()
    {
        if(!this->engineTemplate_.empty())
        {
            orxout(verbose, context::templates) << "Loading an engine template: " << this->engineTemplate_ << endl;
            Template *temp = Template::getTemplate(this->engineTemplate_);
            if(temp)
            {
                this->addTemplate(temp);
            }
        }
    }

}
