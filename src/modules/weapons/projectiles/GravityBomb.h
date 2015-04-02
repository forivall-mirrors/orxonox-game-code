/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Gabriel Nadler
 *   Co-authors:
 *      simonmie
 *
 */


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

namespace orxonox
{

	class _WeaponsExport GravityBomb : public BasicProjectile , public MovableEntity, public RadarViewable
	{
		public:
			GravityBomb(Context* context);
			virtual ~GravityBomb();
			virtual void tick(float dt);

			virtual bool collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint);
			void detonate();

		private:
		static const float INITIAL_VELOCITY;
		static const float SLOW_DOWN_RATIO;

		float velocityAtLastTick_; //Used to check wether the Object is already accelarating in the oposite direction to detect the time to detonate it.

	};
}
#endif /* GravityBOMB_H_ */
