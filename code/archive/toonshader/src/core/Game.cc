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

#include "Game.h"

#include <exception>
#include <cassert>

#include "util/Debug.h"
#include "util/Exception.h"
#include "util/SubString.h"
#include "Clock.h"
#include "CommandLine.h"
#include "ConsoleCommand.h"
#include "Core.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"
#include "GameState.h"

namespace orxonox
{
    static void stop_game()
        { Game::getInstance().stop(); }
    SetConsoleCommandShortcutExternAlias(stop_game, "exit");

    struct _CoreExport GameStateTreeNode
    {
        GameState*                      state_;
        GameStateTreeNode*              parent_;
        std::vector<GameStateTreeNode*> children_;
    };

    std::map<std::string, GameState*> Game::allStates_s;
    Game* Game::singletonRef_s = 0;

    /**
    @brief
        Non-initialising constructor.
    */
    Game::Game(int argc, char** argv)
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;

        this->rootStateNode_ = 0;
        this->activeStateNode_ = 0;

        this->abort_ = false;

        // reset statistics
        this->statisticsStartTime_ = 0;
        this->statisticsTickTimes_.clear();
        this->periodTickTime_ = 0;
        this->periodTime_ = 0;
        this->avgFPS_ = 0.0f;
        this->avgTickTime_ = 0.0f;


        // Set up a basic clock to keep time
        this->gameClock_ = new Clock();

        this->core_ = new orxonox::Core();
        this->core_->initialise(argc, argv);

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

        // Delete all the created nodes
        for (std::vector<GameStateTreeNode*>::const_iterator it = this->allStateNodes_.begin(); it != this->allStateNodes_.end(); ++it)
            delete *it;

        delete this->gameClock_;

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
        // Always start with the ROOT state
        this->requestedStateNodes_.push_back(this->rootStateNode_);
        this->activeStateNode_ = this->rootStateNode_;
        this->loadState(this->rootStateNode_->state_);

        // START GAME
        this->gameClock_->capture(); // first delta time should be about 0 seconds
        while (!this->abort_ && !this->activeStates_.empty())
        {
            this->gameClock_->capture();
            uint64_t currentTime = this->gameClock_->getMicroseconds();

            // STATISTICS
            statisticsTickInfo tickInfo = {currentTime, 0};
            statisticsTickTimes_.push_back(tickInfo);
            this->periodTime_ += this->gameClock_->getDeltaTimeMicroseconds();

            // UPDATE STATE STACK
            while (this->requestedStateNodes_.size() > 1)
            {
                // Note: this->requestedStateNodes_.front() is the currently active state node
                std::vector<GameStateTreeNode*>::iterator it = this->requestedStateNodes_.begin() + 1;
                if (*it == this->activeStateNode_->parent_)
                    this->unloadState(this->activeStateNode_->state_);
                else // has to be child
                    this->loadState((*it)->state_);
                this->activeStateNode_ = *it;
                this->requestedStateNodes_.erase(this->requestedStateNodes_.begin());
            }

            // UPDATE, bottom to top in the stack
            this->core_->update(*this->gameClock_);
            for (std::vector<GameState*>::const_iterator it = this->activeStates_.begin();
                it != this->activeStates_.end(); ++it)
                (*it)->update(*this->gameClock_);

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
        }

        // UNLOAD all remaining states
        while (!this->activeStates_.empty())
            this->unloadState(this->activeStates_.back());
        this->activeStateNode_ = 0;
        this->requestedStateNodes_.clear();
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


    /***** GameState related *****/

    void Game::requestState(const std::string& name)
    {
        GameState* state = this->getState(name);
        if (state == NULL || this->activeStateNode_ == NULL)
            return;

        GameStateTreeNode* requestedNode = 0;

        // this->requestedStateNodes_.back() is the currently active state
        GameStateTreeNode* lastRequestedNode = this->requestedStateNodes_.back();

        // Already the active node?
        if (state == lastRequestedNode->state_)
        {
            COUT(2) << "Warning: Requesting the currently active state! Ignoring." << std::endl;
            return;
        }

        // Check children first
        for (unsigned int i = 0; i < lastRequestedNode->children_.size(); ++i)
        {
            if (lastRequestedNode->children_[i]->state_ == state)
            {
                requestedNode = lastRequestedNode->children_[i];
                break;
            }
        }

        // Check parent and all its grand parents
        GameStateTreeNode* currentNode = lastRequestedNode;
        while (requestedNode == NULL && currentNode != NULL)
        {
            if (currentNode->state_ == state)
                requestedNode = currentNode;
            currentNode = currentNode->parent_;
        }

        if (requestedNode == NULL)
            COUT(1) << "Error: Requested GameState transition is not allowed. Ignoring." << std::endl;
        else
            this->requestedStateNodes_.push_back(requestedNode);
    }

    void Game::requestStates(const std::string& names)
    {
        SubString tokens(names, ",;", " ");
        for (unsigned int i = 0; i < tokens.size(); ++i)
            this->requestState(tokens[i]);
    }

    void Game::popState()
    {
        if (this->activeStateNode_ != NULL && this->requestedStateNodes_.back()->parent_)
            this->requestState(this->requestedStateNodes_.back()->parent_->state_->getName());
        else
            COUT(2) << "Warning: Could not pop GameState. Ignoring." << std::endl;
    }

    GameState* Game::getState(const std::string& name)
    {
        std::map<std::string, GameState*>::const_iterator it = allStates_s.find(getLowercase(name));
        if (it != allStates_s.end())
            return it->second;
        else
        {
            COUT(1) << "Error: Could not find GameState '" << name << "'. Ignoring." << std::endl;
            return 0;
        }
    }

    void Game::setStateHierarchy(const std::string& str)
    {
        // Split string into pieces of the form whitespacesText
        std::vector<std::pair<std::string, unsigned> > stateStrings;
        size_t pos = 0;
        size_t startPos = 0;
        while (pos < str.size())
        {
            unsigned indentation = 0;
            while(pos < str.size() && str[pos] == ' ')
                ++indentation, ++pos;
            startPos = pos;
            while(pos < str.size() && str[pos] != ' ')
                ++pos;
            stateStrings.push_back(std::pair<std::string, unsigned>(
                str.substr(startPos, pos - startPos), indentation));
        }
        unsigned int currentLevel = 0;
        GameStateTreeNode* currentNode = 0;
        for (std::vector<std::pair<std::string, unsigned> >::const_iterator it = stateStrings.begin(); it != stateStrings.end(); ++it)
        {
            std::string newStateName = it->first;
            unsigned newLevel = it->second;
            GameState* newState = this->getState(newStateName);
            if (!newState)
                ThrowException(GameState, std::string("GameState with name '") + newStateName + "' not found!");
            if (newLevel == 0)
            {
                // root
                if (this->rootStateNode_ != NULL)
                    ThrowException(GameState, "No two root GameStates are allowed!");
                GameStateTreeNode* newNode = new GameStateTreeNode;
                this->allStateNodes_.push_back(newNode);
                newNode->state_ = newState;
                newNode->parent_ = 0;
                this->rootStateNode_ = newNode;
                currentNode = this->rootStateNode_;
            }
            else if (currentNode)
            {
                GameStateTreeNode* newNode = new GameStateTreeNode;
                this->allStateNodes_.push_back(newNode);
                newNode->state_ = newState;
                if (newLevel < currentLevel)
                {
                    // Get down the hierarchy
                    do
                        currentNode = currentNode->parent_;
                    while (newLevel < --currentLevel);
                }
                if (newLevel == currentLevel)
                {
                    // same level
                    newNode->parent_ = currentNode->parent_;
                    newNode->parent_->children_.push_back(newNode);
                }
                else if (newLevel == currentLevel + 1)
                {
                    // child
                    newNode->parent_ = currentNode;
                    currentNode->children_.push_back(newNode);
                }
                else
                    ThrowException(GameState, "Indentation error while parsing the hierarchy.");
                currentNode = newNode;
                currentLevel = newLevel;
            }
            else
            {
                ThrowException(GameState, "No root GameState specified!");
            }
        }
    }

    /*** Internal ***/

    void Game::loadState(GameState* state)
    {
        if (!this->activeStates_.empty())
            this->activeStates_.back()->activity_.topState = false;
        state->activate();
        state->activity_.topState = true;
        this->activeStates_.push_back(state);
    }

    void Game::unloadState(orxonox::GameState* state)
    {
        state->activity_.topState = false;
        state->deactivate();
        this->activeStates_.pop_back();
        if (!this->activeStates_.empty())
            this->activeStates_.back()->activity_.topState = true;
    }

    /*static*/ bool Game::addGameState(GameState* state)
    {
        std::map<std::string, GameState*>::const_iterator it = allStates_s.find(getLowercase(state->getName()));
        if (it == allStates_s.end())
            allStates_s[getLowercase(state->getName())] = state;
        else
            ThrowException(GameState, "Cannot add two GameStates with the same name to 'Game'.");

        // just a required dummy return value
        return true;
    }

    /*static*/ void Game::destroyStates()
    {
        // Delete all GameStates created by the macros
        for (std::map<std::string, GameState*>::const_iterator it = allStates_s.begin(); it != allStates_s.end(); ++it)
            delete it->second;
        allStates_s.clear();
    }
}
