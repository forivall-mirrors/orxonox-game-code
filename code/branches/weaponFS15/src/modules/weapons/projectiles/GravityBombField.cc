/*
 * GravityBombField.cc
 *
 *  Created on: Apr 2, 2015
 *      Author: meggiman
 */

#include "GravityBombField.h"
#include "graphics/Model.h"

namespace orxonox{
	RegisterClass(GravityBombField);

	const float GravityBombField::FORCE_FIELD_LIFETIME = 100;
	const float GravityBombField::FORCE_SPHERE_START_RADIUS = 500;
	const float GravityBombField::FORCE_SPHERE_START_STRENGTH = -500;

	GravityBombField::GravityBombField(Context* context) : ForceField(context)
	{
		RegisterObject(GravityBombField);
		lifetime_=FORCE_FIELD_LIFETIME;
		setVelocity(FORCE_SPHERE_START_STRENGTH);
		setDiameter(FORCE_SPHERE_START_RADIUS);
		setMode(modeInvertedSphere_s);
		setCollisionResponse(false);

		//Attach Demo Model for debuging.
			Model* model = new Model(this->getContext());
			model->setMeshSource("rocket.mesh"); //Demo Model from SimpleRocket
			model->scale(0.7f);
			this->attach(model);
	}

	GravityBombField::~GravityBombField(){}

	void GravityBombField::tick(float dt)
	{
		SUPER(GravityBombField,tick,dt);
		lifetime_-=dt;
		if(lifetime_ < 0)
		{
			orxout(debug_output) << "Timeout. Destroying field." << endl;
			this->destroy();
		}
	}

	void GravityBombField::destroy()
	{
		//Animation
		ForceField::destroy();
	}

}
