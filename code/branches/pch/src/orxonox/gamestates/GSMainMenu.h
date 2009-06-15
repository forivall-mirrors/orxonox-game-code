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

#include <OgrePrerequisites.h>
#include "core/GameState.h"

namespace orxonox
{
    class _OrxonoxExport GSMainMenu : public GameState
    {
    public:
        GSMainMenu(const std::string& name);
        ~GSMainMenu();

        void activate();
        void deactivate();
        void update(const Clock& time);

        void startStandalone();
        void startServer();
        void startClient();
        void startDedicated();

    private:
        SimpleInputState* inputState_;
        Scene*            scene_;
        Ogre::Camera*     camera_;

        // console commands
        ConsoleCommand* ccStartStandalone_;
        ConsoleCommand* ccStartServer_;
        ConsoleCommand* ccStartClient_;
        ConsoleCommand* ccStartDedicated_;

        // ambient sound for the main menu
        SoundMainMenu* ambient_;
    };
}

#endif /* _GSMainMenu_H__ */
