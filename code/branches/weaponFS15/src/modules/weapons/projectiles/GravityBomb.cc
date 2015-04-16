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

			this->setMass(15.0);
			if (GameMode::isMaster())
			{
				//Define CollisionType of the bomb
				this->velocityAtLastTick_= 1000;
				this->setCollisionResponse(false);
				this->setCollisionType(WorldEntity::Kinematic);
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
			velocityAtLastTick_=getVelocity().length();
			SUPER(GravityBomb,tick,dt);
			if(velocityAtLastTick_ < this->getVelocity().length())
			{
				orxout(debug_output) << "bomb has stoped moving" <<endl;
				setVelocity(Vector3::ZERO);
				setAcceleration(Vector3::ZERO);
				velocityAtLastTick_=0;
				detonate();
				orxout(debug_output) << "denoting" <<endl;
			}
			else
			{
				velocityAtLastTick_=getVelocity().length();
				orxout(debug_output)<< velocityAtLastTick_ <<endl;
				orxout(debug_output) << "acceleration" << getAcceleration().length() <<endl;
				destroyCheck();
			}
	}

	bool GravityBomb::collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint)
	{
		orxout(debug_output) << "collides" << endl;
		processCollision(otherObject, contactPoint,cs);
		//detonate();
		return true;
	}

	void GravityBomb::detonate()
	{
		GravityBombField* field = new GravityBombField(this->getContext());
		orxout(debug_output) << "denoting" <<endl;
		this->destroy();
	}
}


