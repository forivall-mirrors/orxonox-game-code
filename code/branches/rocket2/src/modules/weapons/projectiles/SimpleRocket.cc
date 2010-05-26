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
    // create the factory for the SimpleRocket

    /**
    @brief
        Constructor. Registers the object and initializes some default values.
    */
    SimpleRocket::SimpleRocket(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(SimpleRocket);// - register the SimpleRocket class to the core

        this->localAngularVelocity_ = 0;
        this->bDestroy_ = false;
        this->lifetime_ = 100;
        this->setMass(15);
        COUT(4) << "simplerocket constructed\n";
        this->counter_=0;
        this->slowing_=false;

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
            collisionShape->setHeight(200);
            this->attachCollisionShape(collisionShape);
            

            this->destroyTimer_.setTimer(this->lifetime_, false, createExecutor(createFunctor(&SimpleRocket::destroyObject, this)));
        }

    }
   
    void SimpleRocket::disableFire(){
        this->setAcceleration(0,0,0);
        this->setVelocity(Vector3(0,0,0));
        
        this->fire_->detachFromParent();
        //this->fire_->setVisible(false);

    }


    void SimpleRocket::tick(float dt)
    {

        SUPER(SimpleRocket, tick, dt);
        counter_++;
        if (this->getVelocity().squaredLength() >130000 && !slowing_) counter_++; //if Velocity bigger than about 360, uses a lot more "fuel" :)

            this->setAngularVelocity(this->getOrientation() * this->localAngularVelocity_);
            this->setVelocity( this->getOrientation()*WorldEntity::FRONT*this->getVelocity().length() );
            this->localAngularVelocity_ = 0;

            
            if (this->fuel_) {
                COUT(0)<<this->getVelocity().length()<<endl;
                if (this->counter_>1000 && counter_%12==0) 
                    
                    if (!this->slowing_) {
                        this->setAcceleration(this->getOrientation()*Vector3(10,10,10));
                        this->slowing_=true;
                    }

                if (this->counter_ > 1800) 
                    this->fuel_=false;
            }
            if( this->bDestroy_ )
                this->destroy();
            if (!this->fuel_) 
                this->disableFire();
        
           
            
        
    }

    /**s
    @brief
        Destructor. Destroys controller, if present and kills sounds, if playing.
    */
    SimpleRocket::~SimpleRocket()
    {
        if (this->isInitialized()) 
        {
            this->getController()->destroy();
            COUT(4)<< "simplerocket destroyed\n";
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
        //this->originalControllableEntity_ = this->owner_->getPlayer()->getControllableEntity();
        this->player_ = this->owner_->getPlayer();
    }



    bool SimpleRocket::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
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
                    effect->setSource("Orxonox/explosion4");
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
//             if (this->owner_)
//                 dmg = this->owner_->getPickups().processModifiers(ModifierType::Damage, dmg, false);

            Pawn* victim = orxonox_cast<Pawn*>(otherObject);
            if (victim)
                victim->hit(this->owner_, contactPoint, dmg);
        }
        return false;
    }

    void SimpleRocket::destroyObject()
    {
        if (GameMode::isMaster())
        {
            this->destroy();
        }
    }
    
    void SimpleRocket::setDestroy()
    {
        this->bDestroy_=true;
        CCOUT(4)<<"trying to destroy";
    }

    void SimpleRocket::fired(unsigned int firemode)
    {
        if (this->owner_)
        {
            {
                ParticleSpawner* effect = new ParticleSpawner(this->owner_->getCreator());
                effect->setPosition(this->getPosition());
                effect->setOrientation(this->getOrientation());
                effect->setDestroyAfterLife(true);
                effect->setSource("Orxonox/explosion4");
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
