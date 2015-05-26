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
#include <math.h>
#include "graphics/Backlight.h"
#include "sound/WorldSound.h"

namespace orxonox {

/**
 * @class	GravityBombField
 *
 * @brief	This class is used by GravityBomb to place the ForceField and Visual effect to the environment.
 * 			The field has a maximum lifetime and gets smaller and stronger the more time passes. In the end, the field explodes and damages all pawns nearby.
 *
 * @author	Manuel Eggimann
 * @date	23.05.2015
 */
class GravityBombField: public ForceField, public RadarViewable {
public:
	GravityBombField(Context* context);
	virtual ~GravityBombField();
	virtual void tick(float dt);
	virtual void destroy();

	/**
	 * @fn	void GravityBombField::setShooter(Pawn* shooter)
	 *
	 * @brief	This function is used to determine save the pawn who created the field and is used inside the GravityBomb class.
	 *
	 * @author	Manuel Eggimann
	 * @date	23.05.2015
	 *
	 * @param [in,out]	the Pawn that created the field.
	 */
	void setShooter(Pawn* shooter)
	{ this->shooter_ = shooter; }

	Pawn* getShooter()
	{ return this->shooter_; }

private:
	//Set these constants inside GravityBombField.cc to alter the behaviour of the field.
	
	static const float FORCE_FIELD_LIFETIME;	///< The lifetime of the ForceField in seconds. After lifetime seconds, has already exploded and the particle effects will start to vanish. 
	static const float FORCE_SPHERE_START_RADIUS;   ///< The initial sphere radius of the Force Field. The forcefield gets smaller by time.
	static const float FORCE_SPHERE_START_STRENGTH; ///< The initial Force the Field exerts on every object with non-zero mass.
	static const float FORCE_FIELD_EXPLOSION_DAMMAGE;   ///< The maximum dammage a pawn gets nearby an exploding field. The farer away from explosion center the smaller the dammage.
	static const float EXPLOSION_DURATION;  ///< Determines how fast the shockwave of the Explosion expands. It takes GravityBombField::EXPLOSION_DURATION seconds for the field to expand from 0 radius to GravityBombField::EXPLOSION_RADIUS.
	static const float EXPLOSION_RADIUS;	///< How far does the shockwave reach. All pawns which outside of a sphere witch this radius rest unharmed by the explosion.
	static const float PEAK_ANGULAR_VELOCITY;   ///< The model of the bomb in the center of the Field does rotate faster and faster as time passes until it reaches PEAK_ANGULAR_VELOCITY at the fields end of life.
	static const float PEAK_EXPLOSION_FORCE;	///< The peak force the explosion exerts on the pawns nearby.
	static const float CENTRE_MODEL_END_SIZE;   ///< Size of the 3d-model of the bomb in the fields centre.

	float forceSphereRadius_;
	float forceStrength_;
	float lifetime_;
	float modelScaling_;
	Vector3 rotationVector_;
	bool fieldExploded_;
	ParticleEmitter * particleSphere_;
	ParticleEmitter * explosionCross_;
	std::vector<Pawn*> victimsAlreadyDamaged_;
	MovableEntity * bombModel_;
	Pawn* shooter_;
	Backlight* centreLight_;
	WorldSound* explosionSound_;
};

}
#endif /* GRAVITYBOMBFIELD_H_ */

