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
#include "GSGUI.h"

#include <OgreViewport.h>
#include "core/Clock.h"
#include "core/ConsoleCommand.h"
#include "core/Game.h"
#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "gui/GUIManager.h"
#include "GraphicsManager.h"

namespace orxonox
{
    AddGameState(GSGUI, "mainMenu");

    GSGUI::GSGUI(const std::string& name)
        : GameState(name)
    {
    }

    GSGUI::~GSGUI()
    {
    }

    void GSGUI::activate()
    {
        guiManager_ = GUIManager::getInstancePtr();

        // show main menu
        //guiManager_->loadScene("MainMenu");
        guiManager_->showGUI("MainMenu", 0);
        GraphicsManager::getInstance().getViewport()->setCamera(guiManager_->getCamera());

        {
            // time factor console command
            FunctorMember<GSGUI>* functor = createFunctor(&GSGUI::startGame);
            functor->setObject(this);
            this->ccStartGame_ = createConsoleCommand(functor, "startGame");
            CommandExecutor::addConsoleCommandShortcut(this->ccStartGame_);
        }
    }

    void GSGUI::deactivate()
    {
        if (this->ccStartGame_)
        {
            delete this->ccStartGame_;
            this->ccStartGame_ = 0;
        }

        guiManager_->hideGUI();
    }

    void GSGUI::update(const Clock& time)
    {
        // tick CEGUI
        guiManager_->update(time);
    }

    void GSGUI::startGame()
    {
        // HACK - HACK
        Game::getInstance().popState();
        Game::getInstance().requestState("standalone");
        Game::getInstance().requestState("level");
    }
}
