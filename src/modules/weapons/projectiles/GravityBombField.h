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

namespace orxonox {
class GravityBombField: public ForceField {
public:
	GravityBombField(Context* context);
	virtual ~GravityBombField();
	virtual void tick(float dt);
	virtual void destroy();
private:
	static const float FORCE_FIELD_LIFETIME;
	static const float FORCE_SPHERE_START_RADIUS;
	static const float FORCE_SPHERE_START_STRENGTH;

	float forceSphereRadius_;
	float forceStrength_;
	float lifetime_;

};

}
#endif /* GRAVITYBOMBFIELD_H_ */

