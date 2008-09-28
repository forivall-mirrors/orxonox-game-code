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
 *      Oliver 'greenman' Scheuss, Reto '1337' Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _TutorialShip_H__
#define _TutorialShip_H__
#include <string>
#include "OrxonoxPrereqs.h"
#include <OgrePrerequisites.h>

#include "util/Math.h"
#include "Camera.h"
#include "Model.h"
#include "RadarViewable.h"
#include "tools/BillboardSet.h"

namespace orxonox
{
    class _OrxonoxExport TutorialShip : public SpaceShip
    {
        public:
            TutorialShip();
            ~TutorialShip();

        private:
            bool create();
            void registerAllVariables();
            void setConfigValues();
            void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void tick(float dt);

            // Methods for console commands
            static void fire();
    };
}

#endif /* _TutorialShip_H__ */
