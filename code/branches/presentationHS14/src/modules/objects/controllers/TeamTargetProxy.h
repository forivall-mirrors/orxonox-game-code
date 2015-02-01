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
 *      Martin Mueller
 *   Co-authors:
 *      ...
 *
 */

#ifndef _TeamTargetProxy_H__
#define _TeamTargetProxy_H__

#include "objects/ObjectsPrereqs.h"
#include "controllers/FormationController.h"

 namespace orxonox
 {
 	/**
 		@brief
 		A controller, that just copies the team and the target of a parent for itself and it's controllable entity.

 		Useful for following (and similar) situations: (-> means attached to)
			turret (rotates) -> some kind of turret base (looks nice) -> spaceship (flies around)
			The turret has a controller that wants to copy the spaceship's target and team. In this case it doesn't work though,
			because the turret isn't directly attached to the spaceship. Here's where this controller comes in. Drawback: the base
			has to be controllable and ticks every second (performance?)
 	*/
 	class _ObjectsExport TeamTargetProxy : public FormationController, public Tickable
 	{
 		public:
 			TeamTargetProxy(Context* context);
 			virtual ~TeamTargetProxy();

 			virtual void tick(float dt);

 		private:
 			bool once_; //!< Flag for executing code in the tick function only once.
 	};
 }

 #endif
