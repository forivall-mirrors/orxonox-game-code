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

	const float GravityBombField::FORCE_FIELD_LIFETIME = 20;
	const float GravityBombField::FORCE_SPHERE_START_RADIUS = 250;
	const float GravityBombField::FORCE_SPHERE_START_STRENGTH = -500;

	GravityBombField::GravityBombField(Context* context) : ForceField(context),RadarViewable(this, static_cast<WorldEntity*>(this))
	{
		RegisterObject(GravityBombField);
		lifetime_=FORCE_FIELD_LIFETIME;
		forceStrength_ = FORCE_SPHERE_START_STRENGTH;
		forceSphereRadius_ = FORCE_SPHERE_START_RADIUS;
		fieldExploded_ = false;

		setVelocity(FORCE_SPHERE_START_STRENGTH);
		setDiameter(2*FORCE_SPHERE_START_RADIUS);
		setMode(modeSphere_s);
		setCollisionResponse(false);

		this->setRadarObjectColour(ColourValue(0.2, 0.2, 1.0,1)); // Blue
		this->setRadarObjectShape(RadarViewable::Dot);
		this->setRadarObjectScale(0.5f);

		//Attach Demo Model for debuging.
			Model* model = new Model(this->getContext());
			model->setMeshSource("rocket.mesh"); //Demo Model from SimpleRocket
			model->scale(0.7f);
			this->attach(model);

		this->particleSphere_ = new ParticleEmitter(this->getContext());
		this->attach(this->particleSphere_);

		particleSphere_->setSource("Orxonox/GravityBombField");
	}

	GravityBombField::~GravityBombField(){}

	void GravityBombField::tick(float dt)
	{
		SUPER(GravityBombField,tick,dt);
		lifetime_-=dt;
		forceStrength_ *= (1+dt/10);
		if(lifetime_ < 0.2 && !fieldExploded_)
		{
			forceStrength_ *= -2;
			forceSphereRadius_ *= 2;
			fieldExploded_ = true;
			orxout(debug_output) << "Field exploded. Inverting Force." << endl;
		}

		setDiameter(forceSphereRadius_*2);
		setVelocity(forceStrength_);

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
