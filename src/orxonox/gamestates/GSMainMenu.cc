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

#include "core/config/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/Game.h"
#include "core/GraphicsManager.h"
#include "core/GUIManager.h"
#include "core/command/ConsoleCommand.h"
#include "core/input/KeyBinderManager.h"
#include "network/Client.h"
#include "util/StringUtils.h"
#include "LevelManager.h"
#include "Scene.h"
#include "sound/AmbientSound.h"
// HACK
#include "core/input/InputManager.h"
#include "core/input/InputState.h"

namespace orxonox
{
    DeclareGameState(GSMainMenu, "mainMenu", false, true);

    static const std::string __CC_startStandalone_name      = "startGame";
    static const std::string __CC_startServer_name          = "startServer";
    static const std::string __CC_startClient_name          = "startClient";
    static const std::string __CC_startDedicated_name       = "startDedicated";
    static const std::string __CC_setMainMenuSoundPath_name = "setMMSoundPath";

    SetConsoleCommand(__CC_startStandalone_name,      &GSMainMenu::startStandalone).defaultValues("").deactivate();
    SetConsoleCommand(__CC_startServer_name,          &GSMainMenu::startServer    ).defaultValues("").deactivate();
    SetConsoleCommand(__CC_startClient_name,          &GSMainMenu::startClient    ).defaultValues("").deactivate();
    SetConsoleCommand(__CC_startDedicated_name,       &GSMainMenu::startDedicated ).defaultValues("").deactivate();
    SetConsoleCommand(__CC_setMainMenuSoundPath_name, &GSMainMenu::setMainMenuSoundPath).hide();

    GSMainMenu::GSMainMenu(const GameStateInfo& info)
        : GameState(info)
    {
        RegisterObject(GSMainMenu);

        InputManager::getInstance().createInputState("MainMenuHackery", true, true)->setKeyHandler(KeyBinderManager::getInstance().getDefaultAsHandler());

        // create an empty Scene
        this->scene_ = new Scene(NULL);
        this->scene_->setSyncMode( 0x0 );
        // and a Camera
        this->camera_ = this->scene_->getSceneManager()->createCamera("mainMenu/Camera");
        if (GameMode::playsSound())
        {
            // Load sound
            this->ambient_ = new AmbientSound();
        }
    }

    GSMainMenu::~GSMainMenu()
    {
        if (GameMode::playsSound())
            this->ambient_->destroy();

        InputManager::getInstance().destroyState("MainMenuHackery");

        this->scene_->getSceneManager()->destroyCamera(this->camera_);
        this->scene_->destroy();
    }

    void GSMainMenu::activate()
    {
        orxout(user_status) << "Loading main menu" << endl;

        // show main menu
        GraphicsManager::getInstance().setCamera(this->camera_);
        GUIManager::getInstance().showGUI("MainMenu", true);
        GUIManager::getInstance().setBackgroundImage("MainMenuBackground", "Background");

        InputManager::getInstance().enterState("MainMenuHackery");

        ModifyConsoleCommand(__CC_startStandalone_name).activate();
        ModifyConsoleCommand(__CC_startServer_name    ).activate();
        ModifyConsoleCommand(__CC_startClient_name    ).activate();
        ModifyConsoleCommand(__CC_startDedicated_name ).activate();
        ModifyConsoleCommand(__CC_setMainMenuSoundPath_name).setObject(this);

        KeyBinderManager::getInstance().setToDefault();

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

        InputManager::getInstance().leaveState("MainMenuHackery");

        GraphicsManager::getInstance().setCamera(0);
        GUIManager::getInstance().setBackgroundImage("");
        GUIManager::hideGUI("MainMenu");

        ModifyConsoleCommand(__CC_startStandalone_name).deactivate();
        ModifyConsoleCommand(__CC_startServer_name    ).deactivate();
        ModifyConsoleCommand(__CC_startClient_name    ).deactivate();
        ModifyConsoleCommand(__CC_startDedicated_name ).deactivate();
        ModifyConsoleCommand(__CC_setMainMenuSoundPath_name).setObject(0);
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

    /**
    @brief
        Start a level in standalone mode.
    @param level
        The filename of the level to be started. If empty, the default level is started.
    */
    void GSMainMenu::startStandalone(const std::string& level)
    {
        if(level != "")
            LevelManager::getInstance().setDefaultLevel(level);

        // HACK
        Game::getInstance().popState();
        Game::getInstance().requestStates("standalone, level");
    }

    /**
    @brief
        Start a level in server mode.
    @param level
        The filename of the level to be started. If empty, the default level is started.
    */
    void GSMainMenu::startServer(const std::string& level)
    {
        if(level != "")
            LevelManager::getInstance().setDefaultLevel(level);

        // HACK
        Game::getInstance().popState();
        Game::getInstance().requestStates("server, level");
    }

    /**
    @brief
        Connect to a game as client.
    @param destination
        The destination to connect to. If empty, the client connects to the default destination.
    */
    void GSMainMenu::startClient(const std::string& destination)
    {
        if(destination != "")
            Client::getInstance()->setDestination(destination, NETWORK_PORT);

        // HACK
        Game::getInstance().popState();
        Game::getInstance().requestStates("client, level");
    }

    /**
    @brief
        Start a level in dedicated mode.
    @param level
        The filename of the level to be started. If empty, the default level is started.
    */
    void GSMainMenu::startDedicated(const std::string& level)
    {
        if(level != "")
            LevelManager::getInstance().setDefaultLevel(level);

        // HACK
        Game::getInstance().popState();
        Game::getInstance().popState();
        Game::getInstance().requestStates("server, level");
    }

}
