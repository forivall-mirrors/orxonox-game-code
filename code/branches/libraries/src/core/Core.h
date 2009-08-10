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
@file
@brief
    Declaration of the Core class.
@details
    The Core class is a singleton, only used to configure some variables
    in the core through the config-file.
*/

#ifndef _Core_H__
#define _Core_H__

#include "CorePrereqs.h"

#include <cassert>
#include <boost/scoped_ptr.hpp>
#include "util/OutputHandler.h"
#include "util/ScopeGuard.h"
#include "util/Singleton.h"

namespace orxonox
{
    class CoreConfiguration;
    using boost::scoped_ptr;

    /**
    @brief
        The Core class is a singleton used to configure the program basics.
    @details
        The class provides information about the media, config and log path.
        It determines those by the use of platform specific functions.
    @remark
        You should only create this singleton once because it destroys the identifiers!
    */
    class _CoreExport Core : public Singleton<Core>
    {
        typedef Loki::ScopeGuardImpl0<void (*)()> SimpleScopeGuard;
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

            void setConfigValues();

            bool preUpdate(const Clock& time) throw();
            bool postUpdate(const Clock& time) throw();

            void loadGraphics();
            void unloadGraphics();

            static int   getSoftDebugLevel(OutputHandler::OutputDevice device = OutputHandler::LD_All);
            static void  setSoftDebugLevel(OutputHandler::OutputDevice device, int level);
            static const std::string& getLanguage();
            static void  resetLanguage();

            static void tsetMediaPath(const std::string& path);
            //! Returns the path to the config files as boost::filesystem::path
            static const boost::filesystem::path& getMediaPath();
            //! Returns the path to the config files as boost::filesystem::path
            static const boost::filesystem::path& getConfigPath();
            //! Returns the path to the log files as boost::filesystem::path
            static const boost::filesystem::path& getLogPath();
            //! Returns the path to the root folder as boost::filesystem::path
            static const boost::filesystem::path& getRootPath();
            //! Returns the path to the data files as std::string
            static std::string getMediaPathString();
            //! Returns the path to the config files as std::string
            static std::string getConfigPathString();
            //! Returns the path to the log files as std::string
            static std::string getLogPathString();
            //! Returns the path to the root folder as std::string
            static std::string getRootPathString();

            static bool isDevelopmentRun() { return getInstance().bDevRun_; }

        private:
            Core(const Core&); //!< Don't use (undefined symbol)

            void checkDevBuild();
            void setExecutablePath();
            void createDirectories();
            void setThreadAffinity(int limitToCPU);

            // Mind the order for the destruction!
            scoped_ptr<SignalHandler>     signalHandler_;
            SimpleScopeGuard              identifierDestroyer_;
            SimpleScopeGuard              consoleCommandDestroyer_;
            scoped_ptr<ConfigFileManager> configFileManager_;
            scoped_ptr<Language>          languageInstance_;
            scoped_ptr<CoreConfiguration> configuration_;
            scoped_ptr<LuaBind>           luaBind_;
            scoped_ptr<TclBind>           tclBind_;
            scoped_ptr<TclThreadManager>  tclThreadManager_;
            scoped_ptr<Shell>             shell_;
            // graphical
            scoped_ptr<GraphicsManager>   graphicsManager_;     //!< Interface to OGRE
            scoped_ptr<InputManager>      inputManager_;        //!< Interface to OIS
            scoped_ptr<GUIManager>        guiManager_;          //!< Interface to GUI

            bool                          bDevRun_;             //!< True for runs in the build directory (not installed)
            bool                          bGraphicsLoaded_;

            static Core* singletonPtr_s;
    };
}

#endif /* _Core_H__ */
