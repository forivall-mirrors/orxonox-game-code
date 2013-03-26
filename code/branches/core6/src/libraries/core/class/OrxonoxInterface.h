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

/**
    @file
    @ingroup Class OrxonoxClass
    @brief Declaration of OrxonoxInterface, the base class of all interfaces in Orxonox.
*/

#ifndef _OrxonoxInterface_H__
#define _OrxonoxInterface_H__

#include "core/CorePrereqs.h"

#include "core/config/Configurable.h"
#include "core/object/Destroyable.h"

namespace orxonox
{
    /**
        @brief This is the class from which all interfaces of the game-logic (not the engine) are derived from.

        Interfaces are derived with @c virtual @c public @c OrxonoxInterface from OrxonoxInterface.
    */
    class _CoreExport OrxonoxInterface : virtual public Configurable, virtual public Destroyable
    {
    };
}

#endif /* _OrxonoxInterface_H__ */
