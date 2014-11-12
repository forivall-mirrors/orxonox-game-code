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
 *      Florian Zinggeler
 *   Co-authors:
 *      ...
 *
 */

/**
    @file DodgeRaceCenterPoint.h
    @brief Declaration of the DodgeRaceCenterPoint class.
    @ingroup DodgeRace
*/

#ifndef _DodgeRaceCenterPoint_H__
#define _DodgeRaceCenterPoint_H__

#include "dodgerace2/DodgeRacePrereqs.h"

//#include "DodgeRace.h"
//#include "DodgeRaceHUDinfo.h"

#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class DodgeRaceCenterPoint : public StaticEntity
    {
        public:
            DodgeRaceCenterPoint(Context* context); //checks whether the gametype is actually DodgeRace.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void changedGametype(); //!< Is called when the gametype has changed.
        private:
            void checkGametype();

    };
}

#endif /* _DodgeRaceCenterPoint_H__ */
