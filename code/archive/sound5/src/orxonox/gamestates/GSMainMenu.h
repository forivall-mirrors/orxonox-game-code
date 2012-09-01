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

#ifndef _GSMainMenu_H__
#define _GSMainMenu_H__

#include "OrxonoxPrereqs.h"

#include "util/OgreForwardRefs.h"
#include "core/GameState.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    class _OrxonoxExport GSMainMenu : public GameState, public OrxonoxClass
    {
    public:
        GSMainMenu(const GameStateInfo& info);
        ~GSMainMenu();

        void activate();
        void deactivate();
        void update(const Clock& time);

        void setConfigValues();
        void reloadSound();
        const std::string& getMainMenuSoundPath();
        void setMainMenuSoundPath(const std::string& path);

        static void startStandalone();
        static void startServer();
        static void startClient();
        static void startDedicated();
        static void startMainMenu();
        static void startIOConsole();

    private:
        InputState*       inputState_;
        Scene*            scene_;
        Ogre::Camera*     camera_;

        // ambient sound for the main menu
        AmbientSound*     ambient_;
        std::string       soundPathMain_;
    };
}

#endif /* _GSMainMenu_H__ */
