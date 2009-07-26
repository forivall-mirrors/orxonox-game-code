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

#include "GSRoot.h"

#include "core/Clock.h"
#include "core/CommandLine.h"
#include "core/ConsoleCommand.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "core/LuaBind.h"
#include "network/NetworkFunction.h"
#include "ToluaBindCore.h"
#include "ToluaBindOrxonox.h"
#include "tools/Timer.h"
#include "interfaces/TimeFactorListener.h"
#include "interfaces/Tickable.h"
#include "LevelManager.h"

namespace orxonox
{
    DeclareGameState(GSRoot, "root", false, false);
    SetCommandLineSwitch(console).information("Start in console mode (text IO only)");
    // Shortcuts for easy direct loading
    SetCommandLineSwitch(server).information("Start in server mode");
    SetCommandLineSwitch(client).information("Start in client mode");
    SetCommandLineSwitch(dedicated).information("Start in dedicated server mode");
    SetCommandLineSwitch(standalone).information("Start in standalone mode");

    GSRoot::GSRoot(const GameStateInfo& info)
        : GameState(info)
        , timeFactor_(1.0f)
        , bPaused_(false)
        , timeFactorPauseBackup_(1.0f)
    {
        this->ccSetTimeFactor_ = 0;
        this->ccPause_ = 0;

        // Tell LuaBind about all tolua interfaces
        LuaBind::getInstance().addToluaInterface(&tolua_Core_open, "Core");
        LuaBind::getInstance().addToluaInterface(&tolua_Orxonox_open, "Orxonox");
    }

    GSRoot::~GSRoot()
    {
        NetworkFunctionBase::destroyAllNetworkFunctions();
    }

    void GSRoot::activate()
    {
        // reset game speed to normal
        this->timeFactor_ = 1.0f;

        {
            // time factor console command
            FunctorMember<GSRoot>* functor = createFunctor(&GSRoot::setTimeFactor);
            functor->setObject(this);
            this->ccSetTimeFactor_ = createConsoleCommand(functor, "setTimeFactor");
            CommandExecutor::addConsoleCommandShortcut(this->ccSetTimeFactor_).accessLevel(AccessLevel::Offline).defaultValue(0, 1.0);
        }

        {
            // time factor console command
            FunctorMember<GSRoot>* functor = createFunctor(&GSRoot::pause);
            functor->setObject(this);
            this->ccPause_ = createConsoleCommand(functor, "pause");
            CommandExecutor::addConsoleCommandShortcut(this->ccPause_).accessLevel(AccessLevel::Offline);
        }

        // create the global LevelManager
        this->levelManager_ = new LevelManager();

        // Load level directly?
        bool loadLevel = false;
        if (CommandLine::getValue("standalone").getBool())
        {
            Game::getInstance().requestStates("graphics, standalone, level");
            loadLevel = true;
        }
        if (CommandLine::getValue("server").getBool())
        {
            Game::getInstance().requestStates("graphics, server, level");
            loadLevel = true;
        }
        if (CommandLine::getValue("client").getBool())
        {
            Game::getInstance().requestStates("graphics, client, level");
            loadLevel = true;
        }
        if (CommandLine::getValue("dedicated").getBool())
        {
            Game::getInstance().requestStates("dedicated, level");
            loadLevel = true;
        }
        
        // Determine where to start otherwise
        if (!loadLevel && !CommandLine::getValue("console").getBool())
        {
            // Also load graphics
            Game::getInstance().requestState("graphics");
        }
    }

    void GSRoot::deactivate()
    {
/*
        if (this->ccSetTimeFactor_)
        {
            delete this->ccSetTimeFactor_;
            this->ccSetTimeFactor_ = 0;
        }

        if (this->ccPause_)
        {
            delete this->ccPause_;
            this->ccPause_ = 0;
        }
*/

        delete this->levelManager_;
    }

    void GSRoot::update(const Clock& time)
    {
        if (this->getActivity().topState)
        {
            // This state can not 'survive' on its own.
            // Load a user interface therefore
            Game::getInstance().requestState("ioConsole");
        }

        for (ObjectList<TimerBase>::iterator it = ObjectList<TimerBase>::begin(); it; ++it)
            it->tick(time);

        /*** HACK *** HACK ***/
        // Call the Tickable objects
        float leveldt = time.getDeltaTime();
        if (leveldt > 1.0f)
        {
            // just loaded
            leveldt = 0.0f;
        }
        for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; ++it)
            it->tick(leveldt * this->timeFactor_);
        /*** HACK *** HACK ***/
    }

    /**
    @brief
        Changes the speed of Orxonox
    @remark
        This function is a hack when placed here!
        Timefactor should be related to the scene (level or so), not the game
    */
    void GSRoot::setTimeFactor(float factor)
    {
        if (GameMode::isMaster())
        {
            if (!this->bPaused_)
            {
                TimeFactorListener::timefactor_s = factor;

                for (ObjectList<TimeFactorListener>::iterator it = ObjectList<TimeFactorListener>::begin(); it != ObjectList<TimeFactorListener>::end(); ++it)
                    it->changedTimeFactor(factor, this->timeFactor_);

                this->timeFactor_ = factor;
            }
            else
                this->timeFactorPauseBackup_ = factor;
        }
    }

    void GSRoot::pause()
    {
        if (GameMode::isMaster())
        {
            if (!this->bPaused_)
            {
                this->timeFactorPauseBackup_ = this->timeFactor_;
                this->setTimeFactor(0.0f);
                this->bPaused_ = true;
            }
            else
            {
                this->bPaused_ = false;
                this->setTimeFactor(this->timeFactorPauseBackup_);
            }
        }
    }
}
