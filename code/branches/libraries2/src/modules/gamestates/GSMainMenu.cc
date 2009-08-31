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

#include "GSMainMenu.h"

#include <OgreSceneManager.h>

#include "core/input/InputManager.h"
#include "core/input/InputState.h"
#include "core/Game.h"
#include "core/Clock.h"
#include "core/ConsoleCommand.h"
#include "core/GraphicsManager.h"
#include "core/GUIManager.h"
#include "objects/Scene.h"
#include "sound/SoundMainMenu.h"

namespace orxonox
{
    DeclareGameState(GSMainMenu, "mainMenu", false, true);

    GSMainMenu::GSMainMenu(const GameStateInfo& info)
        : GameState(info)
        , inputState_(0)
    {
        inputState_ = InputManager::getInstance().createInputState("mainMenu");
        inputState_->setHandler(GUIManager::getInstancePtr());
        inputState_->setJoyStickHandler(&InputHandler::EMPTY);
        inputState_->setIsExclusiveMouse(false);

        // create an empty Scene
        this->scene_ = new Scene(0);
        // and a Camera
        this->camera_ = this->scene_->getSceneManager()->createCamera("mainMenu/Camera");
    }

    GSMainMenu::~GSMainMenu()
    {
        InputManager::getInstance().destroyState("mainMenu");

        this->scene_->getSceneManager()->destroyCamera(this->camera_);
        delete this->scene_;
    }

    void GSMainMenu::activate()
    {
        // show main menu
        GUIManager::getInstance().showGUI("MainMenu");
        GUIManager::getInstance().setCamera(this->camera_);
        GraphicsManager::getInstance().setCamera(this->camera_);

        {
            FunctorMember<GSMainMenu>* functor = createFunctor(&GSMainMenu::startStandalone);
            functor->setObject(this);
            this->ccStartStandalone_ = createConsoleCommand(functor, "startGame");
            CommandExecutor::addConsoleCommandShortcut(this->ccStartStandalone_);
        }
        {
            FunctorMember<GSMainMenu>* functor = createFunctor(&GSMainMenu::startServer);
            functor->setObject(this);
            this->ccStartServer_ = createConsoleCommand(functor, "startServer");
            CommandExecutor::addConsoleCommandShortcut(this->ccStartServer_);
        }
        {
            FunctorMember<GSMainMenu>* functor = createFunctor(&GSMainMenu::startClient);
            functor->setObject(this);
            this->ccStartClient_ = createConsoleCommand(functor, "startClient");
            CommandExecutor::addConsoleCommandShortcut(this->ccStartClient_);
        }
        {
            FunctorMember<GSMainMenu>* functor = createFunctor(&GSMainMenu::startDedicated);
            functor->setObject(this);
            this->ccStartDedicated_ = createConsoleCommand(functor, "startDedicated");
            CommandExecutor::addConsoleCommandShortcut(this->ccStartDedicated_);
        }

        InputManager::getInstance().enterState("mainMenu");

        this->ambient_ = new SoundMainMenu();
        this->ambient_->play(true);
    }

    void GSMainMenu::deactivate()
    {
        delete this->ambient_;

        InputManager::getInstance().leaveState("mainMenu");

        GUIManager::getInstance().setCamera(0);
        GraphicsManager::getInstance().setCamera(0);

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

    void GSMainMenu::startStandalone()
    {
        // HACK - HACK
        Game::getInstance().popState();
        Game::getInstance().requestStates("standalone, level");
    }

    void GSMainMenu::startServer()
    {
        // HACK - HACK
        Game::getInstance().popState();
        Game::getInstance().requestStates("server, level");
    }

    void GSMainMenu::startClient()
    {
        // HACK - HACK
        Game::getInstance().popState();
        Game::getInstance().requestStates("client, level");
    }

    void GSMainMenu::startDedicated()
    {
        // HACK - HACK
        Game::getInstance().popState();
        Game::getInstance().popState();
        Game::getInstance().requestStates("dedicated, level");
    }
}
