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
	const float GravityBombField::PEAK_EXPLOSION_FORCE = 1e6;
	const float GravityBombField::FORCE_FIELD_EXPLOSION_DAMMAGE = 100;
	const float GravityBombField::EXPLOSION_DURATION = 1;
	const float GravityBombField::EXPLOSION_RADIUS = 400;
	const float GravityBombField::PEAK_ANGULAR_VELOCITY = 20;

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

		//Attach Model
		Model* model = new Model(this->getContext());
		model->setMeshSource("GravityBomb.mesh"); //Demo Model from SimpleRocket
		model->scale(3.0f);
		bombModel_ = new MovableEntity(context);
		bombModel_->attach(model);
		this->attach(bombModel_);

		Backlight* centreLight = new Backlight(context);
		centreLight->setColour(ColourValue(0.9,0.5,0.5,1));
		centreLight->setScale(1.0);
		centreLight->setTrailMaterial("Trail/backlighttrail");
		centreLight->setMaterial("Examples/Flare");
		centreLight->setLifetime(20);
		bombModel_->attach(centreLight);

		Vector3 randomRotation;
		srand(time(NULL));
		randomRotation.x = rand();
		randomRotation.y = rand();
		randomRotation.y = rand();
		randomRotation.normalise();
		bombModel_->setAngularAcceleration(randomRotation*(PEAK_ANGULAR_VELOCITY/FORCE_FIELD_LIFETIME));

		//Add Collision Shape
		SphereCollisionShape* collisionShape = new SphereCollisionShape(context);
		collisionShape->setRadius(3.0);
		this->attachCollisionShape(collisionShape);

		this->particleSphere_ = new ParticleEmitter(this->getContext());
		this->attach(this->particleSphere_);
		particleSphere_->setSource("Orxonox/GravityBombField");
	}

	GravityBombField::~GravityBombField(){}

	void GravityBombField::tick(float dt)
	{
		SUPER(GravityBombField,tick,dt);
		lifetime_-=dt;

		if(lifetime_ > EXPLOSION_DURATION)
		{
			forceStrength_ *= (1+dt/10);
			forceSphereRadius_ = FORCE_SPHERE_START_RADIUS*(1-((FORCE_FIELD_LIFETIME-lifetime_)/FORCE_FIELD_LIFETIME)*((FORCE_FIELD_LIFETIME-lifetime_)/FORCE_FIELD_LIFETIME)*((FORCE_FIELD_LIFETIME-lifetime_)/FORCE_FIELD_LIFETIME));
		}
		else if(lifetime_ > 0)
		{
			if (!fieldExploded_)
			{
				forceStrength_ = PEAK_EXPLOSION_FORCE;
				fieldExploded_ = true;

				explosionCross_ = new ParticleEmitter(this->getContext());
				this->attach(explosionCross_);
				explosionCross_->setSource("Orxonox/FieldExplosion");
			}

			for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
			{
				Vector3 distanceVector = it->getWorldPosition()-this->getWorldPosition();
				 orxout(debug_output) << "Found Pawn:" << it->getWorldPosition() << endl;
				if(distanceVector.length()< forceSphereRadius_)
				 {
					 orxout(debug_output) << "Force sphere radius is: " << forceSphereRadius_ << " Distance to Pawn is: " << distanceVector.length();
					 if (std::find(victimsAlreadyDamaged_.begin(),victimsAlreadyDamaged_.end(),*it) == victimsAlreadyDamaged_.end())
					 {
						 orxout(debug_output) << "Found Pawn to damage: " << it->getWorldPosition() << endl;
						 float damage = FORCE_FIELD_EXPLOSION_DAMMAGE*(1-distanceVector.length()/EXPLOSION_RADIUS);
						 orxout(debug_output) << "Damage: " << damage << endl;
						 it->hit(shooter_, it->getWorldPosition(), NULL, damage, 0,0);
//						 it->removeHealth(damage);
						 victimsAlreadyDamaged_.push_back(*it);
					 }
				 }
			}

			forceSphereRadius_ = EXPLOSION_RADIUS*(1-lifetime_/EXPLOSION_DURATION);
			explosionCross_->setScale(forceSphereRadius_/FORCE_SPHERE_START_RADIUS);
		}
		else if (lifetime_ > -4)
		{
			bombModel_->setVisible(false);
			this->setRadarVisibility(false);
			forceStrength_ = 0;
			forceSphereRadius_ = 0.00001;
			particleSphere_->getParticleInterface()->removeAllEmitters();
			explosionCross_->getParticleInterface()->removeAllEmitters();
		}

		setDiameter(forceSphereRadius_*2);
		setVelocity(forceStrength_);
		particleSphere_->setScale(forceSphereRadius_/FORCE_SPHERE_START_RADIUS);

		if (lifetime_ <= -4)
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
