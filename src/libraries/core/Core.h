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
#include "util/DestructionHelper.h"
#include "util/Singleton.h"
#include "CoreConfig.h"

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

            /// Leave empty and use destroy() instead
            ~Core() {}
            /// Destructor that also executes when the object fails to construct
            void destroy();

            void preUpdate(const Clock& time);
            void postUpdate(const Clock& time);

            void loadGraphics();
            void unloadGraphics();

            void loadModules();
            void unloadModules();
            void loadModule(ModuleInstance* module);
            void unloadModule(ModuleInstance* module);

            inline CoreConfig* getConfig() const
                { return this->config_; }

        private:
            Core(const Core&); //!< Don't use (undefined symbol)

            void setThreadAffinity(int limitToCPU);

            ApplicationPaths*         applicationPaths_;
            ConfigurablePaths*        configurablePaths_;
            DynLibManager*            dynLibManager_;
            SignalHandler*            signalHandler_;
            ConfigFileManager*        configFileManager_;
            Language*                 languageInstance_;
            Loader*                   loaderInstance_;
            IOConsole*                ioConsole_;
            TclBind*                  tclBind_;
            TclThreadManager*         tclThreadManager_;
            Scope<ScopeID::ROOT>*     rootScope_;
            // graphical
            GraphicsManager*          graphicsManager_;            //!< Interface to OGRE
            InputManager*             inputManager_;               //!< Interface to OIS
            GUIManager*               guiManager_;                 //!< Interface to GUI
            Scope<ScopeID::GRAPHICS>* graphicsScope_;
            bool                      bGraphicsLoaded_;
            ModuleInstance*           rootModule_;
            std::list<ModuleInstance*>modules_;

            /// Helper object that stores the config values
            CoreConfig*               config_;

            /// Helper object that executes the surrogate destructor destroy()
            DestructionHelper<Core>   destructionHelper_;

            static Core*              singletonPtr_s;
    };
}

#endif /* _Core_H__ */
