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
#include "core/input/KeyBinderManager.h"
#include "core/Game.h"
#include "core/ConsoleCommand.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/GraphicsManager.h"
#include "core/GUIManager.h"
#include "Scene.h"
#include "sound/AmbientSound.h"

namespace orxonox
{
    DeclareGameState(GSMainMenu, "mainMenu", false, true);

    GSMainMenu::GSMainMenu(const GameStateInfo& info)
        : GameState(info)
        , inputState_(0)
    {
        RegisterRootObject(GSMainMenu);
        inputState_ = InputManager::getInstance().createInputState("mainMenu");
        inputState_->setMouseMode(MouseMode::Nonexclusive);
        inputState_->setHandler(GUIManager::getInstancePtr());
        inputState_->setKeyHandler(KeyBinderManager::getInstance().getDefaultAsHandler());
        inputState_->setJoyStickHandler(&InputHandler::EMPTY);

        // create an empty Scene
        this->scene_ = new Scene(NULL);
        this->scene_->setSyncMode( 0x0 );
        // and a Camera
        this->camera_ = this->scene_->getSceneManager()->createCamera("mainMenu/Camera");
        if (GameMode::playsSound())
        {
            // Load sound
            this->ambient_ = new AmbientSound(0);
            this->ambient_->setSyncMode(0x0);
        }
    }

    GSMainMenu::~GSMainMenu()
    {
        if (GameMode::playsSound())
            this->ambient_->destroy();

        InputManager::getInstance().destroyState("mainMenu");

        this->scene_->getSceneManager()->destroyCamera(this->camera_);
        this->scene_->destroy();
    }

    void GSMainMenu::activate()
    {
        // show main menu
        GUIManager::getInstance().showGUI("MainMenu", true, GraphicsManager::getInstance().isFullScreen());
        GUIManager::getInstance().setCamera(this->camera_);
        GUIManager::getInstance().setBackground("MainMenuBackground");
//         GUIManager::getInstance().setBackground("");
        GraphicsManager::getInstance().setCamera(this->camera_);

        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(createFunctor(&GSMainMenu::startStandalone), "startGame"));
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(createFunctor(&GSMainMenu::startServer), "startServer"));
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(createFunctor(&GSMainMenu::startClient), "startClient"));
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(createFunctor(&GSMainMenu::startDedicated), "startDedicated"));
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(createFunctor(&GSMainMenu::startMainMenu), "startMainMenu"));

        // create command to change sound path
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(createFunctor(&GSMainMenu::setMainMenuSoundPath, this), "setMMSoundPath"));

        KeyBinderManager::getInstance().setToDefault();
        InputManager::getInstance().enterState("mainMenu");

        this->setConfigValues();

        if (GameMode::playsSound())
        {
            this->ambient_->setLooping(true);
            this->ambient_->play(); // works without source
        }
    }

    void GSMainMenu::deactivate()
    {
        if (GameMode::playsSound())
        {
            this->ambient_->stop();
        }

        InputManager::getInstance().leaveState("mainMenu");

        GUIManager::getInstance().setCamera(0);
        GUIManager::getInstance().setBackground("");
        GUIManager::hideGUI("MainMenu");
        GraphicsManager::getInstance().setCamera(0);
    }

    void GSMainMenu::update(const Clock& time)
    {
    }

    void GSMainMenu::setConfigValues()
    {
        SetConfigValue(soundPathMain_, "mainmenu.ogg")
            .description("Contains the path to the main menu sound file.")
            .callback(this, &GSMainMenu::reloadSound);
    }

    void GSMainMenu::reloadSound()
    {
        if (GameMode::playsSound())
        {
            this->ambient_->setAmbientSource(soundPathMain_);
        }
    }

    const std::string& GSMainMenu::getMainMenuSoundPath()
    {
        return soundPathMain_;
    }

    void GSMainMenu::setMainMenuSoundPath(const std::string& path)
    {
        ModifyConfigValue(soundPathMain_, set, path);
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
        Game::getInstance().requestStates("server, level");
    }
    void GSMainMenu::startMainMenu()
    {
        // HACK - HACK
        Game::getInstance().popState();
        Game::getInstance().popState();
        Game::getInstance().requestStates("mainmenu");
    }
}
