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
    @file DockingEffect.h
    @brief Definition of the DockingEffect class.
    @ingroup Docking
*/

#ifndef _DockingEffect_H__
#define _DockingEffect_H__

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "infos/PlayerInfo.h"

#include "DockingPrereqs.h"
#include "DockingTarget.h"

#include <list>
#include "core/BaseObject.h"

namespace orxonox
{

    /**
    @brief
        Handles DockingEffects for @ref orxonox::Docking "Docks".

    @author
        Sven Stucki

    @ingroup Docking
    */
    class _DockingExport DockingEffect : public BaseObject
    {
        public:
            DockingEffect(BaseObject* creator);
            virtual ~DockingEffect();

            virtual bool docking(PlayerInfo* player) = 0; //!< Called when player docked
            virtual bool release(PlayerInfo* player) = 0; //!< Called when player wants to undock

            static bool invokeEffect(bool dock, PlayerInfo* player, std::list<DockingEffect*> & effects); //!< Invokes the event specific method of all DockingEffects in the list
            static DockingTarget *findTarget(std::string name); //!< Iterates through all DockingTarget objects to find the one with name=target
    };

}

#endif /* _DockingEffect_H__ */
