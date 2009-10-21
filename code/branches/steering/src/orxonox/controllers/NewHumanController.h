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

//#include "tools/interfaces/Tickable.h"
#include "HumanController.h"
#include "core/input/InputHandler.h"
#include "core/input/InputState.h"

namespace orxonox
{
    class _OrxonoxExport NewHumanController : public HumanController, public InputHandler
    {
        public:
            NewHumanController(BaseObject* creator);
            virtual ~NewHumanController();

            virtual void tick(float dt);

            void startControl();
            void stopControl();

        private:
            static NewHumanController* localController_s;
            InputState* gameInputState_;

            //mouse handler functions
            void mouseMoved    (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize);
    };
}

#endif /* _NewHumanController_H__ */
