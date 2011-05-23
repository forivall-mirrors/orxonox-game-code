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
    @file MoveToDockingTarget.h
    @brief Definition of the MoveToDockingTarget class.
    @ingroup Docking
*/

#ifndef _MoveToDockingTarget_H__
#define _MoveToDockingTarget_H__

#include "DockingPrereqs.h"
#include "DockingAnimation.h"
#include "DockingController.h"
#include "Dock.h"

namespace orxonox
{

    /**
    @brief
        Base class for docking animations used by @ref orxonox::Docking "Docks".

    @author
        Sven Stucki

    @ingroup Docking
    */
    class _DockingExport MoveToDockingTarget : public DockingAnimation
    {
        public:
            MoveToDockingTarget(BaseObject* creator);
            virtual ~MoveToDockingTarget();

            virtual bool docking(PlayerInfo* player); //!< Called when a player starts docking
            virtual bool release(PlayerInfo* player); //!< Called when player wants to undock
    };

}

#endif /* _MoveToDockingTarget_H__ */
