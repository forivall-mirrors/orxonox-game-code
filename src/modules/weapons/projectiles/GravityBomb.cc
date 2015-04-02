/*
 * GravityBomb.cc
 *
 *  Created on: Mar 26, 2015
 *      Author: meggiman
 */
#include "GravityBomb.h"


namespace orxonox{
	RegisterClass(GravityBomb);

	const float GravityBomb::INITIAL_VELOCITY = 20;
	const float GravityBomb::SLOW_DOWN_RATIO = 2;

	GravityBomb::GravityBomb(Context* context):
				BasicProjectile(),
				MovableEntity(context),
				RadarViewable(this,static_cast<WorldEntity*>(this))
		{
			RegisterObject(GravityBomb);

			this->setMass(15.0);
			if (GameMode::isMaster())
			{
				//Define movement of the bomb
				this->setVelocity(this->getOrientation()*WorldEntity::FRONT*INITIAL_VELOCITY);
				this->velocityAtLastTick_=INITIAL_VELOCITY;
				this->setAcceleration(this->getOrientation()*WorldEntity::BACK*SLOW_DOWN_RATIO);
				this->setCollisionType(WorldEntity::Dynamic);
				this->enableCollisionCallback();

				//Add Collision Shape
				SphereCollisionShape* collisionShape = new SphereCollisionShape(context);
				collisionShape->setRadius(5.0);
				this->attachCollisionShape(collisionShape);

				//Create Bomb Model


			}
		}

	GravityBomb::~GravityBomb(){}

	void GravityBomb::tick(float dt)
	{
			if(velocityAtLastTick_ < this->getVelocity().length())
			{
				setVelocity(Vector3::ZERO);
				setAcceleration(Vector3::ZERO);
				velocityAtLastTick_=0;
				detonate();
			}
			velocityAtLastTick_=getVelocity().length();
	}

	bool GravityBomb::collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint)
	{
		detonate();
		processCollision(otherObject, contactPoint,cs);
		return true;
	}

	void GravityBomb::detonate()
	{
		GravityBombField* field = new GravityBombField(this->getContext());
	}
}


