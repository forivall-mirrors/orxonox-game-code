/*
 * GravityBombField.cc
 *
 *  Created on: Apr 2, 2015
 *      Author: meggiman
 */

#include "GravityBombField.h"

namespace orxonox{
	RegisterClass(GravityBombField);

	const float GravityBombField::FORCE_FIELD_LIFETIME = 5;
	const float GravityBombField::FORCE_SPHERE_START_RADIUS = 100;
	const float GravityBombField::FORCE_SPHERE_START_STRENGTH = -1000;

	GravityBombField::GravityBombField(Context* context) : ForceField(context)
	{
		RegisterObject(GravityBombField);
		lifetime_=FORCE_FIELD_LIFETIME;
		setVelocity(FORCE_SPHERE_START_STRENGTH);
		setDiameter(FORCE_SPHERE_START_RADIUS);
		setMode(modeInvertedSphere_s);
	}

	GravityBombField::~GravityBombField(){}

	void GravityBombField::tick(float dt)
	{
		SUPER(GravityBombField,tick,dt);
		lifetime_-=dt;
		if(lifetime_ < 0)
		{
			this->destroy();
		}
	}

	void GravityBombField::destroy()
	{
		//Animation
		ForceField::destroy();
	}

}
