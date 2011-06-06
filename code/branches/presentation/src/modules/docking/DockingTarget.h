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
    @file DockingTarget.h
    @brief Definition of the DockingTarget class.
    @ingroup Docking
*/

#ifndef _DockingTarget_H__
#define _DockingTarget_H__

#include "DockingPrereqs.h"

#include "core/BaseObject.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    /**
    @brief
        DockingTargets for @ref orxonox::Docking "Docks".

    @author
        Sven Stucki

    @ingroup Docking
    */
    class _DockingExport DockingTarget : public StaticEntity
    {
        public:
            DockingTarget(BaseObject* creator);
            virtual ~DockingTarget();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

    };

}

#endif /* _DockingTarget_H__ */
