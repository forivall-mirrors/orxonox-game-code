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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

#include "Rocket.h"

#include "core/XMLPort.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "worldentities/pawns/Pawn.h"
#include "graphics/ParticleSpawner.h"
#include "graphics/Model.h"
#include "objects/collisionshapes/ConeCollisionShape.h"
#include "infos/PlayerInfo.h"
#include "controllers/Controller.h"
#include "worldentities/CameraPosition.h"

namespace orxonox
{
	CreateFactory(Rocket);    // put your code in here:
    // create the factory for the Rocket

    /**
    @brief
        Constructor. Registers the object and initializes some default values.
    */
    Rocket::Rocket(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(Rocket);// - register the Rocket class to the core
        
        this->setCollisionType(WorldEntity::Kinematic);
        this->setVelocity(0,0,-100);
        this->model_ = new Model(this);
        this->model_->setMeshSource("can.mesh");
        this->attach(this->model_);
        this->lifetime_ = 100;
        
        if (GameMode::isMaster())
        {
            this->enableCollisionCallback();
            this->setCollisionResponse(false);
            this->setCollisionType(Kinematic);

            this->collisionShape_ = new ConeCollisionShape(this);
            this->collisionShape_->setRadius(3);
            this->collisionShape_->setHeight(500);
            this->attachCollisionShape(this->collisionShape_);

            this->destroyTimer_.setTimer(this->lifetime_, false, createExecutor(createFunctor(&Rocket::destroyObject, this)));
        }
        
        this->camPosition_ = new CameraPosition(this);
        this->camPosition_->setPosition(0,0,0);
        this->attach( this->camPosition_ );
        this->addCameraPosition( this->camPosition_ );
    }

    /**
    @brief
        Destructor. Destroys controller, if present.
    */
    Rocket::~Rocket()
    {
        if(this->isInitialized())
        {
            this->collisionShape_->destroy();
            this->model_->destroy();
            
            if (GameMode::isMaster() && this->owner_)
                this->owner_->getPlayer()->startControl(this->originalControllableEntity_);
            this->camPosition_->destroy();
        }
    }

    /**
    @brief
        Method for creating a Rocket through XML.
    */
    void Rocket::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        // this calls the XMLPort function of the parent class
        SUPER(Rocket, XMLPort, xmlelement, mode);
    }
    
    void Rocket::setOwner(Pawn* owner)
    {
        this->owner_ = owner;
            
        this->originalControllableEntity_ = this->owner_->getPlayer()->getControllableEntity();
        this->originalControllableEntity_->setDestroyWhenPlayerLeft( false );
        this->owner_->getPlayer()->startControl(this);
    }

    /**
    @brief
        Defines which actions the Rocket has to take in each tick.
    @param dt
        The length of the tick.
    */
    void Rocket::tick(float dt)
    {
        SUPER(Rocket, tick, dt);
        
        this->setAngularVelocity(this->localAngularVelocity_);
        this->setVelocity( (this->getOrientation()*WorldEntity::FRONT)*100 );
        this->localAngularVelocity_ = 0;
//         this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() * getMass() * this->auxilaryThrust_);
//         this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() * getMass() * this->auxilaryThrust_);
//         if (this->localLinearAcceleration_.z() > 0)
//             this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() * getMass() * this->auxilaryThrust_);
//         else
//             this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() * getMass() * this->primaryThrust_);
//         this->physicalBody_->applyCentralForce(physicalBody_->getWorldTransform().getBasis() * this->localLinearAcceleration_);
//         this->localLinearAcceleration_.setValue(0, 0, 0);
//     
//         this->localAngularAcceleration_ *= this->getLocalInertia() * this->rotationThrust_;
//         this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * this->localAngularAcceleration_);
    }
    
    bool Rocket::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        if (!this->bDestroy_ && GameMode::isMaster())
        {
            if (otherObject == this->owner_)
                return false;

            this->bDestroy_ = true;

            if (this->owner_)
            {
                {
                    ParticleSpawner* effect = new ParticleSpawner(this->owner_->getCreator());
                    effect->setPosition(this->getPosition());
                    effect->setOrientation(this->getOrientation());
                    effect->setDestroyAfterLife(true);
                    effect->setSource("Orxonox/explosion3");
                    effect->setLifetime(2.0f);
                }
                {
                    ParticleSpawner* effect = new ParticleSpawner(this->owner_->getCreator());
                    effect->setPosition(this->getPosition());
                    effect->setOrientation(this->getOrientation());
                    effect->setDestroyAfterLife(true);
                    effect->setSource("Orxonox/smoke4");
                    effect->setLifetime(3.0f);
                }
            }

            float dmg = this->damage_;
            if (this->owner_)
                dmg = this->owner_->getPickups().processModifiers(ModifierType::Damage, dmg, false);

            Pawn* victim = orxonox_cast<Pawn*>(otherObject);
            if (victim)
                victim->damage(dmg, this->owner_);
        }
        return false;
    }
    
    void Rocket::destroyObject()
    {
        if (GameMode::isMaster())
            this->destroy();
    }

    /**
    @brief
        Rotates the Rocket around the y-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Rocket::rotateYaw(const Vector2& value)
    {
        this->localAngularVelocity_.x = value.x;
    }

    /**
    @brief
        Rotates the Rocket around the x-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Rocket::rotatePitch(const Vector2& value)
    {
        this->localAngularVelocity_.y = value.x;
    }

    /**
    @brief
        Rotates the Rocket around the z-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Rocket::rotateRoll(const Vector2& value)
    {
        this->localAngularVelocity_.z = value.x;
    }
    
}
