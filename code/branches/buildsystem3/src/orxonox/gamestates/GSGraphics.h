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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _GSGraphics_H__
#define _GSGraphics_H__

#include "OrxonoxPrereqs.h"
#include <OgrePrerequisites.h>
#include <OgreWindowEventUtilities.h>
#include "core/GameState.h"
#include "core/OrxonoxClass.h"
#include "GSRoot.h"

namespace orxonox
{
    class _OrxonoxExport GSGraphics : public GameState<GSRoot>, public OrxonoxClass,
                                      public Ogre::WindowEventListener, public Ogre::LogListener
    {
        friend class ClassIdentifier<GSGraphics>;

    public:
        GSGraphics();
        ~GSGraphics();

        Ogre::Root*     getOgreRoot()   { return this->ogreRoot_  ; }
        Ogre::Viewport* getViewport()   { return this->viewport_  ; }
        GUIManager*     getGUIManager() { return this->guiManager_; }

    private: // functions
        void enter();
        void leave();
        void ticked(const Clock& time);

        void setConfigValues();

        void setupOgre();
        void declareResources();
        void loadRenderer();
        void initialiseResources();

        // console commands
        void printScreen();

        // event from Ogre::LogListener
        void messageLogged(const std::string& message, Ogre::LogMessageLevel lml,
            bool maskDebug, const std::string& logName);

        // window events from Ogre::WindowEventListener
        void windowMoved       (Ogre::RenderWindow* rw);
        void windowResized     (Ogre::RenderWindow* rw);
        void windowFocusChange (Ogre::RenderWindow* rw);
        void windowClosed      (Ogre::RenderWindow* rw);

        void requestWindowEventListenerUpdate() { this->bWindowEventListenerUpdateRequired_ = true; }

    private: // variables
        Ogre::RenderWindow*   renderWindow_;          //!< the current render window
        Ogre::Viewport*       viewport_;              //!< default full size viewport
        bool bWindowEventListenerUpdateRequired_;     //!< True if a new WindowEventListener was created but not yet updated.

        // managed singletons
        InputManager*         inputManager_;
        InGameConsole*        console_;
        GUIManager*           guiManager_;
        Ogre::Root*           ogreRoot_;                  //!< Ogre's root
        Ogre::LogManager*     ogreLogger_;
        GraphicsEngine*       graphicsEngine_;   //!< Interface to Ogre

        KeyBinder*            masterKeyBinder_;
        XMLFile*              debugOverlay_;

        // config values
        std::string           resourceFile_;             //!< resources file name
        std::string           ogreConfigFile_;           //!< ogre config file name
        std::string           ogrePluginsFile_;          //!< ogre plugins file name
        std::string           ogreLogFile_;              //!< log file name for Ogre log messages
        int                   ogreLogLevelTrivial_;      //!< Corresponding Orxonx debug level for LL_TRIVIAL
        int                   ogreLogLevelNormal_;       //!< Corresponding Orxonx debug level for LL_NORMAL
        int                   ogreLogLevelCritical_;     //!< Corresponding Orxonx debug level for LL_CRITICAL
        std::string           defaultMasterKeybindings_; //!< Filename of default master keybindings.

        // console commands
        ConsoleCommand*       ccPrintScreen_;
    };
}

#endif /* _GSGraphics_H__ */
