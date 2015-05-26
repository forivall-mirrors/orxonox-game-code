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

	const float GravityBomb::LIFETIME = 5;  ///< The gravity bomb lifetime in seconds.

	GravityBomb::GravityBomb(Context* context):
				BasicProjectile(),
				MovableEntity(context),
				RadarViewable(this,static_cast<WorldEntity*>(this))
		{
			RegisterObject(GravityBomb);

			this->setMass(10.0);
			this->isDetonated_ = false;
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
			timeToLife_ -= dt;
			if(timeToLife_ < 0)
			{
				//orxout(debug_output) << "bomb has stoped moving" <<endl;
				setVelocity(Vector3::ZERO); //Stop the bomb.
				isDetonated_ = true;
			}
			else
			{
				//orxout(debug_output)<< "Time to live:" << timeToLife_ <<endl;
				destroyCheck(); //Every BasicProjectil has to call this method in each tick.
			}
			if(isDetonated_) detonate();
			else SUPER(GravityBomb, tick, dt);
	}

	bool GravityBomb::collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint)
	{
		if(otherObject != getShooter()) //Ensure that the bomb cannot collide with its shooter.
		{
			orxout(debug_output) << "collides" << endl;
			processCollision(otherObject, contactPoint,cs);
			isDetonated_ = true;
			return true;
		}
		else{
			//orxout(debug_output) << "collided with shooter. Has no effect..." << endl;
			return false;
		}
	}

	void GravityBomb::detonate()
	{
		//Create the GravityBombField and destroy the Projectil.
		GravityBombField* field = new GravityBombField(this->getContext());
		field->setShooter(this->getShooter());
		field->setPosition(getPosition());
		//orxout(debug_output) << "detonating. Creating GravityBombField." <<endl;
		//orxout(debug_output) << "Field is at Position: " << getPosition() << endl;
		this->destroy();
	}
}


