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
#include <loki/ScopeGuard.h>

#include "util/Singleton.h"
#include "OrxonoxClass.h"

namespace orxonox
{
    /**
    @brief
        The Core class is a singleton used to configure the program basics.
    @remark
        You should only create this singleton once because it destroys the identifiers!
    */
    class _CoreExport Core : public Singleton<Core>, public OrxonoxClass
    {
        typedef Loki::ScopeGuardImpl0<void (*)()> SimpleScopeGuard;
        friend class Singleton<Core>;
        friend class Game;

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

            //! Returns the configured language.
            const std::string& getLanguage()
                { return this->language_; }
            void resetLanguage();

            void updateLastLevelTimestamp();
            inline long long getLastLevelTimestamp() const
                { return this->lastLevelTimestamp_; }

            void updateOgreConfigTimestamp();
            inline long long getOgreConfigTimestamp() const
                { return this->ogreConfigTimestamp_; }

            inline bool inDevMode(void) const
                { return this->bDevMode_; }

        private:
            Core(const Core&); //!< Don't use (undefined symbol)

            void languageChanged();
            void initRandomNumberGenerator();

            void preUpdate(const Clock& time);
            void postUpdate(const Clock& time);

            void loadGraphics();
            void unloadGraphics();

            void setThreadAffinity(int limitToCPU);
            // MANAGED SINGLETONS/OBJECTS
            // Mind the order for the destruction!
            scoped_ptr<PathConfig>        pathConfig_;
            scoped_ptr<DynLibManager>     dynLibManager_;
            scoped_ptr<SignalHandler>     signalHandler_;
            SimpleScopeGuard              identifierDestroyer_;
            SimpleScopeGuard              consoleCommandDestroyer_;
            scoped_ptr<ConfigFileManager> configFileManager_;
            scoped_ptr<Language>          languageInstance_;
            scoped_ptr<IOConsole>         ioConsole_;
            scoped_ptr<TclBind>           tclBind_;
            scoped_ptr<TclThreadManager>  tclThreadManager_;
            scoped_ptr<Scope<ScopeID::Root> > rootScope_;
            // graphical
            scoped_ptr<GraphicsManager>   graphicsManager_;     //!< Interface to OGRE
            scoped_ptr<InputManager>      inputManager_;        //!< Interface to OIS
            scoped_ptr<GUIManager>        guiManager_;          //!< Interface to GUI
            scoped_ptr<Scope<ScopeID::Graphics> > graphicsScope_;

            bool                          bGraphicsLoaded_;
            int                           softDebugLevelLogFile_;      //!< The debug level for the log file (belongs to OutputHandler)
            std::string                   language_;                   //!< The language
            bool                          bInitRandomNumberGenerator_; //!< If true, srand(time(0)) is called
            bool                          bStartIOConsole_;            //!< Set to false if you don't want to use the IOConsole
            long long                     lastLevelTimestamp_;         ///< Timestamp when the last level was started
            long long                     ogreConfigTimestamp_;        ///< Timestamp wehen the ogre config level was modified
            bool                          bDevMode_;                   //!< Developers bit. If set to false, some options are not available as to not confuse the normal user.

            static Core*                  singletonPtr_s;
    };
}

#endif /* _Core_H__ */
