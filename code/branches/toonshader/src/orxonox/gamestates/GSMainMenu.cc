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
#include "GSMainMenu.h"

//#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include "core/Clock.h"
#include "core/ConsoleCommand.h"
#include "core/Game.h"
#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "gui/GUIManager.h"
#include "objects/Scene.h"
#include "GraphicsManager.h"

namespace orxonox
{
    AddGameState(GSMainMenu, "mainMenu");

    GSMainMenu::GSMainMenu(const std::string& name)
        : GameState(name)
        , inputState_(0)
    {
    }

    GSMainMenu::~GSMainMenu()
    {
    }

    void GSMainMenu::activate()
    {
        inputState_ = InputManager::getInstance().createInputState<SimpleInputState>("mainMenu");
        inputState_->setHandler(GUIManager::getInstancePtr());
        inputState_->setJoyStickHandler(&InputManager::EMPTY_HANDLER);

        // create an empty Scene
        this->scene_ = new Scene(0);
        // and a Camera
        this->camera_ = this->scene_->getSceneManager()->createCamera("mainMenu/Camera");

        // show main menu
        GUIManager::getInstance().showGUI("mainmenu");
        GUIManager::getInstance().setCamera(this->camera_);
        GraphicsManager::getInstance().setCamera(this->camera_);

        {
            FunctorMember<GSMainMenu>* functor = createFunctor(&GSMainMenu::startGame);
            functor->setObject(this);
            this->ccStartGame_ = createConsoleCommand(functor, "startGame");
            CommandExecutor::addConsoleCommandShortcut(this->ccStartGame_);
        }

        InputManager::getInstance().requestEnterState("mainMenu");
    }

    void GSMainMenu::deactivate()
    {
        InputManager::getInstance().requestLeaveState("mainMenu");
        InputManager::getInstance().requestDestroyState("mainMenu");

        GUIManager::getInstance().setCamera(0);
        GraphicsManager::getInstance().setCamera(0);
        this->scene_->getSceneManager()->destroyCamera(this->camera_);
        delete this->scene_;

/*
        if (this->ccStartGame_)
        {
            delete this->ccStartGame_;
            this->ccStartGame_ = 0;
        }
*/
    }

    void GSMainMenu::update(const Clock& time)
    {
    }

    void GSMainMenu::startGame()
    {
        // HACK - HACK
        Game::getInstance().popState();
        Game::getInstance().requestStates("standalone, level");
    }
}
