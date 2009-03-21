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

/**
@file
@brief
    Implementation of the Game class.
*/

#include "OrxonoxStableHeaders.h"
#include "Game.h"

#include <exception>
#include <cassert>

#include "util/Debug.h"
#include "util/Exception.h"
#include "core/CommandLine.h"
#include "core/ConsoleCommand.h"
#include "core/Core.h"
#include "core/Identifier.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"

#include "gamestates/GSRoot.h"
#include "gamestates/GSGraphics.h"
#include "gamestates/GSStandalone.h"
#include "gamestates/GSServer.h"
#include "gamestates/GSClient.h"
#include "gamestates/GSDedicated.h"
#include "gamestates/GSGUI.h"
#include "gamestates/GSIOConsole.h"

/*
@brief
    Main method. Game starts here (except for static initialisations).
*/
int main(int argc, char** argv)
{
    {
        orxonox::Game orxonox(argc, argv);
        orxonox.run();
        // objects gets destroyed here!
    }

    // Clean up class hierarchy stuff (identifiers, xmlport, configvalue, consolecommand)
    // Needs to be done after Game destructor because of ~OrxonoxClass
    orxonox::Identifier::destroyAllIdentifiers();

    return 0;
}

namespace orxonox
{
    void stop_game()
    {
        Game::getInstance().stop();
    }

    SetCommandLineArgument(state, "gui").shortcut("s");
    SetConsoleCommandShortcutExternAlias(stop_game, "exit");

    Game* Game::singletonRef_s = 0;

    /**
    @brief
        Non-initialising constructor.
    */
    Game::Game(int argc, char** argv)
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;

        this->abort_ = false;

        // reset statistics
        this->statisticsStartTime_ = 0;
        this->statisticsTickTimes_.clear();
        this->periodTickTime_ = 0;
        this->periodTime_ = 0;
        this->avgFPS_ = 0.0f;
        this->avgTickTime_ = 0.0f;

        this->core_ = new orxonox::Core();
        this->gameClock_ = this->core_->initialise(argc, argv);

        RegisterRootObject(Game);
        this->setConfigValues();
    }

    /**
    @brief
    */
    Game::~Game()
    {
        // Destroy pretty much everyhting left
        delete this->core_;

        assert(singletonRef_s);
        singletonRef_s = 0;
    }

    void Game::setConfigValues()
    {
        SetConfigValue(statisticsRefreshCycle_, 250000)
            .description("Sets the time in microseconds interval at which average fps, etc. get updated.");
        SetConfigValue(statisticsAvgLength_, 1000000)
            .description("Sets the time in microseconds interval at which average fps, etc. gets calculated.");
    }

    /**
    @brief
        Main loop of the orxonox game.
    @note
        We use the Ogre::Timer to measure time since it uses the most precise
        method an any platform (however the windows timer lacks time when under
        heavy kernel load!).
    */
    void Game::run()
    {
        // create the gamestates
        GSRoot root;
        GSGraphics graphics;
        GSStandalone standalone;
        GSServer server;
        GSClient client;
        GSDedicated dedicated;
        GSGUI gui;
        GSIOConsole ioConsole;

        // make the hierarchy
        root.addChild(&graphics);
        graphics.addChild(&standalone);
        graphics.addChild(&server);
        graphics.addChild(&client);
        graphics.addChild(&gui);
        root.addChild(&ioConsole);
        root.addChild(&dedicated);

        root.activate();

        // get initial state from command line
        root.gotoState(CommandLine::getValue("state"));

        this->gameClock_->capture(); // first delta time should be about 0 seconds
        while (!this->abort_)
        {
            this->gameClock_->capture();
            uint64_t currentTime = this->gameClock_->getMicroseconds();

            // STATISTICS
            statisticsTickInfo tickInfo = {currentTime, 0};
            statisticsTickTimes_.push_back(tickInfo);
            this->periodTime_ += this->gameClock_->getDeltaTimeMicroseconds();

            // UPDATE
            root.tick(*this->gameClock_);

            // STATISTICS
            if (this->periodTime_ > statisticsRefreshCycle_)
            {
                std::list<statisticsTickInfo>::iterator it = this->statisticsTickTimes_.begin();
                assert(it != this->statisticsTickTimes_.end());
                int64_t lastTime = currentTime - this->statisticsAvgLength_;
                if ((int64_t)it->tickTime < lastTime)
                {
                    do
                    {
                        assert(this->periodTickTime_ > it->tickLength);
                        this->periodTickTime_ -= it->tickLength;
                        ++it;
                        assert(it != this->statisticsTickTimes_.end());
                    } while ((int64_t)it->tickTime < lastTime);
                    this->statisticsTickTimes_.erase(this->statisticsTickTimes_.begin(), it);
                }

                uint32_t framesPerPeriod = this->statisticsTickTimes_.size();
                this->avgFPS_ = (float)framesPerPeriod / (currentTime - this->statisticsTickTimes_.front().tickTime) * 1000000.0;
                this->avgTickTime_ = (float)this->periodTickTime_ / framesPerPeriod / 1000.0;

                this->periodTime_ -= this->statisticsRefreshCycle_;
            }

            if (root.stateRequest_ != "")
                root.gotoState(root.stateRequest_);
        }

        root.gotoState("root");
        root.deactivate();
    }

    void Game::stop()
    {
        this->abort_ = true;
    }

    void Game::addTickTime(uint32_t length)
    {
        assert(!this->statisticsTickTimes_.empty());
        this->statisticsTickTimes_.back().tickLength += length;
        this->periodTickTime_+=length;
    }
}
