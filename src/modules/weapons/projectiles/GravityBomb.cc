/*
 * GravityBomb.cc
 *
 *  Created on: Mar 26, 2015
 *      Author: meggiman
 */
#include "GravityBomb.h"
#include "graphics/Model.h"


namespace orxonox{
	RegisterClass(GravityBomb);

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
				this->timeToLife_= 5;
				this->setCollisionResponse(false);
				this->setCollisionType(WorldEntity::Dynamic);
				this->enableCollisionCallback();

				//Add Collision Shape
				SphereCollisionShape* collisionShape = new SphereCollisionShape(context);
				collisionShape->setRadius(1.0);
				this->attachCollisionShape(collisionShape);

				//Create Bomb Model
	            Model* model = new Model(this->getContext());
	            model->setMeshSource("rocket.mesh"); //Demo Model from SimpleRocket
	            model->scale(0.7f);
	            this->attach(model);

			}
		}

	GravityBomb::~GravityBomb(){}

	void GravityBomb::tick(float dt)
	{
			SUPER(GravityBomb,tick,dt);
			timeToLife_ -= dt;
			if(timeToLife_ < 0)
			{
				orxout(debug_output) << "bomb has stoped moving" <<endl;
				setVelocity(Vector3::ZERO);
				setAcceleration(Vector3::ZERO);
				detonate();
			}
			else
			{
				orxout(debug_output)<< "Time to live:" << timeToLife_ <<endl;
				destroyCheck();
			}
			if(isDetonated_) detonate();
	}

	bool GravityBomb::collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint)
	{
		if(otherObject != getShooter())
		{
			orxout(debug_output) << "collides" << endl;
			processCollision(otherObject, contactPoint,cs);
			isDetonated_ = true;
			return true;
		}
		else{
			orxout(debug_output) << "collided with shooter. Has no effect..." << endl;
			return false;
		}
	}

	void GravityBomb::detonate()
	{
		GravityBombField* field = new GravityBombField(this->getContext());
		field->setPosition(getPosition());
		orxout(debug_output) << "detonating. Creating GravityBombField." <<endl;
		orxout(debug_output) << "Field is at Position: " << getPosition() << endl;
		this->destroy();
	}
}


