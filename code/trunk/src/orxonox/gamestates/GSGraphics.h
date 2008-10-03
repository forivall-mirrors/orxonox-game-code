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
    class _OrxonoxExport GSGraphics : public GameState<GSRoot>, public OrxonoxClass, public Ogre::WindowEventListener
    {
        friend class ClassIdentifier<GSGraphics>;
    public:
        GSGraphics();
        ~GSGraphics();

        Ogre::Viewport* getViewport() { return this->viewport_; }
        GUIManager* getGUIManager() { return this->guiManager_; }

    private:
        void enter();
        void leave();
        void ticked(const Clock& time);

        void setConfigValues();

        void declareResources();
        void loadRenderer();
        void initialiseResources();

        void printScreen();

        // window events from Ogre::WindowEventListener
        void windowMoved       (Ogre::RenderWindow* rw);
        void windowResized     (Ogre::RenderWindow* rw);
        void windowFocusChange (Ogre::RenderWindow* rw);
        void windowClosed      (Ogre::RenderWindow* rw);

        Ogre::Root*           ogreRoot_;
        Ogre::RenderWindow*   renderWindow_;          //!< the current render window
        Ogre::Viewport*       viewport_;              //!< default full size viewport

        // managed singletons
        InputManager*         inputManager_;
        InGameConsole*        console_;
        GUIManager*           guiManager_;

        KeyBinder*            masterKeyBinder_;

        // variables for time statistics
        unsigned long         frameCount_;
        unsigned int          statisticsRefreshCycle_;
        unsigned long long    statisticsStartTime_;
        unsigned long         statisticsStartCount_;
        unsigned int          tickTime_;

        // config values
        std::string           resourceFile_;          //!< resources file name
        unsigned int          detailLevelParticle_;   //!< Detail level of particle effects (0: off, 1: low, 2: normal, 3: high)
    };
}

#endif /* _GSGraphics_H__ */
