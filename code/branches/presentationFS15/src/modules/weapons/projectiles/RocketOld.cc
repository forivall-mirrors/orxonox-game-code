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

/**
    @file RocketOld.h
    @brief Implementation of the RocketOld class.
*/

#include "RocketOld.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Scene.h"
#include "controllers/Controller.h"
#include "graphics/Model.h"
#include "graphics/ParticleSpawner.h"
#include "infos/PlayerInfo.h"
#include "objects/collisionshapes/ConeCollisionShape.h"
#include "sound/WorldSound.h"
#include "worldentities/CameraPosition.h"
#include "worldentities/pawns/Pawn.h"
//#include "particleuniverse/include/ParticleUniverseSystemManager.h"

namespace orxonox
{
    RegisterClass(RocketOld);

    /**
    @brief
        Constructor. Registers the object and initializes some default values.
    */
    RocketOld::RocketOld(Context* context)
        : ControllableEntity(context)
        , BasicProjectile()
        , RadarViewable(this, static_cast<WorldEntity*>(this))
    {
        RegisterObject(RocketOld);// Register the RocketOld class to the core

        this->localAngularVelocity_ = 0;
        this->lifetime_ = 100.0f;

        if (GameMode::isMaster())
        {
            this->setCollisionType(WorldEntity::Kinematic);
            this->setVelocity(0,0,-100);

            // Create RocketOld model
            Model* model = new Model(this->getContext());
            model->setMeshSource("rocket.mesh");
            model->scale(0.7f);
            this->attach(model);

            // Add effects.
            ParticleEmitter* fire = new ParticleEmitter(this->getContext());
            this->attach(fire);
            fire->setOrientation(this->getOrientation());
            fire->setSource("Orxonox/rocketfire");
            
            // Add Particle Universe Effects
    		//ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
    		//ParticleUniverse::ParticleSystem* pSys1 = pManager->createParticleSystem("pSys1", "bubbles", this->getScene()->getSceneManager());
    		//this->attachOgreObject(pSys1);

            this->enableCollisionCallback();
            this->setCollisionResponse(false);
            this->setCollisionType(Kinematic);

            // Add collision shape
            ConeCollisionShape* collisionShape = new ConeCollisionShape(this->getContext());
            collisionShape->setRadius(3);
            collisionShape->setHeight(500);
            this->attachCollisionShape(collisionShape);

            this->destroyTimer_.setTimer(this->lifetime_, false, createExecutor(createFunctor(&BasicProjectile::destroyObject, this)));

            // Add sound
            this->defSndWpnEngine_ = new WorldSound(this->getContext());
            this->defSndWpnEngine_->setLooping(true);
            this->defSndWpnEngine_->setSource("sounds/Rocket_engine.ogg");
            this->defSndWpnEngine_->setVolume(1.0f);
            this->attach(defSndWpnEngine_);

            this->defSndWpnLaunch_ = new WorldSound(this->getContext());
            this->defSndWpnLaunch_->setLooping(false);
            this->defSndWpnLaunch_->setSource("sounds/Rocket_launch.ogg");
            this->defSndWpnLaunch_->setVolume(1.0f);
            this->attach(defSndWpnLaunch_);
        }
        else
        {
            this->defSndWpnEngine_ = 0;
            this->defSndWpnLaunch_ = 0;
        }

        // Add camera
        CameraPosition* camPosition = new CameraPosition(this->getContext());
        camPosition->setPosition(0,4,15);
        camPosition->setAllowMouseLook(true);
        this->addCameraPosition(camPosition);

        this->setRadarObjectColour(ColourValue(1.0, 0.5, 0.0)); // orange
        this->setRadarObjectShape(RadarViewable::Triangle);
        this->setRadarObjectScale(0.5f);
    }

    /**
    @brief
        Destructor. Destroys controller, if present and kills sounds, if playing.
    */
    RocketOld::~RocketOld()
    {
        if(this->isInitialized())
        {
            if (GameMode::isMaster())
            {
                this->destructionEffect();

                if (this->getPlayer() && this->getController())
                    this->player_->stopTemporaryControl();
            }

            if ( this->defSndWpnEngine_ )
                this->defSndWpnEngine_->destroy();

            if ( this->defSndWpnLaunch_ )
                this->defSndWpnLaunch_->destroy();
        }
    }

    /**
    @brief
        Sets the entity that fired the RocketOld.
    @param shooter
        A pointer to the Pawn that fired the RocketOld.
    */
    void RocketOld::setShooter(Pawn* shooter)
    {
        this->BasicProjectile::setShooter(shooter);
        
        this->player_ = this->getShooter()->getPlayer();
        if(this->player_)
            this->player_->startTemporaryControl(this);

        if( GameMode::isMaster() )
        {
            this->defSndWpnEngine_->play();
            this->defSndWpnLaunch_->play();
        }
    }

    /**
    @brief
        Defines which actions the RocketOld has to take in each tick.
    @param dt
        The length of the tick.
    */
    void RocketOld::tick(float dt)
    {
        SUPER(RocketOld, tick, dt);

        if( this->hasLocalController() )
        {
            this->setAngularVelocity(this->getOrientation() * this->localAngularVelocity_);
            this->setVelocity( this->getOrientation()*WorldEntity::FRONT*this->getVelocity().length() );
            this->localAngularVelocity_ = 0;
        }

       this->destroyCheck();
    }

    bool RocketOld::collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint)
    {
        return this->processCollision(otherObject, contactPoint, cs);
    }

    /**
    @brief
        Destroys the RocketOld and stops the sound,
    */
    void RocketOld::destroyObject(void)
    {
        if (GameMode::isMaster() && this->defSndWpnEngine_->isPlaying())
            this->defSndWpnEngine_->stop();

        this->BasicProjectile::destroyObject();
    }

    /**
    @brief
        Destroys the RocketOld upon pressing "fire".
    */
    void RocketOld::fired(unsigned int firemode)
    {
        this->destroyObject();
    }

    /**
    @brief
        The effects that are displayed, when the RocketOld is destroyed.
    */
    void RocketOld::destructionEffect()
    {
        ParticleSpawner *effect1, *effect2;
        if(this->getShooter())
        {
            effect1 = new ParticleSpawner(this->getShooter()->getContext());
            effect2 = new ParticleSpawner(this->getShooter()->getContext());
        }
        else
        {
            effect1 = new ParticleSpawner(this->getContext());
            effect2 = new ParticleSpawner(this->getContext());
        }

        effect1->setPosition(this->getPosition());
        effect1->setOrientation(this->getOrientation());
        effect1->setDestroyAfterLife(true);
        effect1->setSource("Orxonox/explosion4");
        effect1->setLifetime(2.0f);

        effect2->setPosition(this->getPosition());
        effect2->setOrientation(this->getOrientation());
        effect2->setDestroyAfterLife(true);
        effect2->setSource("Orxonox/smoke4");
        effect2->setLifetime(3.0f);
    }

    /**
    @brief
        Rotates the RocketOld around the y-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void RocketOld::rotateYaw(const Vector2& value)
    {
        ControllableEntity::rotateYaw(value);

        if( !this->isInMouseLook() )
            this->localAngularVelocity_.y += value.x;
    }

    /**
    @brief
        Rotates the RocketOld around the x-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void RocketOld::rotatePitch(const Vector2& value)
    {
        ControllableEntity::rotatePitch(value);

        if( !this->isInMouseLook() )
            this->localAngularVelocity_.x += value.x;
    }

    /**
    @brief
        Rotates the RocketOld around the z-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void RocketOld::rotateRoll(const Vector2& value)
    {
        ControllableEntity::rotateRoll(value);

        if( !this->isInMouseLook() )
            this->localAngularVelocity_.z += value.x;
    }

}
