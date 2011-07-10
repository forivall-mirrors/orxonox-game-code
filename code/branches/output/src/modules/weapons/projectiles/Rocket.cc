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

#include "Rocket.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/CameraPosition.h"
#include "worldentities/pawns/Pawn.h"
#include "graphics/ParticleSpawner.h"
#include "graphics/Model.h"
#include "objects/collisionshapes/ConeCollisionShape.h"
#include "infos/PlayerInfo.h"
#include "controllers/Controller.h"
#include "sound/WorldSound.h"
#include "Scene.h"

namespace orxonox
{
    CreateFactory(Rocket);
    // create the factory for the Rocket

    /**
    @brief
        Constructor. Registers the object and initializes some default values.
    */
    Rocket::Rocket(BaseObject* creator)
        : ControllableEntity(creator)
        , BasicProjectile()
        , RadarViewable(creator, static_cast<WorldEntity*>(this))
    {
        RegisterObject(Rocket);// - register the Rocket class to the core

        this->localAngularVelocity_ = 0;
        this->lifetime_ = 100;

        if (GameMode::isMaster())
        {
            this->setCollisionType(WorldEntity::Kinematic);
            this->setVelocity(0,0,-100);

            Model* model = new Model(this);
            model->setMeshSource("rocket.mesh");
            model->scale(0.7f);
            this->attach(model);
            ParticleEmitter* fire = new ParticleEmitter(this);
            this->attach(fire);
            fire->setOrientation(this->getOrientation());
            fire->setSource("Orxonox/rocketfire");

            this->enableCollisionCallback();
            this->setCollisionResponse(false);
            this->setCollisionType(Kinematic);

            ConeCollisionShape* collisionShape = new ConeCollisionShape(this);
            collisionShape->setRadius(3);
            collisionShape->setHeight(500);
            this->attachCollisionShape(collisionShape);

            this->destroyTimer_.setTimer(this->lifetime_, false, createExecutor(createFunctor(&Rocket::destroyObject, this)));

            this->defSndWpnEngine_ = new WorldSound(this);
            this->defSndWpnEngine_->setLooping(true);
            this->defSndWpnEngine_->setSource("sounds/Rocket_engine.ogg");
            this->defSndWpnEngine_->setVolume(1.0f);
            this->attach(defSndWpnEngine_);

            this->defSndWpnLaunch_ = new WorldSound(this);
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

        CameraPosition* camPosition = new CameraPosition(this);
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
    Rocket::~Rocket()
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
        this->player_ = this->getOwner()->getPlayer();
        this->getOwner()->getPlayer()->startTemporaryControl(this);

        if( GameMode::isMaster() )
        {
            this->defSndWpnEngine_->play();
            this->defSndWpnLaunch_->play();
        }
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

        if( this->hasLocalController() )
        {
            this->setAngularVelocity(this->getOrientation() * this->localAngularVelocity_);
            this->setVelocity( this->getOrientation()*WorldEntity::FRONT*this->getVelocity().length() );
            this->localAngularVelocity_ = 0;
        }

        if( GameMode::isMaster() )
        {
            if( this->getBDestroy() )
                this->destroy();

        }
    }

    /* Calls the collidesAgainst function of BasicProjectile
     */
    bool Rocket::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        return BasicProjectile::basicCollidesAgainst(otherObject,contactPoint,this->getOwner(),this);
    }

    void Rocket::destroyObject()
    {
        if (GameMode::isMaster())
        {
            if(this->defSndWpnEngine_->isPlaying())
            {
                this->defSndWpnEngine_->stop();
            }
            this->destroy();
        }
    }

    void Rocket::fired(unsigned int firemode)
    {
        this->destroy();
    }

    void Rocket::destructionEffect()
    {
        ParticleSpawner *effect1, *effect2;
        if( this->getOwner() )
        {
            effect1 = new ParticleSpawner(this->getOwner()->getCreator());
            effect2 = new ParticleSpawner(this->getOwner()->getCreator());
        }
        else
        {
            effect1 = new ParticleSpawner(static_cast<BaseObject*>(this->getScene().get()));
            effect2 = new ParticleSpawner(static_cast<BaseObject*>(this->getScene().get()));
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
        Rotates the Rocket around the y-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Rocket::rotateYaw(const Vector2& value)
    {
        ControllableEntity::rotateYaw(value);

        if( !this->isInMouseLook() )
            this->localAngularVelocity_.y += value.x;
    }

    /**
    @brief
        Rotates the Rocket around the x-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Rocket::rotatePitch(const Vector2& value)
    {
        ControllableEntity::rotatePitch(value);

        if( !this->isInMouseLook() )
            this->localAngularVelocity_.x += value.x;
    }

    /**
    @brief
        Rotates the Rocket around the z-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Rocket::rotateRoll(const Vector2& value)
    {
        ControllableEntity::rotateRoll(value);

        if( !this->isInMouseLook() )
            this->localAngularVelocity_.z += value.x;
    }

}
