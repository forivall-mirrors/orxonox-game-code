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
 *      Fabian 'x3n' Landau
 *      Benjamin Knecht
 *
 */

#include "GSLevel.h"
#include "GSLevelMemento.h"

#include <OgreCompositorManager.h>

#include "util/Clock.h"
#include "core/input/InputManager.h"
#include "core/input/InputState.h"
#include "core/input/KeyBinderManager.h"
#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "core/GUIManager.h"
#include "core/Loader.h"
#include "core/XMLFile.h"
#include "core/command/ConsoleCommandIncludes.h"

#include "LevelManager.h"
#include "PlayerManager.h"
#include "GSRoot.h"

namespace orxonox
{
    DeclareGameState(GSLevel, "level", false, false);

    static const std::string __CC_startMainMenu_name = "startMainMenu";
    static const std::string __CC_changeGame_name = "changeGame";
    static const std::string __CC_reloadLevel_name = "reloadLevel";

    SetConsoleCommand(__CC_startMainMenu_name, &GSLevel::startMainMenu).deactivate();
    SetConsoleCommand(__CC_changeGame_name, &GSLevel::changeGame).defaultValues("").deactivate();
    SetConsoleCommand(__CC_reloadLevel_name, &GSLevel::reloadLevel).deactivate();

    GSLevel::GSLevel(const GameStateInfo& info)
        : GameState(info)
        , gameInputState_(0)
        , guiMouseOnlyInputState_(0)
        , guiKeysOnlyInputState_(0)
        , startFile_(0)
        , bShowIngameGUI_(false)
    {
    }

    GSLevel::~GSLevel()
    {
    }

    void GSLevel::activate()
    {
        orxout(user_status) << "Loading level" << endl;

        if (GameMode::showsGraphics())
        {
            gameInputState_ = InputManager::getInstance().createInputState("game");
            gameInputState_->setMouseExclusive(true);
            gameInputState_->setHandler(KeyBinderManager::getInstance().getDefaultAsHandler());
            KeyBinderManager::getInstance().setToDefault();

            guiMouseOnlyInputState_ = InputManager::getInstance().createInputState("guiMouseOnly");
            guiMouseOnlyInputState_->setMouseExclusive(true);
            guiMouseOnlyInputState_->setMouseHandler(&GUIManager::getInstance());

            guiKeysOnlyInputState_ = InputManager::getInstance().createInputState("guiKeysOnly");
            guiKeysOnlyInputState_->setKeyHandler(&GUIManager::getInstance());
        }

        if (GameMode::isMaster())
        {
            this->loadLevel();
        }

        if (GameMode::showsGraphics())
        {
            // level is loaded: we can start capturing the input
            InputManager::getInstance().enterState("game");

            // connect the HumanPlayer to the game
            PlayerManager::getInstance().clientConnected(0);

            ModifyConsoleCommand(__CC_startMainMenu_name).activate();
        }

        if (GameMode::isStandalone())
        {
            ModifyConsoleCommand(__CC_changeGame_name).activate();
            ModifyConsoleCommand(__CC_reloadLevel_name).setObject(this).activate();
        }
    }

    void GSLevel::deactivate()
    {
        if (GameMode::showsGraphics())
            InputManager::getInstance().leaveState("game");

        // disconnect all HumanPlayers
        PlayerManager::getInstance().disconnectAllClients();

        if (GameMode::isMaster())
            this->unloadLevel();

        if (GameMode::showsGraphics())
        {
#if OGRE_VERSION < 0x010700
            // unload all compositors (this is only necessary because we don't yet destroy all resources!)
            Ogre::CompositorManager::getSingleton().removeAll();
#endif

            gameInputState_->setHandler(0);
            guiMouseOnlyInputState_->setHandler(0);
            guiKeysOnlyInputState_->setHandler(0);
            InputManager::getInstance().destroyState("game");
            InputManager::getInstance().destroyState("guiKeysOnly");
            InputManager::getInstance().destroyState("guiMouseOnly");

            ModifyConsoleCommand(__CC_startMainMenu_name  ).deactivate();
        }

        if (GameMode::isStandalone())
        {
            ModifyConsoleCommand(__CC_changeGame_name).deactivate();
            ModifyConsoleCommand(__CC_reloadLevel_name).setObject(NULL).deactivate();
        }
    }

    void GSLevel::update(const Clock& time)
    {
        // Note: Temporarily moved to GSRoot.
        //// Call the scene objects
        //for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; ++it)
        //    it->tick(time.getDeltaTime() * this->timeFactor_);
    }

    void GSLevel::loadLevel()
    {
        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it != ObjectList<BaseObject>::end(); ++it)
            this->staticObjects_.insert(*it);

        // call the loader
        startFile_ = new XMLFile(LevelManager::getInstance().getDefaultLevel());
        bool loaded = Loader::open(startFile_);

        Core::getInstance().updateLastLevelTimestamp();
        if(!loaded)
            GSRoot::delayedStartMainMenu();
    }

    void GSLevel::unloadLevel()
    {
        Loader::unload(startFile_);
        delete startFile_;

        orxout(internal_info) << "Remaining objects:" << endl;
        unsigned int i = 0;
        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it != ObjectList<BaseObject>::end(); ++it)
        {
            std::set<BaseObject*>::const_iterator find = this->staticObjects_.find(*it);
            if (find == this->staticObjects_.end())
            {
                orxout(internal_info) << ++i << ": " << it->getIdentifier()->getName() << " (" << *it << "), references: " << it->getReferenceCount() << endl;
            }
        }
        if (i == 0)
            orxout(internal_info) << i << " objects remaining. Well done!" << endl;
        else
            orxout(internal_warning) << i << " objects remaining. Try harder!" << endl;
    }

    void GSLevel::reloadLevel()
    {
        // export all states
        std::vector<GSLevelMementoState*> states;
        for (ObjectList<GSLevelMemento>::iterator it = ObjectList<GSLevelMemento>::begin(); it != ObjectList<GSLevelMemento>::end(); ++it)
        {
            GSLevelMementoState* state = it->exportMementoState();
            if (state)
                states.push_back(state);
        }

        // reload level (or better: reload the whole gamestate)
        this->deactivate();
        this->activate();

        // import all states
        for (ObjectList<GSLevelMemento>::iterator it = ObjectList<GSLevelMemento>::begin(); it != ObjectList<GSLevelMemento>::end(); ++it)
            it->importMementoState(states);

        // delete states
        for (size_t i = 0; i < states.size(); ++i)
            delete states[i];
    }

    /**
    @brief
        Starts the MainMenu.
    */
    /*static*/ void GSLevel::startMainMenu(void)
    {
        // HACK
        Game::getInstance().popState();
        Game::getInstance().popState();
    }

    /**
    @brief
        Terminates the current game and starts a new game.
    @param level
        The filename of the level to be started.
    */
    /*static*/ void GSLevel::changeGame(const std::string& level)
    {
        if(level != "")
            LevelManager::getInstance().setDefaultLevel(level);

        // HACK
        Game::getInstance().popState();
        Game::getInstance().popState();
        Game::getInstance().requestStates("standalone, level");
    }



    ///////////////////////////////////////////////////////////////////////////

    RegisterAbstractClass(GSLevelMemento).inheritsFrom<OrxonoxInterface>();

    GSLevelMemento::GSLevelMemento()
    {
        RegisterObject(GSLevelMemento);
    }
}
