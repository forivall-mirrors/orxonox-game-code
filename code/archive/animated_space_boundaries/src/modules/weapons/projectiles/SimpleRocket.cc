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
 *      simonmie
 *
 */

#include "SimpleRocket.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/pawns/Pawn.h"
#include "graphics/ParticleSpawner.h"
#include "graphics/Model.h"
#include "objects/collisionshapes/ConeCollisionShape.h"
#include "infos/PlayerInfo.h"
#include "controllers/Controller.h"
#include "weapons/RocketController.h"
#include "sound/WorldSound.h"
#include "util/Debug.h"

namespace orxonox
{

    CreateFactory(SimpleRocket);

    SimpleRocket::SimpleRocket(BaseObject* creator) : ControllableEntity(creator), BasicProjectile()
    {
        RegisterObject(SimpleRocket);// - register the SimpleRocket class to the core

        this->localAngularVelocity_ = 0;
        this->lifetime_ = 120;

        this->setMass(15);
//        COUT(4) << "simplerocket constructed\n";

        if (GameMode::isMaster())
        {
            this->setCollisionType(WorldEntity::Kinematic);
            this->fuel_=true;

            Model* model = new Model(this);
            model->setMeshSource("rocket.mesh");
            model->scale(0.7f);
            this->attach(model);

            this->fire_ = new ParticleEmitter(this);
            this->attach(this->fire_);

            this->fire_->setOrientation(this->getOrientation());
            this->fire_->setSource("Orxonox/simplerocketfire");
            this->enableCollisionCallback();
            this->setCollisionResponse(false);
            this->setCollisionType(Kinematic);

            // TODO: fix the orientation and size of this collision shape to match the rocket
            ConeCollisionShape* collisionShape = new ConeCollisionShape(this);
            collisionShape->setOrientation(this->getOrientation());
            collisionShape->setRadius(1.5f);
            collisionShape->setHeight(5);
            this->attachCollisionShape(collisionShape);
            this->destroyTimer_.setTimer(this->lifetime_, false, createExecutor(createFunctor(&SimpleRocket::destroyObject, this)));
        }

    }



    /**
    * @brief updates state of rocket, disables fire if no fuel
    * @param dt tick-length
    */
    void SimpleRocket::tick(float dt)
    {

        SUPER(SimpleRocket, tick, dt);
        if ( GameMode::isMaster() )
        {


            this->setAngularVelocity(this->getOrientation() * this->localAngularVelocity_);
            this->setVelocity( this->getOrientation()*WorldEntity::FRONT*this->getVelocity().length() );
            this->localAngularVelocity_ = 0;


            if (this->fuel_)
            {
                if (this->destroyTimer_.getRemainingTime()<  (static_cast<float>(this->FUEL_PERCENTAGE)/100) *this->lifetime_ )
                    this->fuel_=false;
            } else
                this->disableFire();

            if( this->getBDestroy() )
                this->destroy();
        }

    }

    /**
    * @brief Sets the Acceleration to 0 and detaches the fire
    * @return void
    */
    void SimpleRocket::disableFire()
    {
        this->setAcceleration(0,0,0);
        this->fire_->detachFromParent();
    }

    /**s
    @brief
        Destructor. Destroys controller, if present and kills sounds, if playing.
    */
    SimpleRocket::~SimpleRocket()
    {
        if (this->isInitialized())
        {
            if( GameMode::isMaster() )
            {
                this->getController()->destroy();
            }
        }
    }

    /**
    @brief
        Method for creating a SimpleRocket through XML.
    */
    void SimpleRocket::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        // this calls the XMLPort function of the parent class
        SUPER(SimpleRocket, XMLPort, xmlelement, mode);
    }

    void SimpleRocket::setOwner(Pawn* owner)
    {
        this->owner_ = owner;
        this->player_ = this->getOwner()->getPlayer();
    }


    /* Calls the collidesAgainst function of BasicProjectile
     */
    bool SimpleRocket::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        return BasicProjectile::basicCollidesAgainst(otherObject,contactPoint,this->getOwner(),this);
    }

    void SimpleRocket::destroyObject()
    {
        if (GameMode::isMaster())
        {
            this->destroy();
        }
    }

    /**
    @brief
        Rotates the SimpleRocket around the y-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void SimpleRocket::rotateYaw(const Vector2& value)
    {
        ControllableEntity::rotateYaw(value);

        if( !this->isInMouseLook() )
            this->localAngularVelocity_.y += value.x;
    }

    /**
    @brief
        Rotates the SimpleRocket around the x-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void SimpleRocket::rotatePitch(const Vector2& value)
    {
        ControllableEntity::rotatePitch(value);

        if( !this->isInMouseLook() )
            this->localAngularVelocity_.x += value.x;
    }

    /**
    @brief
        Rotates the SimpleRocket around the z-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void SimpleRocket::rotateRoll(const Vector2& value)
    {
        ControllableEntity::rotateRoll(value);

        if( !this->isInMouseLook() )
            this->localAngularVelocity_.z += value.x;
    }

}
