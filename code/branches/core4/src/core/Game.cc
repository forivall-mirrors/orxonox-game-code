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
#include <boost/weak_ptr.hpp>

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
    using boost::shared_ptr;
    using boost::weak_ptr;

    static void stop_game()
        { Game::getInstance().stop(); }
    SetConsoleCommandShortcutExternAlias(stop_game, "exit");

    std::map<std::string, Game::GameStateInfo> Game::gameStateDeclarations_s;
    Game* Game::singletonRef_s = 0;


    /**
    @brief
        Represents one node of the game state tree.
    */
    struct GameStateTreeNode
    {
        GameState* state_;
        weak_ptr<GameStateTreeNode> parent_;
        std::vector<shared_ptr<GameStateTreeNode> > children_;
    };


    /**
    @brief
        Another helper class for the Game singleton: we cannot derive
        Game from OrxonoxClass because we need to handle the Identifier
        destruction in the Core destructor.
    */
    class GameConfiguration : public OrxonoxClass
    {
    public:
        GameConfiguration()
        {
            RegisterRootObject(GameConfiguration);
            this->setConfigValues();
        }

        void setConfigValues()
        {
            SetConfigValue(statisticsRefreshCycle_, 250000)
                .description("Sets the time in microseconds interval at which average fps, etc. get updated.");
            SetConfigValue(statisticsAvgLength_, 1000000)
                .description("Sets the time in microseconds interval at which average fps, etc. gets calculated.");
        }

        unsigned int statisticsRefreshCycle_;
        unsigned int statisticsAvgLength_;
    };


    /**
    @brief
        Non-initialising constructor.
    */
    Game::Game(int argc, char** argv)
    {
        if (singletonRef_s != 0)
        {
            COUT(0) << "Error: The Game singleton cannot be recreated! Shutting down." << std::endl;
            abort();
        }
        singletonRef_s = this;

        this->bAbort_ = false;
        bChangingState_ = false;

        // Create an empty root state
        declareGameState<GameState>("GameState", "emptyRootGameState", true, false);

        // reset statistics
        this->statisticsStartTime_ = 0;
        this->statisticsTickTimes_.clear();
        this->periodTickTime_ = 0;
        this->periodTime_ = 0;
        this->avgFPS_ = 0.0f;
        this->avgTickTime_ = 0.0f;

        // Set up a basic clock to keep time
        this->gameClock_ = new Clock();

        // Create the Core
        this->core_ = new Core(argc, argv);

        // After the core has been created, we can safely instantiate the GameStates
        for (std::map<std::string, GameStateInfo>::const_iterator it = gameStateDeclarations_s.begin();
            it != gameStateDeclarations_s.end(); ++it)
        {
            // Only create the states appropriate for the game mode
            //if (GameMode::showsGraphics || !it->second.bGraphicsMode)
            GameStateConstrParams params = { it->second.stateName, it->second.bIgnoreTickTime };
            gameStates_[getLowercase(it->second.stateName)] = GameStateFactory::fabricate(it->second.className, params);
        }

        // The empty root state is ALWAYS loaded!
        this->rootStateNode_ = shared_ptr<GameStateTreeNode>(new GameStateTreeNode());
        this->rootStateNode_->state_ = getState("emptyRootGameState");
        this->activeStateNode_ = this->rootStateNode_;
        this->activeStates_.push_back(this->rootStateNode_->state_);

        // Do this after the Core creation!
        this->configuration_ = new GameConfiguration();
    }

    /**
    @brief
    */
    Game::~Game()
    {
        // Destroy the configuration helper class instance
        delete this->configuration_;

        // Destroy the GameStates (note that the nodes still point to them, but doesn't matter)
        for (std::map<std::string, GameState*>::const_iterator it = gameStates_.begin();
            it != gameStates_.end(); ++it)
            delete it->second;

        // Destroy the Core and with it almost everything
        delete this->core_;
        delete this->gameClock_;

        // Take care of the GameStateFactories
        GameStateFactory::destroyFactories();

        // Don't assign singletonRef_s with NULL! Recreation is not supported
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
        if (this->requestedStateNodes_.empty())
            COUT(0) << "Warning: Starting game without requesting GameState. This automatically terminates the program." << std::endl;

        // START GAME
        this->gameClock_->capture(); // first delta time should be about 0 seconds
        while (!this->bAbort_ && (!this->activeStates_.empty() || this->requestedStateNodes_.size() > 0))
        {
            this->gameClock_->capture();
            uint64_t currentTime = this->gameClock_->getMicroseconds();

            // STATISTICS
            StatisticsTickInfo tickInfo = {currentTime, 0};
            statisticsTickTimes_.push_back(tickInfo);
            this->periodTime_ += this->gameClock_->getDeltaTimeMicroseconds();

            // UPDATE STATE STACK
            while (this->requestedStateNodes_.size() > 0)
            {
                shared_ptr<GameStateTreeNode> requestedStateNode = this->requestedStateNodes_.front();
                assert(this->activeStateNode_);
                if (!this->activeStateNode_->parent_.expired() && requestedStateNode == this->activeStateNode_->parent_.lock())
                    this->unloadState(this->activeStateNode_->state_);
                else // has to be child
                {
                    try
                    {
                        this->loadState(requestedStateNode->state_);
                    }
                    catch (const std::exception& ex)
                    {
                        COUT(1) << "Error: Loading GameState '" << requestedStateNode->state_->getName() << "' failed: " << ex.what() << std::endl;
                        // All scheduled operations have now been rendered inert --> flush them and issue a warning
                        if (this->requestedStateNodes_.size() > 1)
                            COUT(1) << "All " << this->requestedStateNodes_.size() - 1 << " scheduled transitions have been ignored." << std::endl;
                        this->requestedStateNodes_.clear();
                        break;
                    }
                }
                this->activeStateNode_ = requestedStateNode;
                this->requestedStateNodes_.erase(this->requestedStateNodes_.begin());
            }

            // UPDATE, Core first
            try
            {
                this->core_->update(*this->gameClock_);
            }
            catch (...)
            {
                COUT(0) << "An exception occured while ticking the Core. This should really never happen!" << std::endl;
                COUT(0) << "Closing the program." << std::endl;
                this->stop();
                break;
            }

            // UPDATE, GameStates bottom to top in the stack
            // Note: The first element is the empty root state, which doesn't need ticking
            for (std::vector<GameState*>::const_iterator it = this->activeStates_.begin() + 1;
                it != this->activeStates_.end(); ++it)
            {
                try
                {
                    // Add tick time for most of the states
                    uint64_t timeBeforeTick;
                    if (!(*it)->ignoreTickTime())
                        timeBeforeTick = this->gameClock_->getRealMicroseconds();
                    (*it)->update(*this->gameClock_);
                    if (!(*it)->ignoreTickTime())
                        this->addTickTime(static_cast<uint32_t>(this->gameClock_->getRealMicroseconds() - timeBeforeTick));
                }
                catch (...)
                {
                    COUT(1) << "An exception occured while ticking GameState '" << (*it)->getName() << "'. This should really never happen!" << std::endl;
                    COUT(1) << "Unloading all GameStates depending on the one that crashed." << std::endl;
                    if ((*it)->getParent() != NULL)
                        this->requestState((*it)->getParent()->getName());
                    else
                        this->stop();
                    break;
                }

            }

            // STATISTICS
            if (this->periodTime_ > this->configuration_->statisticsRefreshCycle_)
            {
                std::list<StatisticsTickInfo>::iterator it = this->statisticsTickTimes_.begin();
                assert(it != this->statisticsTickTimes_.end());
                int64_t lastTime = currentTime - this->configuration_->statisticsAvgLength_;
                if ((int64_t)it->tickTime < lastTime)
                {
                    do
                    {
                        assert(this->periodTickTime_ >= it->tickLength);
                        this->periodTickTime_ -= it->tickLength;
                        ++it;
                        assert(it != this->statisticsTickTimes_.end());
                    } while ((int64_t)it->tickTime < lastTime);
                    this->statisticsTickTimes_.erase(this->statisticsTickTimes_.begin(), it);
                }

                uint32_t framesPerPeriod = this->statisticsTickTimes_.size();
                this->avgFPS_ = static_cast<float>(framesPerPeriod) / (currentTime - this->statisticsTickTimes_.front().tickTime) * 1000000.0f;
                this->avgTickTime_ = static_cast<float>(this->periodTickTime_) / framesPerPeriod / 1000.0f;

                this->periodTime_ -= this->configuration_->statisticsRefreshCycle_;
            }
        }

        // UNLOAD all remaining states
        while (this->activeStates_.size() > 1)
            this->unloadState(this->activeStates_.back());
        this->activeStateNode_ = this->rootStateNode_;
        this->requestedStateNodes_.clear();
    }

    void Game::stop()
    {
        this->bAbort_ = true;
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
        if (state == NULL)
            return;

        //if (this->bChangingState_)
        //{
        //    COUT(2) << "Warning: Requesting GameStates while loading/unloading a GameState is illegal! Ignoring." << std::endl;
        //    return;
        //}

        shared_ptr<GameStateTreeNode> lastRequestedNode;
        if (this->requestedStateNodes_.empty())
            lastRequestedNode = this->activeStateNode_;
        else
            lastRequestedNode = this->requestedStateNodes_.back();
        if (state == lastRequestedNode->state_)
        {
            COUT(2) << "Warning: Requesting the currently active state! Ignoring." << std::endl;
            return;
        }

        // Check children first
        shared_ptr<GameStateTreeNode> requestedNode;
        for (unsigned int i = 0; i < lastRequestedNode->children_.size(); ++i)
        {
            if (lastRequestedNode->children_[i]->state_ == state)
            {
                requestedNode = lastRequestedNode->children_[i];
                break;
            }
        }

        // Check parent and all its grand parents
        shared_ptr<GameStateTreeNode> currentNode = lastRequestedNode;
        while (requestedNode == NULL && currentNode != NULL)
        {
            if (currentNode->state_ == state)
                requestedNode = currentNode;
            currentNode = currentNode->parent_.lock();
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
        shared_ptr<GameStateTreeNode> lastRequestedNode;
        if (this->requestedStateNodes_.empty())
            lastRequestedNode = this->activeStateNode_;
        else
            lastRequestedNode = this->requestedStateNodes_.back();
        if (lastRequestedNode != this->rootStateNode_)
            this->requestState(lastRequestedNode->parent_.lock()->state_->getName());
        else
            COUT(2) << "Warning: Can't pop the internal dummy root GameState" << std::endl;
    }

    GameState* Game::getState(const std::string& name)
    {
        std::map<std::string, GameState*>::const_iterator it = gameStates_.find(getLowercase(name));
        if (it != gameStates_.end())
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
            stateStrings.push_back(std::make_pair(str.substr(startPos, pos - startPos), indentation));
        }
        unsigned int currentLevel = 0;
        shared_ptr<GameStateTreeNode> currentNode = this->rootStateNode_;
        for (std::vector<std::pair<std::string, unsigned> >::const_iterator it = stateStrings.begin(); it != stateStrings.end(); ++it)
        {
            std::string newStateName = it->first;
            unsigned newLevel = it->second + 1; // empty root is 0
            GameState* newState = this->getState(newStateName);
            if (!newState)
                ThrowException(GameState, "GameState with name '" << newStateName << "' not found!");
            if (newState == this->rootStateNode_->state_)
                ThrowException(GameState, "You shouldn't use 'emptyRootGameState' in the hierarchy...");
            shared_ptr<GameStateTreeNode> newNode(new GameStateTreeNode);
            newNode->state_ = newState;

            if (newLevel <= currentLevel)
            {
                do
                    currentNode = currentNode->parent_.lock();
                while (newLevel <= --currentLevel);
            }
            if (newLevel == currentLevel + 1)
            {
                // Add the child
                newNode->parent_ = currentNode;
                currentNode->children_.push_back(newNode);
                currentNode->state_->addChild(newNode->state_);
            }
            else
                ThrowException(GameState, "Indentation error while parsing the hierarchy.");
            currentNode = newNode;
            currentLevel = newLevel;
        }
    }

    /*** Internal ***/

    void Game::loadState(GameState* state)
    {
        this->bChangingState_ = true;
        state->activate();
        if (!this->activeStates_.empty())
            this->activeStates_.back()->activity_.topState = false;
        this->activeStates_.push_back(state);
        state->activity_.topState = true;
        this->bChangingState_ = false;
    }

    void Game::unloadState(orxonox::GameState* state)
    {
        this->bChangingState_ = true;
        state->activity_.topState = false;
        this->activeStates_.pop_back();
        if (!this->activeStates_.empty())
            this->activeStates_.back()->activity_.topState = true;
        try
        {
            state->deactivate();
        }
        catch (const std::exception& ex)
        {
            COUT(2) << "Warning: Unloading GameState '" << state->getName() << "' threw an exception: " << ex.what() << std::endl;
            COUT(2) << "         There might be potential resource leaks involved! To avoid this, improve exception-safety." << std::endl;
        }
        this->bChangingState_ = false;
    }

    std::map<std::string, Game::GameStateFactory*> Game::GameStateFactory::factories_s;

    /*static*/ GameState* Game::GameStateFactory::fabricate(const std::string& className, const GameStateConstrParams& params)
    {
        std::map<std::string, GameStateFactory*>::const_iterator it = factories_s.find(className);
        assert(it != factories_s.end());
        return it->second->fabricate(params);
    }

    /*static*/ void Game::GameStateFactory::destroyFactories()
    {
        for (std::map<std::string, GameStateFactory*>::const_iterator it = factories_s.begin(); it != factories_s.end(); ++it)
            delete it->second;
        factories_s.clear();
    }
}
