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
    @file InvaderCenterPoint.h
    @brief Declaration of the InvaderCenterPoint class.
    @ingroup Invader
*/

#ifndef _InvaderCenterPoint_H__
#define _InvaderCenterPoint_H__

#include "invader/InvaderPrereqs.h"

#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _InvaderExport InvaderCenterPoint : public StaticEntity
    {
        public:
            InvaderCenterPoint(Context* context); //!< Constructor. Registers and initializes the object and checks whether the gametype is actually Invader.
            virtual ~InvaderCenterPoint() {}


            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method to create a PongCenterpoint through XML.

            virtual void changedGametype(); //!< Is called when the gametype has changed.
        private:
            void checkGametype(); //!< Checks whether the gametype is Pong and if it is, sets its centerpoint.

    };
}

#endif /* _InvaderCenterPoint_H__ */
