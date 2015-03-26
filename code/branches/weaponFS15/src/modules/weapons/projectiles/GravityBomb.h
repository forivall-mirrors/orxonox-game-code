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

namespace orxonox
{

	class ConeCollisionShape;

	class _WeaponsExport GravityBomb : public BasicProjectile , public MovableEntity, public RadarViewable
	{
		public:
			GravityBomb(Context* context);
			virtual ~GravityBomb();
			virtual void tick(float dt);

			virtual bool collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint);

		private:
		static const float FUEL_START;
		static const float FORCE_SPHERE_START_RADIUS;
		static const float FORCE_SPHERE_START_STRENGTH;

		float fuel_;
		float lifetime_;
		float forceSphereRadius_;
		float forceStrength_;


	};
}
#endif /* GravityBOMB_H_ */
