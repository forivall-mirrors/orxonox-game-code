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
 *      Sven Stucki
 *   Co-authors:
 *      ...
 *
 */

/**
    @file DockToShip.h
    @brief DockingEffect which transfers control from spaceship to docked ship ASDF
    @ingroup Docking
*/

#ifndef _DockToShip_H__
#define _DockToShip_H__

#include "DockingPrereqs.h"
#include "DockToShip.h"

#include "worldentities/ControllableEntity.h"


namespace orxonox
{

    /**
    @brief
        Allows players to dock onto a ship

    @author
        Sven Stucki

    @ingroup Docking
    */
    class _DockingExport DockToShip : public DockingEffect
    {
        public:
            DockToShip(BaseObject* creator);
            virtual ~DockToShip();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void setTargetId(const std::string& str);
            const std::string& getTargetId() const;

            virtual bool docking(PlayerInfo* player); //!< Called when docking starts
            virtual bool release(PlayerInfo* player); //!< Called when player wants undock
        private:
            std::string target_;
    };

}

#endif /* _DockToShip_H__ */
