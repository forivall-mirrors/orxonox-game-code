/*
 * GravityBombField.cc
 *
 *  Created on: Apr 2, 2015
 *      Author: Manuel Eggimann
 */

#include "GravityBombField.h"
#include "graphics/Model.h"

namespace orxonox{
	RegisterClass(GravityBombField);

	//Change these constants to alter the behaviour of the field.
	
	const float GravityBombField::FORCE_FIELD_LIFETIME = 15;
	const float GravityBombField::FORCE_SPHERE_START_RADIUS = 250;
	const float GravityBombField::FORCE_SPHERE_START_STRENGTH = -700;
	const float GravityBombField::PEAK_EXPLOSION_FORCE = 5e4;
	const float GravityBombField::FORCE_FIELD_EXPLOSION_DAMMAGE = 100;
	const float GravityBombField::EXPLOSION_DURATION = 1;
	const float GravityBombField::EXPLOSION_RADIUS = 600;
	const float GravityBombField::PEAK_ANGULAR_VELOCITY = 20;
	const float GravityBombField::CENTRE_MODEL_END_SIZE = 1.5;

	GravityBombField::GravityBombField(Context* context) : ForceField(context),RadarViewable(this, static_cast<WorldEntity*>(this))
	{
		RegisterObject(GravityBombField);
		//Initialize variable with their initial values.
		lifetime_=FORCE_FIELD_LIFETIME;
		forceStrength_ = FORCE_SPHERE_START_STRENGTH;
		forceSphereRadius_ = FORCE_SPHERE_START_RADIUS;
		modelScaling_ = 1;
		fieldExploded_ = false;

		setVelocity(FORCE_SPHERE_START_STRENGTH);
		setDiameter(2*FORCE_SPHERE_START_RADIUS);
		setMode(modeSphere_s);
		setCollisionResponse(false);

		//Make the Field visible on Radar and minimap.
		this->setRadarObjectColour(ColourValue(1.0, 0.0, 0.2,1)); // Red
		this->setRadarObjectShape(RadarViewable::Dot);
		this->setRadarObjectScale(1.0f);
		

		//Attach Model
		Model* model = new Model(this->getContext());
		model->setMeshSource("GravityBomb.mesh"); //Demo Model from SimpleRocket
		model->scale(2.5f);
		bombModel_ = new MovableEntity(context);
		bombModel_->attach(model);
		this->attach(bombModel_);

		//Add a Backlight to the centre.
		centreLight_ = new Backlight(context);
		centreLight_->setColour(ColourValue(0.2,0.9,0.2,1));
		centreLight_->setScale(0.3);
		centreLight_->setTrailMaterial("Trail/backlighttrail");
		centreLight_->setMaterial("Examples/Flare");
		centreLight_->setLifetime(20);
		bombModel_->attach(centreLight_);

		//Let the Bomb Modell in the centre rotate in a random direction.
		Vector3 randomRotation;
		srand(time(NULL));
		randomRotation.x = rand();
		randomRotation.y = rand();
		randomRotation.y = rand();
		randomRotation.normalise();
		bombModel_->setAngularAcceleration(randomRotation*(PEAK_ANGULAR_VELOCITY/FORCE_FIELD_LIFETIME));

		//Add Collision Shape
		SphereCollisionShape* collisionShape = new SphereCollisionShape(context);
		collisionShape->setRadius(10.0);
		this->attachCollisionShape(collisionShape);

		//Add particle effect to visualize the force field.
		this->particleSphere_ = new ParticleEmitter(this->getContext());
		this->attach(this->particleSphere_);
		particleSphere_->setSource("Orxonox/GravityBombField");

		//Add a sound effect to the field.
		WorldSound* fieldSound = new WorldSound(context);
		fieldSound->setSource("sounds/GravityField.ogg");
		fieldSound->setLooping(true);
		fieldSound->setVolume(1.0);
		this->attach(fieldSound);
		fieldSound->play();
	}

	GravityBombField::~GravityBombField(){}

	
	void GravityBombField::tick(float dt)
	{
		SUPER(GravityBombField,tick,dt);
		lifetime_-=dt;

		if(lifetime_ > EXPLOSION_DURATION)//If field is still alive, make it smaller and stronger.
		{
			modelScaling_ += ((CENTRE_MODEL_END_SIZE-1) / FORCE_FIELD_LIFETIME)*dt;
			forceStrength_ *= (1+dt/10);
			forceSphereRadius_ = FORCE_SPHERE_START_RADIUS*(1-((FORCE_FIELD_LIFETIME-lifetime_)/FORCE_FIELD_LIFETIME)*((FORCE_FIELD_LIFETIME-lifetime_)/FORCE_FIELD_LIFETIME)*((FORCE_FIELD_LIFETIME-lifetime_)/FORCE_FIELD_LIFETIME));
		}
		else if(lifetime_ > 0)
		{
			if (!fieldExploded_) // Start the field explosion if it has not been started yet.
			{
				forceStrength_ = pow((EXPLOSION_DURATION + lifetime_),4)/EXPLOSION_DURATION * PEAK_EXPLOSION_FORCE;
				fieldExploded_ = true;

				//Add particle effect to visualize explosion
				explosionCross_ = new ParticleEmitter(this->getContext());
				explosionCross_->setSource("Orxonox/FieldExplosion");
				explosionCross_->setOrientation(rand(), rand(), rand(), rand());
				explosionCross_->setScale(0.7);
				this->attach(explosionCross_);

				//Add explosion sound effect.
				explosionSound_ = new WorldSound(getContext());
				explosionSound_->setSource("sounds/GravityFieldExplosion.ogg");
				explosionSound_->setVolume(1.0);
				explosionSound_->play();
			}

			//Check if any pawn is inside the shockwave and hit it with dammage proportional to the distance between explosion centre and pawn. Make sure, the same pawn is damaged only once.
			for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
			{
				Vector3 distanceVector = it->getWorldPosition()-this->getWorldPosition();
				 //orxout(debug_output) << "Found Pawn:" << it->getWorldPosition() << endl;
				if(distanceVector.length()< forceSphereRadius_)
				 {
					 //orxout(debug_output) << "Force sphere radius is: " << forceSphereRadius_ << " Distance to Pawn is: " << distanceVector.length();
					 if (std::find(victimsAlreadyDamaged_.begin(),victimsAlreadyDamaged_.end(),*it) == victimsAlreadyDamaged_.end())
					 {
						 //orxout(debug_output) << "Found Pawn to damage: " << it->getWorldPosition() << endl;
						 float damage = FORCE_FIELD_EXPLOSION_DAMMAGE*(1-distanceVector.length()/EXPLOSION_RADIUS);
						 //orxout(debug_output) << "Damage: " << damage << endl;
						 it->hit(shooter_, it->getWorldPosition(), NULL, damage, 0,0);
						 victimsAlreadyDamaged_.push_back(*it);
					 }
				 }
			}

			forceSphereRadius_ = EXPLOSION_RADIUS*(1-lifetime_/EXPLOSION_DURATION);
			explosionCross_->setScale(forceSphereRadius_/FORCE_SPHERE_START_RADIUS);
		}
		else if (lifetime_ > -6) //The field has to exist for 6 more seconds for the particles of the particle effect to vanish smoothly.
		{
			//Make the bomb model invisible, let the strength of the field be zero and remove all particle emitters so the particle effect will slowly vanish.
			bombModel_->setVisible(false);
			this->setRadarVisibility(false);
			forceStrength_ = 0;
			forceSphereRadius_ = 0.00001;
			particleSphere_->getParticleInterface()->removeAllEmitters();
			explosionCross_->getParticleInterface()->removeAllEmitters();
		}
		
		setDiameter(forceSphereRadius_*2);
		setVelocity(forceStrength_);
		if(lifetime_>0) particleSphere_->setScale(forceSphereRadius_/FORCE_SPHERE_START_RADIUS);
		bombModel_->setScale(modelScaling_);

		if (lifetime_ <= -4)
		{
			orxout(debug_output) << "Timeout. Destroying field." << endl;
			this->destroy();
		}
	}

	void GravityBombField::destroy()
	{
		ForceField::destroy();
	}

}
