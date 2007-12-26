/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

/*!
    @file DebugLevel.h
    @brief Definition of the DebugLevel class.

    The DebugLevel class is a singleton, only used to configure the amount of debug
    output (see Debug.h) into the console and the log-file (see OutputHandler.h).
*/

#ifndef _DebugLevel_H__
#define _DebugLevel_H__

#include "CorePrereqs.h"

namespace orxonox
{
    //! The DebugLevel class is a singleton, only used to configure the amount of debug output.
    class _CoreExport DebugLevel
    {
        public:
            static inline int getSoftDebugLevel()
            {
                static DebugLevel theOneAndOnlyInstance = DebugLevel();
                return theOneAndOnlyInstance.softDebugLevel_;
            }

        private:
            DebugLevel();                       // don't create
            DebugLevel(const DebugLevel& dl) {} // don't copy
            ~DebugLevel() {}                    // don't delete

            int softDebugLevel_;                            //!< The debug level
            ConfigValueContainer* softDebugLevelContainer_; //!< The config value container for the debug level
    };
}

#endif /* _DebugLevel_H__ */
