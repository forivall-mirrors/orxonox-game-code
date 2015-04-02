/*
 * GravityBombField.cc
 *
 *  Created on: Apr 2, 2015
 *      Author: meggiman
 */

#include "GravityBombField.h"

namespace orxonox{
	RegisterClass(GravityBombField);

	const float GravityBombField::FORCE_FIELD_LIFETIME = 10;
	const float GravityBombField::FORCE_SPHERE_START_RADIUS = 50;
	const float GravityBombField::FORCE_SPHERE_START_STRENGTH = -300;

	GravityBombField::GravityBombField(Context* context) : ForceField(context)
	{
		lifetime_=FORCE_FIELD_LIFETIME;
		setVelocity(FORCE_SPHERE_START_STRENGTH);
		setDiameter(FORCE_SPHERE_START_RADIUS);
		setMode(modeInvertedSphere_s);
	}

	GravityBombField::~GravityBombField(){}

	void GravityBombField::tick(float dt)
	{
		lifetime_-=dt;
		if(lifetime_ < 0)
		{
			this->destroy();
		}
	}

	void GravityBombField::destroy()
	{
		//Animation
		//SUPER(GravityBombField,destroy);
	}

}
