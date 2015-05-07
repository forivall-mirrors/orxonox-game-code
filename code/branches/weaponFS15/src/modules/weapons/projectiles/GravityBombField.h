/*
 * GravityBombField.h
 *
 *  Created on: Apr 2, 2015
 *      Author: meggiman
 */

#ifndef GRAVITYBOMBFIELD_H_
#define GRAVITYBOMBFIELD_H_

#include "graphics/ParticleSpawner.h"
#include "interfaces/RadarViewable.h"
#include "objects/ForceField.h"
#include "BasicProjectile.h"
#include "worldentities/MovableEntity.h"
#include "core/CoreIncludes.h"
#include "GravityBomb.h"
#include "graphics/ParticleSpawner.h"
#include "tools/ParticleInterface.h"
#include <stdlib.h>
#include <time.h>
#include "graphics/Backlight.h"

namespace orxonox {
class GravityBombField: public ForceField, public RadarViewable {
public:
	GravityBombField(Context* context);
	virtual ~GravityBombField();
	virtual void tick(float dt);
	virtual void destroy();

	void setShooter(Pawn* shooter)
	{ this->shooter_ = shooter; }

	Pawn* getShooter()
	{ return this->shooter_; }

private:
	static const float FORCE_FIELD_LIFETIME;
	static const float FORCE_SPHERE_START_RADIUS;
	static const float FORCE_SPHERE_START_STRENGTH;
	static const float FORCE_FIELD_EXPLOSION_DAMMAGE;
	static const float EXPLOSION_DURATION;
	static const float EXPLOSION_RADIUS;
	static const float PEAK_ANGULAR_VELOCITY;
	static const float PEAK_EXPLOSION_FORCE;

	float forceSphereRadius_;
	float forceStrength_;
	float lifetime_;
	Vector3 rotationVector_;
	bool fieldExploded_;
	ParticleEmitter * particleSphere_;
	ParticleEmitter * explosionCross_;
	std::vector<Pawn*> victimsAlreadyDamaged_;
	MovableEntity * bombModel_;
	Pawn* shooter_;
};

}
#endif /* GRAVITYBOMBFIELD_H_ */

