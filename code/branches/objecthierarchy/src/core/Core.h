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
 *      Reto Grieder
 *
 */

/**
    @file
    @brief Declaration of the Core class.

    The Core class is a singleton, only used to configure some variables
    in the core through the config-file.
*/

#ifndef _Core_H__
#define _Core_H__

#include "CorePrereqs.h"

#include "OrxonoxClass.h"
#include "util/OutputHandler.h"

namespace orxonox
{
    //! The Core class is a singleton, only used to configure some config-values.
    class _CoreExport Core : public OrxonoxClass
    {
        public:
            static Core& getInstance();
            static bool& isCreatingCoreSettings();
            void setConfigValues();
            void debugLevelChanged();
            void languageChanged();

            static int getSoftDebugLevel(OutputHandler::OutputDevice device = OutputHandler::LD_All);
            static void setSoftDebugLevel(OutputHandler::OutputDevice device, int level);
            static const std::string& getLanguage();
            static void resetLanguage();

            // fast access global variables.
            static bool showsGraphics() { return bShowsGraphics_s; }
            static bool hasServer()     { return bHasServer_s; }
            static bool isClient()      { return bIsClient_s; }
            static bool isStandalone()  { return bIsStandalone_s; }
            static bool isMaster()      { return bIsMaster_s; }
            static void setShowsGraphics(bool val) { bShowsGraphics_s = val; updateIsMaster(); }
            static void setHasServer    (bool val) { bHasServer_s     = val; updateIsMaster(); }
            static void setIsClient     (bool val) { bIsClient_s      = val; updateIsMaster(); }
            static void setIsStandalone (bool val) { bIsStandalone_s  = val; updateIsMaster(); }
            static void updateIsMaster  ()         { bIsMaster_s      = (bHasServer_s || bIsStandalone_s); }

        private:
            void resetLanguageIntern();

            Core();
            Core(const Core& other);
            virtual ~Core();

            int softDebugLevel_;                            //!< The debug level
            int softDebugLevelConsole_;                     //!< The debug level for the console
            int softDebugLevelLogfile_;                     //!< The debug level for the logfile
            int softDebugLevelShell_;                       //!< The debug level for the ingame shell
            std::string language_;                          //!< The language

            static bool bShowsGraphics_s;                   //!< global variable that tells whether to show graphics
            static bool bHasServer_s;                       //!< global variable that tells whether this is a server
            static bool bIsClient_s;
            static bool bIsStandalone_s;
            static bool bIsMaster_s;
    };
}

#endif /* _Core_H__ */
