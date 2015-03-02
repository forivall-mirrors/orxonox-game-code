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
 *      Gabriel Nadler
 *   Co-authors:
 *      simonmie
 *
 */

/**
    @file SimpleRocket.h
    @brief Implementation of the SimpleRocket class.
*/


#include "SimpleRocket.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "controllers/Controller.h"
#include "graphics/Model.h"
#include "graphics/ParticleSpawner.h"
#include "infos/PlayerInfo.h"
#include "objects/collisionshapes/ConeCollisionShape.h"
#include "worldentities/pawns/Pawn.h"
#include "sound/WorldSound.h"

#include "weapons/RocketController.h"

namespace orxonox
{
    RegisterClass(SimpleRocket);

    const float SimpleRocket::FUEL_PERCENTAGE = 0.8f;

    SimpleRocket::SimpleRocket(Context* context)
        : ControllableEntity(context)
        , BasicProjectile()
        , RadarViewable(this, static_cast<WorldEntity*>(this))
    {
        RegisterObject(SimpleRocket);// Register the SimpleRocket class to the core

        this->localAngularVelocity_ = 0;
        this->lifetime_ = 10.f;

        this->setMass(15.0);

        if (GameMode::isMaster())
        {
            this->setCollisionType(WorldEntity::Kinematic);
            this->fuel_ = true;

            // Create rocket model.
            Model* model = new Model(this->getContext());
            model->setMeshSource("rocket.mesh");
            model->scale(0.7f);
            this->attach(model);

            // Add effects.
            this->fire_ = new ParticleEmitter(this->getContext());
            this->attach(this->fire_);

            this->fire_->setOrientation(this->getOrientation());
            this->fire_->setSource("Orxonox/simplerocketfire");
            this->enableCollisionCallback();
            this->setCollisionResponse(false);
            this->setCollisionType(Kinematic);

            // Add collision shape.
            // TODO: fix the orientation and size of this collision shape to match the rocket
            ConeCollisionShape* collisionShape = new ConeCollisionShape(this->getContext());
            collisionShape->setOrientation(this->getOrientation());
            collisionShape->setRadius(1.5f);
            collisionShape->setHeight(5);
            this->attachCollisionShape(collisionShape);

            this->destroyTimer_.setTimer(this->lifetime_, false, createExecutor(createFunctor(&BasicProjectile::destroyObject, this)));
        }

        this->setRadarObjectColour(ColourValue(1.0, 1.0, 0.0)); // yellow
        this->setRadarObjectShape(RadarViewable::Triangle);
        this->setRadarObjectScale(0.5f);
    }



    /**
    @brief
        Updates state of rocket, disables fire if no fuel
    @param dt
        tick-length
    */
    void SimpleRocket::tick(float dt)
    {
        SUPER(SimpleRocket, tick, dt);

        if (GameMode::isMaster())
        {
            this->setAngularVelocity(this->getOrientation() * this->localAngularVelocity_);
            this->setVelocity( this->getOrientation()*WorldEntity::FRONT*this->getVelocity().length() );
            this->localAngularVelocity_ = 0;

            if (this->fuel_)
            {
                if (this->destroyTimer_.getRemainingTime() < this->FUEL_PERCENTAGE*this->lifetime_ )
                    this->fuel_ = false;
            } else
                this->disableFire();
        }

        this->destroyCheck();
    }

    /**
    @brief
        Sets the Acceleration to 0 and detaches the fire.
    */
    void SimpleRocket::disableFire()
    {
        this->setAcceleration(0,0,0);
        this->fire_->detachFromParent();
    }

    /**
    @brief
        Destructor. Destroys controller, if present and kills sounds, if playing.
    */
    SimpleRocket::~SimpleRocket()
    {
        if (this->isInitialized())
        {
            if( GameMode::isMaster() )
            {
                if (this->getController())
                    this->getController()->destroy();
            }
        }
    }

    /**
    @brief
        Set the entity that fired the SimpleRocket.
    @param shooter
        A pointer to the Pawn that fired the SimpleRocket.
    */
    void SimpleRocket::setShooter(Pawn* shooter)
    {
        BasicProjectile::setShooter(shooter);

        this->player_ = this->getShooter()->getPlayer();
    }

    bool SimpleRocket::collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint)
    {
        return this->processCollision(otherObject, contactPoint, cs);
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
