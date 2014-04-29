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

 #ifndef _TurretController_H__
 #define _TurretController_H__

#include "OrxonoxPrereqs.h"
#include "controllers/ArtificialController.h"

 namespace orxonox
 {
 	class _OrxonoxExport TurretController : public ArtificialController, public Tickable
 	{
 		public:
 			TurretController(Context* context);
 			virtual ~TurretController();

 			virtual void tick(float dt);

 		private:
            bool gotOrient_;
            float attackRadius_;
            Ogre::Real maxPitch_;
            Ogre::Real maxYaw_;
            Quaternion startOrient_;
            Vector3 startDir_;
            Vector3 localZ_;
            Vector3 localZStart_;
            Vector3 localY_;
            Vector3 localYStart_;
            Vector3 localX_;
            Vector3 localXStart_;

            void aimAtPositionRot(const Vector3 &position);
            void aimAtTargetRot();
 			void searchTarget();
 			bool isInRange(const Vector3 &position);
 			bool isLookingAtTargetNew(float angle) const;
 	};
 }

 #endif