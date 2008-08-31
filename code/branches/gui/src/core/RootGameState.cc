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

#include "RootGameState.h"

#include "Clock.h"
#include "Debug.h"
#include "Exception.h"
#include "CommandLine.h"

namespace orxonox
{
    SetCommandLineArgument(state, "gui").setShortcut("s");

    RootGameState::RootGameState(const std::string& name)
        : GameState<GameStateBase>(name)
        , stateRequest_("")
    {
    }

    RootGameState::~RootGameState()
    {
    }

    /**
    @brief
        Internal method that actually makes the state transition. Since it is internal,
        the method can assume certain things to be granted (like 'this' is always active).
    */
    void RootGameState::makeTransition(GameStateBase* source, GameStateBase* destination)
    {
        if (source != 0)
        {
            // transition was not initiated by root itself
            this->activeChild_ = 0;
        }

        if (destination == this)
        {
            // this marks the end of the game.
            return;
        }

        // Check for 'destination' in the children map first
        std::map<GameStateBase*, GameStateBase*>::const_iterator it
            = this->grandchildrenToChildren_.find(destination);
        if (it != this->grandchildrenToChildren_.end())
        {
            OrxAssert(dynamic_cast<GameStateBase*>(it->second) != 0,
                "There was a mix with RootGameState and GameState, could not cast.");
            GameStateBase* child = static_cast<GameStateBase*>(it->second);
            // child state. Don't use 'state', might be a grandchild!
            this->activeChild_ = child;
            child->makeTransition(this, destination);
        }
        else
        {
            // root doesn't have a parent..
            OrxAssert(false, "GameState '" + destination->getName() + "' not found in children list of Root.");
        }
    }

    void RootGameState::gotoState(const std::string& name)
    {
        GameStateBase* request = getState(name);
        if (request)
        {
            GameStateBase* current = getCurrentState();
            if (current)
            {
                current->makeTransition(0, request);
            }
            else
            {
                // Root is not yet active. This is a violation.
                ThrowException(GameState, "Activate Root before requesting a state.");
            }
        }
        else
        {
            COUT(2) << "Warning: GameState '" << name << "' doesn't exist." << std::endl;
        }
    }

    /**
    @brief
        Makes a state transition according to the state tree. You can choose any state
        in the tree to do the call. The function finds the current state on its own.
    @param state
        The state to be entered, has to exist in the tree.
    */
    void RootGameState::requestState(const std::string& name)
    {
        this->stateRequest_ = name;
    }

    /**
    @brief
        Starts the game. The little 'while' denotes the main loop.
        Whenever the root state is selected, the game ends.
    @param name
        State to start with (usually main menu or specified by command line)
    */
    void RootGameState::start(int argc, char** argv)
    {
        parseCommandLine(argc, argv);

        this->activate();

        // get initial state from command line
        std::string initialState;
        CommandLine::getValue<std::string>("state", &initialState);
        gotoState(initialState);

        Clock clock;
        while (this->activeChild_)
        {
            clock.capture();

            this->tick(clock);

            if (this->stateRequest_ != "")
                gotoState(stateRequest_);
        }

        this->deactivate();
    }

    void RootGameState::parseCommandLine(int argc, char** argv)
    {
        std::vector<std::string> args;
        for (int i = 1; i < argc; ++i)
            args.push_back(argv[i]);

        try
        {
            orxonox::CommandLine::parse(args);
        }
        catch (orxonox::ArgumentException& ex)
        {
            COUT(1) << ex.what() << std::endl;
            COUT(0) << "Usage:" << std::endl << "orxonox " << CommandLine::getUsageInformation() << std::endl;
        }
    }
}
