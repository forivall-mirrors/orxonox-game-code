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

#ifndef _GSLevelMemento_H__
#define _GSLevelMemento_H__

#include "OrxonoxPrereqs.h"

#include "core/class/OrxonoxInterface.h"

namespace orxonox
{
    /**
        @brief This class is an interface for all instances that want to maintain a state beyond the reloading of a level.
    */
    class _OrxonoxExport GSLevelMemento : virtual public OrxonoxInterface
    {
        friend class GSLevel;

        public:
            GSLevelMemento(); // Implemented in GSLevel.cc

        protected:
            /**
             * Returns the state of this memento. Returns NULL if no state needed to persist.
             */
            virtual GSLevelMementoState* exportMementoState() = 0;

            /**
             * A list of the states of all mementos. must find the correct one by itself.
             */
            virtual void importMementoState(const std::vector<GSLevelMementoState*>& states) = 0;
    };

    /**
        @brief Represents the state of a memento. Needs to be inherited by the actual state.
    */
    struct _OrxonoxExport GSLevelMementoState
    {
        virtual ~GSLevelMementoState() {} // virtual destructor for RTTI
    };
}

#endif /* _GSLevelMemento_H__ */
