/*
 * GravityBomb.cc
 *
 *  Created on: Mar 26, 2015
 *      Author: meggiman
 */
#include "GravityBomb.h"


namespace orxonox{
	RegisterClass(GravityBomb);

	const float GravityBomb::FUEL_START = 10;
	const float GravityBomb::FORCE_SPHERE_START_RADIUS = 30;
	const float GravityBomb::FORCE_SPHERE_START_STRENGTH = 100;

	GravityBomb::GravityBomb(Context* context):
			BasicProjectile(),
			MovableEntity(context),
			RadarViewable(this,static_cast<WorldEntity*>(this))
		{
			RegisterObject(GravityBomb);
			this->lifetime_=FUEL_START;
			this->forceSphereRadius_= FORCE_SPHERE_START_RADIUS;
			this->forceStrength_ = FORCE_SPHERE_START_STRENGTH;

			ForceField* field = new ForceField(context);
			field->setMode("sphere");
			field->setDiameter(this->forceSphereRadius_);
			field->setVelocity(this->forceStrength_);
			this->attach(field);

		}

	GravityBomb::~GravityBomb(){}

	void GravityBomb::tick(float dt)
	{

	}

	bool GravityBomb::collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint)
	{
		return true;
	}
}


