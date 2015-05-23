/*
 * GravityBomb.h
 *
 *  Created on: Mar 19, 2015
 *      Author: meggiman
 */

#ifndef GravityBOMB_H_
#define GravityBOMB_H_

#include "weapons/WeaponsPrereqs.h"

#include "tools/Timer.h"

#include "graphics/ParticleSpawner.h"
#include "interfaces/RadarViewable.h"
#include "objects/ForceField.h"
#include "BasicProjectile.h"
#include "worldentities/MovableEntity.h"
#include "core/CoreIncludes.h"
#include "objects/collisionshapes/SphereCollisionShape.h"
#include "../../../orxonox/worldentities/WorldEntity.h"
#include "GravityBombField.h"
#include "sound\WorldSound.h"

namespace orxonox
{
	/**
	 * @class	GravityBomb
	 *
	 * @brief	This class implements how long the bomb flies before it places the GravityField at it's last possition.
	 * 			The field will be created either because the timelimit of the bomb expired or it hit something. After creation of the field,
	 * 			the projectile (this object) is destroyed.
	 *
	 * @author	Manuel Eggimann
	 * @date	23.05.2015
	 */
	class _WeaponsExport GravityBomb : public BasicProjectile , public MovableEntity, public RadarViewable
	{
		public:
			GravityBomb(Context* context);
			virtual ~GravityBomb();
			virtual void tick(float dt);

			virtual bool collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint);
			void detonate();
		private:
		static const float LIFETIME;

		bool isDetonated_; //Used to check whether the Bomb has to be destroyed during next tick.
		float timeToLife_; //Time the bomb flies before it explodes.
		WorldSound* bombSound_;
	};
}
#endif /* GravityBOMB_H_ */
