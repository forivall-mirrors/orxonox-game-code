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

#ifndef _UpdateListener_H__
#define _UpdateListener_H__

#include "core/CorePrereqs.h"

#include "core/object/Listable.h"

namespace orxonox
{
    /**
     * Inherit from UpdateListener if you need to receive calls before or after the game is ticked. All classes inheriting from UpdateListener
     * need to be strictly independent of each other and may not rely on a specific order in which all UpdateListeners are called.
     *
     * If you do have such a dependency between two UpdateListeners, e.g. A::preUpdate() always needs to be called before B::preUpdate(), then
     * you need to create a third class C (which inherits from UpdateListener) with the following implementation:
     * void C::preUpdate()
     * {
     *     A::preUpdate();
     *     B::preUpdate();
     * }
     * This is the only way to ensure that A gets called before B. In this example, only C inherits from UpdateListener, while A and B do not.
     * Instead they receive the update from C.
     */
    class _CoreExport UpdateListener : virtual public Listable
    {
        public:
            UpdateListener();

            /// Gets called by Core before the framework is ticked.
            virtual void preUpdate(const Clock& time) = 0;
            /// Gets called by Core after the framework was ticked (but before graphics are drawn).
            virtual void postUpdate(const Clock& time) = 0;
    };
}

#endif /* _UpdateListener_H__ */
