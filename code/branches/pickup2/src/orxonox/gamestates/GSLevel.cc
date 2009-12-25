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

#include <OgreCompositorManager.h>

#include "util/Clock.h"
#include "core/input/InputManager.h"
#include "core/input/InputState.h"
#include "core/input/KeyBinderManager.h"
#include "core/ConsoleCommand.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "core/GUIManager.h"
#include "core/Loader.h"
#include "core/XMLFile.h"

#include "LevelManager.h"
#include "PlayerManager.h"

namespace orxonox
{
    DeclareGameState(GSLevel, "level", false, false);

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
        if (GameMode::showsGraphics())
        {
            gameInputState_ = InputManager::getInstance().createInputState("game");
            gameInputState_->setMouseMode(MouseMode::Exclusive);
            gameInputState_->setHandler(KeyBinderManager::getInstance().getDefaultAsHandler());
            KeyBinderManager::getInstance().setToDefault();

            guiMouseOnlyInputState_ = InputManager::getInstance().createInputState("guiMouseOnly");
            guiMouseOnlyInputState_->setMouseMode(MouseMode::Exclusive);
            guiMouseOnlyInputState_->setMouseHandler(GUIManager::getInstancePtr());

            guiKeysOnlyInputState_ = InputManager::getInstance().createInputState("guiKeysOnly");
            guiKeysOnlyInputState_->setKeyHandler(GUIManager::getInstancePtr());
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
        }
    }

    void GSLevel::deactivate()
    {
        if (GameMode::showsGraphics())
        {
            // unload all compositors (this is only necessary because we don't yet destroy all resources!)
            Ogre::CompositorManager::getSingleton().removeAll();

            InputManager::getInstance().leaveState("game");
        }

        // disconnect all HumanPlayers
        PlayerManager::getInstance().disconnectAllClients();

        if (GameMode::isMaster())
            this->unloadLevel();

        if (GameMode::showsGraphics())
        {
            gameInputState_->setHandler(0);
            guiMouseOnlyInputState_->setHandler(0);
            guiKeysOnlyInputState_->setHandler(0);
            InputManager::getInstance().destroyState("game");
            InputManager::getInstance().destroyState("guiKeysOnly");
            InputManager::getInstance().destroyState("guiMouseOnly");
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
        COUT(0) << "Loading level..." << std::endl;
        startFile_ = new XMLFile(LevelManager::getInstance().getDefaultLevel());
        Loader::open(startFile_);
    }

    void GSLevel::unloadLevel()
    {
        Loader::unload(startFile_);
        delete startFile_;

        COUT(3) << "Unloaded level. Remaining objects:" << std::endl;
        unsigned int i = 0;
        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it != ObjectList<BaseObject>::end(); ++it)
        {
            std::set<BaseObject*>::const_iterator find = this->staticObjects_.find(*it);
            if (find == this->staticObjects_.end())
            {
                COUT(3) << ++i << ": " << it->getIdentifier()->getName() << " (" << *it << ')' << std::endl;
            }
        }
        COUT(3) << i << " objects remaining.";
        if (i == 0)
            COUT(3) << " Well done!" << std::endl;
        else
            COUT(3) << " Try harder!" << std::endl;
    }
}
