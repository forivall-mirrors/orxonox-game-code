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
 *      Benjamin Knecht
 *
 */

#ifndef _GSLevel_H__
#define _GSLevel_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <set>
#include "core/class/OrxonoxClass.h"
#include "core/GameState.h"

namespace orxonox
{
    class _OrxonoxExport GSLevel : public GameState
    {
    public:
        GSLevel(const GameStateInfo& info);
        ~GSLevel();

        void activate();
        void deactivate();
        void update(const Clock& time);

        static void startMainMenu(void); //!< Starts the MainMenu
        static void changeGame(const std::string& level); //!< Terminates the current game and starts a new game.

    protected:
        void loadLevel();
        void unloadLevel();

        InputState*              gameInputState_;          //!< input state for normal ingame playing
        InputState*              guiMouseOnlyInputState_;  //!< input state if we only need the mouse to use the GUI
        InputState*              guiKeysOnlyInputState_;   //!< input state if we only need the keys to use the GUI

        XMLFile* startFile_;
        std::set<BaseObject*> staticObjects_;
        bool bShowIngameGUI_;
    };
}

#endif /* _GSLevel_H__ */
