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

#include <cassert>
#include "OrxonoxClass.h"
#include "util/OutputHandler.h"

// Only allow main to access setDevBuild, so we need a forward declaration
int main(int, char**);

namespace orxonox
{
    //! The Core class is a singleton, only used to configure some config-values.
    class _CoreExport Core : public OrxonoxClass
    {
        friend int ::main(int, char**); // sets isDevBuild_s

        public:
            Core();
            ~Core();
            void setConfigValues();

            static Core& getInstance() { assert(Core::singletonRef_s); return *Core::singletonRef_s; }

            static int   getSoftDebugLevel(OutputHandler::OutputDevice device = OutputHandler::LD_All);
            static void  setSoftDebugLevel(OutputHandler::OutputDevice device, int level);
            static const std::string& getLanguage();
            static void  resetLanguage();

            static bool isDevBuild() { return Core::isDevBuild_s; }

            static void tsetMediaPath(const std::string& path)
            { assert(singletonRef_s); singletonRef_s->_tsetMediaPath(path); }
            static const std::string& getMediaPath()  { return mediaPath_s; }
            static const std::string& getConfigPath() { return configPath_s; }
            static const std::string& getLogPath()    { return logPath_s; }

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
            Core(const Core&);
            void resetLanguageIntern();
            void initializeRandomNumberGenerator();
            void debugLevelChanged();
            void languageChanged();
            void mediaPathChanged();
            void _tsetMediaPath(const std::string& path);

            static void createDirectories();
            static void checkDevBuild();

            int softDebugLevel_;                            //!< The debug level
            int softDebugLevelConsole_;                     //!< The debug level for the console
            int softDebugLevelLogfile_;                     //!< The debug level for the logfile
            int softDebugLevelShell_;                       //!< The debug level for the ingame shell
            std::string language_;                          //!< The language
            bool bInitializeRandomNumberGenerator_;         //!< If true, srand(time(0)) is called

            static bool bShowsGraphics_s;                   //!< global variable that tells whether to show graphics
            static bool bHasServer_s;                       //!< global variable that tells whether this is a server
            static bool bIsClient_s;
            static bool bIsStandalone_s;
            static bool bIsMaster_s;

            static bool isDevBuild_s;                       //!< True for builds in the build directory (not installed)
            static std::string configPath_s;                //!< Path to the config file folder
            static std::string logPath_s;                   //!< Path to the log file folder
            static std::string mediaPath_s;                 //!< Path to the data/media file folder

            static Core* singletonRef_s;
    };
}

#endif /* _Core_H__ */
