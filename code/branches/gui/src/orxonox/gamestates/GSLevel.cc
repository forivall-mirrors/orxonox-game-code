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

#include "OrxonoxStableHeaders.h"
#include "GSLevel.h"

#include "GraphicsEngine.h"
#include "core/ConsoleCommand.h"
#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "core/input/KeyBinder.h"
#include "overlays/console/InGameConsole.h"
#include "gui/GUIManager.h"

namespace orxonox
{
    GSLevel::GSLevel()
        : GameState("level")
        , timefactor_(1.0f)
    {
    }

    GSLevel::~GSLevel()
    {
    }

    void GSLevel::enter()
    {
        KeyBinder* keyBinder = new KeyBinder();
        keyBinder->loadBindings();
        InputManager::getInstance().createInputState<SimpleInputState>("game", 20)->setHandler(keyBinder);

        // add console commands
        //ConsoleCommand* command = createConsoleCommand(createFunctor(&classname::function), )
        //CommandExecutor::addConsoleCommandShortcut(command);
    }

    void GSLevel::leave()
    {
    }

    bool GSLevel::tick(float dt)
    {
        // Call those objects that need the real time
        for (Iterator<TickableReal> it = ObjectList<TickableReal>::start(); it; ++it)
            it->tick(dt);
        // Call the scene objects
        for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it)
            it->tick(dt * this->timefactor_);

        // call server/client with normal dt
        //if (client_g)
        //    client_g->tick(dt * this->timefactor_);
        //if (server_g)
        //    server_g->tick(dt * this->timefactor_);

        return true;
    }

    /**
    @brief
        Changes the speed of Orxonox
    */
    //void GSLevel::setTimeFactor(float factor)
    //{
    //    float change = factor / Orxonox::getInstance().getTimeFactor();
    //    Orxonox::getInstance().timefactor_ = factor;
    //    for (Iterator<ParticleInterface> it = ObjectList<ParticleInterface>::begin(); it; ++it)
    //        it->setSpeedFactor(it->getSpeedFactor() * change);

    //    for (Iterator<Backlight> it = ObjectList<Backlight>::begin(); it; ++it)
    //        it->setTimeFactor(Orxonox::getInstance().getTimeFactor());
    //}
}
