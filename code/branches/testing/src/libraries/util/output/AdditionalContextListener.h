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
    @ingroup Output
    @brief Declaration of the AdditionalContextListener interface
*/

#ifndef _AdditionalContextListener_H__
#define _AdditionalContextListener_H__

#include "util/UtilPrereqs.h"

#include <vector>

#include "OutputDefinitions.h"

namespace orxonox
{
    /**
        @brief AdditionalContextListener is an interface which is used to notify OutputManager about additional contexts in OutputListeners
    */
    class _UtilExport AdditionalContextListener
    {
        public:
            virtual void updatedLevelMask(const OutputListener* listener) = 0;
            virtual void updatedAdditionalContextsLevelMask(const OutputListener* listener) = 0;
            virtual void updatedAdditionalContextsMask(const OutputListener* listener) = 0;
    };
}

#endif /* _AdditionalContextListener_H__ */
