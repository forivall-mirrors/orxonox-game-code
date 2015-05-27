/*
 * GravityBomb.cc
 *
 *  Created on: Mar 26, 2015
 *      Author: Manuel Eggimann
 */
#include "GravityBomb.h"
#include "graphics/Model.h"


namespace orxonox{
	RegisterClass(GravityBomb);

	const float GravityBomb::LIFETIME = 2.5;  ///< The gravity bomb lifetime in seconds.

	GravityBomb::GravityBomb(Context* context):
				BasicProjectile(),
				MovableEntity(context),
				RadarViewable(this,static_cast<WorldEntity*>(this))
		{
			RegisterObject(GravityBomb);

			this->setMass(10.0);
			this->hasCollided_ = false;
			if (GameMode::isMaster())
			{
				//Define CollisionType of the bomb
				this->timeToLife_= LIFETIME;
				this->setCollisionResponse(false);
				this->setCollisionType(WorldEntity::Dynamic);
				this->enableCollisionCallback();

				//Add Collision Shape
				SphereCollisionShape* collisionShape = new SphereCollisionShape(context);
				collisionShape->setRadius(1.0);
				this->attachCollisionShape(collisionShape);

				//Create Bomb Model
	            Model* rocketModel = new Model(this->getContext());
	            rocketModel->setMeshSource("GravityBombRocket.mesh"); //Demo Model from SimpleRocket
	            rocketModel->scale(3.0f);
	            this->attach(rocketModel);
				//Add second model because the bomb consists of the bomb and attached rockets (2 separate models)
	            Model* bombModel =  new Model(this->getContext());
	            bombModel->setMeshSource("GravityBomb.mesh"); //Demo Model from SimpleRocket
				bombModel->scale(3.0f);
				this->attach(bombModel);

				//Add particle effect to the flying rockets.
				ParticleEmitter* fire = new ParticleEmitter(this->getContext());
				fire->setOrientation(this->getOrientation());
				fire->setSource("Orxonox/simplerocketfire");
				this->attach(fire);

				//Add sound effect while the bomb is flying.
				WorldSound* bombSound = new WorldSound(context);
				bombSound->setSource("sounds/GravityBombFlight.ogg");
				bombSound->setLooping(true);
				bombSound->setVolume(1.0);
				this->attach(bombSound);
				bombSound->play();
			}
		}

	GravityBomb::~GravityBomb(){}

	void GravityBomb::tick(float dt)
	{
		SUPER(GravityBomb, tick, dt);
		timeToLife_ -= dt;
		if (timeToLife_ < 0)
		{
			//orxout(debug_output) << "bomb has stoped moving" <<endl;
			setVelocity(Vector3::ZERO); //Stop the bomb.
			detonate();
			this->destroy();
		}
		else 
		{
			if (hasCollided_) detonate();
			destroyCheck(); //Bomb is going to be destroyed by destroyCheck(). As written in BasicProectile, this Method should be called by every Projectile.
		}
	}

	bool GravityBomb::collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint)
	{
		hasCollided_ = processCollision(otherObject, contactPoint, cs);
		return hasCollided_;
	}

	void GravityBomb::detonate()
	{
		//Create the GravityBombField and destroy the Projectil.
		GravityBombField* field = new GravityBombField(this->getContext());
		field->setShooter(this->getShooter());
		field->setPosition(getPosition());
		//orxout(debug_output) << "detonating. Creating GravityBombField." <<endl;
		//orxout(debug_output) << "Field is at Position: " << getPosition() << endl;
	}
}


