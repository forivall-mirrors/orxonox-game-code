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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _ArtificialController_H__
#define _ArtificialController_H__

#include "OrxonoxPrereqs.h"

#include "Controller.h"
#include "objects/worldentities/pawns/Pawn.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport ArtificialController : public Controller, public PawnListener
    {
        public:
            ArtificialController(BaseObject* creator);
            virtual ~ArtificialController();

            virtual void destroyedPawn(Pawn* pawn);

        protected:
            void moveToTargetPosition(float dt);
            void searchRandomTargetPosition();
            void searchNewTarget();
            void forgetTarget();
            void aimAtTarget();

            bool isCloseAtTarget(float distance) const;
            bool isLookingAtTarget(float angle) const;

            bool bHasTargetPosition_;
            Vector3 targetPosition_;
            Pawn* target_;
            bool bShooting_;

        private:
    };
}

#endif /* _ArtificialController_H__ */
