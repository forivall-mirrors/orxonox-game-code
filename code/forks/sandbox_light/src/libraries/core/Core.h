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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
    @defgroup CoreGame Core and Game
    @ingroup Management
*/

/**
    @file
    @ingroup Management CoreGame
    @brief Declaration of the Core singleton which is used to configure the program basics.
*/

#ifndef _Core_H__
#define _Core_H__

#include "CorePrereqs.h"

#include <string>
#include <boost/scoped_ptr.hpp>

#include "util/Singleton.h"

namespace orxonox
{
    /**
    @brief
        The Core class is a singleton used to configure the program basics.
    @remark
        You should only create this singleton once because it destroys the identifiers!
    */
    class _CoreExport Core : public Singleton<Core>
    {
        friend class Singleton<Core>;

        public:
            /**
            @brief
                Determines the executable path, checks for build directory runs, creates
                the output directories and sets up the other core library singletons.
            @throws
                GeneralException
            */
            Core(const std::string& cmdLine);
            ~Core();

        private:
            Core(const Core&); //!< Don't use (undefined symbol)

            void initRandomNumberGenerator();

            void setThreadAffinity(int limitToCPU);

            // MANAGED SINGLETONS/OBJECTS
            // Mind the order for the destruction!
            scoped_ptr<PathConfig>        pathConfig_;
            scoped_ptr<SignalHandler>     signalHandler_;

            int                           softDebugLevelLogFile_;      //!< The debug level for the log file (belongs to OutputHandler)
            bool                          bInitRandomNumberGenerator_; //!< If true, srand(time(0)) is called

            static Core*                  singletonPtr_s;
    };
}

#endif /* _Core_H__ */
