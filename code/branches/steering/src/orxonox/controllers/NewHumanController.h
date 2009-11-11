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
 *      Michael Wirth
 *   Co-authors:
 *      ...
 *
 */

#ifndef _NewNewHumanController_H__
#define _NewNewHumanController_H__

#include "OrxonoxPrereqs.h"

#include "HumanController.h"
#include "overlays/OrxonoxOverlay.h"
#include "util/OgreForwardRefs.h"

namespace orxonox
{
    class _OrxonoxExport NewHumanController : public HumanController
    {
        public:
            NewHumanController(BaseObject* creator);
            virtual ~NewHumanController();

            virtual void tick(float dt);

            virtual void yaw(const Vector2& value);
            virtual void pitch(const Vector2& value);

            virtual void doFire(unsigned int firemode);

            void changeMode(unsigned int controleMode);

            virtual Vector3 getTarget();

        private:
            float                       currentYaw_;
            float                       currentPitch_;
            OrxonoxOverlay              *CrossHairOverlay;
            float			overlaySize;
            unsigned int                controlMode;
    };
}

#endif /* _NewHumanController_H__ */
