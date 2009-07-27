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
#include "util/Sleep.h"
#include "util/SubString.h"
#include "Clock.h"
#include "CommandLine.h"
#include "ConsoleCommand.h"
#include "Core.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"
#include "GameMode.h"
#include "GameState.h"

namespace orxonox
{
    using boost::shared_ptr;
    using boost::weak_ptr;

    static void stop_game()
        { Game::getInstance().stop(); }
    SetConsoleCommandShortcutExternAlias(stop_game, "exit");

    std::map<std::string, GameStateInfo> Game::gameStateDeclarations_s;
    Game* Game::singletonRef_s = 0;


    /**
    @brief
        Represents one node of the game state tree.
    */
    struct GameStateTreeNode
    {
        std::string name_;
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
            SetConfigValue(fpsLimit_, 50)
                .description("Sets the desired framerate (0 for no limit).");
        }

        unsigned int statisticsRefreshCycle_;
        unsigned int statisticsAvgLength_;
        unsigned int fpsLimit_;
    };


    /**
    @brief
        Non-initialising constructor.
    */
    Game::Game(const std::string& cmdLine)
    {
        if (singletonRef_s != 0)
        {
            COUT(0) << "Error: The Game singleton cannot be recreated! Shutting down." << std::endl;
            abort();
        }
        singletonRef_s = this;

        this->bAbort_ = false;
        bChangingState_ = false;

#ifdef ORXONOX_PLATFORM_WINDOWS
        minimumSleepTime_ = 1000/*us*/;
#else
        minimumSleepTime_ = 0/*us*/;
#endif

        // Create an empty root state
        this->declareGameState<GameState>("GameState", "emptyRootGameState", true, false);

        // Set up a basic clock to keep time
        this->gameClock_ = new Clock();

        // Create the Core
        this->core_ = new Core(cmdLine);

        // After the core has been created, we can safely instantiate the GameStates that don't require graphics
        for (std::map<std::string, GameStateInfo>::const_iterator it = gameStateDeclarations_s.begin();
            it != gameStateDeclarations_s.end(); ++it)
        {
            if (!it->second.bGraphicsMode)
                constructedStates_[it->second.stateName] = GameStateFactory::fabricate(it->second);
        }

        // The empty root state is ALWAYS loaded!
        this->rootStateNode_ = shared_ptr<GameStateTreeNode>(new GameStateTreeNode());
        this->rootStateNode_->name_ = "emptyRootGameState";
        this->loadedTopStateNode_ = this->rootStateNode_;
        this->loadedStates_.push_back(this->getState(rootStateNode_->name_));

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
        for (std::map<std::string, GameState*>::const_iterator it = constructedStates_.begin();
            it != constructedStates_.end(); ++it)
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

        // reset statistics
        this->statisticsStartTime_ = 0;
        this->statisticsTickTimes_.clear();
        this->periodTickTime_ = 0;
        this->periodTime_ = 0;
        this->avgFPS_ = 0.0f;
        this->avgTickTime_ = 0.0f;
        this->excessSleepTime_ = 0;

        // START GAME
        // first delta time should be about 0 seconds
        this->gameClock_->capture();
        // A first item is required for the fps limiter
        StatisticsTickInfo tickInfo = {0, 0};
        statisticsTickTimes_.push_back(tickInfo);
        while (!this->bAbort_ && (!this->loadedStates_.empty() || this->requestedStateNodes_.size() > 0))
        {
            // Generate the dt
            this->gameClock_->capture();

            // Statistics init
            StatisticsTickInfo tickInfo = {gameClock_->getMicroseconds(), 0};
            statisticsTickTimes_.push_back(tickInfo);
            this->periodTime_ += this->gameClock_->getDeltaTimeMicroseconds();

            // Update the GameState stack if required
            this->updateGameStateStack();

            // Core preUpdate (doesn't throw)
            if (!this->core_->preUpdate(*this->gameClock_))
            {
                this->stop();
                break;
            }

            // Update the GameStates bottom up in the stack
            this->updateGameStates();

            // Core postUpdate (doesn't throw)
            if (!this->core_->postUpdate(*this->gameClock_))
            {
                this->stop();
                break;
            }

            // Evaluate statistics
            this->updateStatistics();

            // Limit framerate
            this->updateFPSLimiter();
        }

        // UNLOAD all remaining states
        while (this->loadedStates_.size() > 1)
            this->unloadState(this->loadedStates_.back()->getName());
        this->loadedTopStateNode_ = this->rootStateNode_;
        this->requestedStateNodes_.clear();
    }

    void Game::updateGameStateStack()
    {
        while (this->requestedStateNodes_.size() > 0)
        {
            shared_ptr<GameStateTreeNode> requestedStateNode = this->requestedStateNodes_.front();
            assert(this->loadedTopStateNode_);
            if (!this->loadedTopStateNode_->parent_.expired() && requestedStateNode == this->loadedTopStateNode_->parent_.lock())
                this->unloadState(loadedTopStateNode_->name_);
            else // has to be child
            {
                try
                {
                    this->loadState(requestedStateNode->name_);
                }
                catch (const std::exception& ex)
                {
                    COUT(1) << "Error: Loading GameState '" << requestedStateNode->name_ << "' failed: " << ex.what() << std::endl;
                    // All scheduled operations have now been rendered inert --> flush them and issue a warning
                    if (this->requestedStateNodes_.size() > 1)
                        COUT(1) << "All " << this->requestedStateNodes_.size() - 1 << " scheduled transitions have been ignored." << std::endl;
                    this->requestedStateNodes_.clear();
                    break;
                }
            }
            this->loadedTopStateNode_ = requestedStateNode;
            this->requestedStateNodes_.erase(this->requestedStateNodes_.begin());
        }
    }

    void Game::updateGameStates()
    {
        // Note: The first element is the empty root state, which doesn't need ticking
        for (std::vector<GameState*>::const_iterator it = this->loadedStates_.begin() + 1;
            it != this->loadedStates_.end(); ++it)
        {
            std::string exceptionMessage;
            try
            {
                // Add tick time for most of the states
                uint64_t timeBeforeTick;
                if ((*it)->getInfo().bIgnoreTickTime)
                    timeBeforeTick = this->gameClock_->getRealMicroseconds();
                (*it)->update(*this->gameClock_);
                if ((*it)->getInfo().bIgnoreTickTime)
                    this->subtractTickTime(static_cast<int32_t>(this->gameClock_->getRealMicroseconds() - timeBeforeTick));
            }
            catch (const std::exception& ex)
            { exceptionMessage = ex.what(); }
            catch (...)
            { exceptionMessage = "Unknown exception"; }
            if (!exceptionMessage.empty())
            {
                COUT(1) << "An exception occurred while updating '" << (*it)->getName() << "': " << exceptionMessage << std::endl;
                COUT(1) << "This should really never happen!" << std::endl;
                COUT(1) << "Unloading all GameStates depending on the one that crashed." << std::endl;
                shared_ptr<GameStateTreeNode> current = this->loadedTopStateNode_;
                while (current->name_ != (*it)->getName() && current)
                    current = current->parent_.lock();
                if (current && current->parent_.lock())
                    this->requestState(current->parent_.lock()->name_);
                else
                    this->stop();
                break;
            }
        }
    }

    void Game::updateStatistics()
    {
        // Add the tick time of this frame (rendering time has already been subtracted)
        uint64_t currentTime = gameClock_->getMicroseconds();
        uint64_t currentRealTime = gameClock_->getRealMicroseconds();
        this->statisticsTickTimes_.back().tickLength += currentRealTime - currentTime;
        this->periodTickTime_ += currentRealTime - currentTime;
        if (this->periodTime_ > this->configuration_->statisticsRefreshCycle_)
        {
            std::list<StatisticsTickInfo>::iterator it = this->statisticsTickTimes_.begin();
            assert(it != this->statisticsTickTimes_.end());
            int64_t lastTime = currentTime - this->configuration_->statisticsAvgLength_;
            if (static_cast<int64_t>(it->tickTime) < lastTime)
            {
                do
                {
                    assert(this->periodTickTime_ >= it->tickLength);
                    this->periodTickTime_ -= it->tickLength;
                    ++it;
                    assert(it != this->statisticsTickTimes_.end());
                } while (static_cast<int64_t>(it->tickTime) < lastTime);
                this->statisticsTickTimes_.erase(this->statisticsTickTimes_.begin(), it);
            }

            uint32_t framesPerPeriod = this->statisticsTickTimes_.size();
            this->avgFPS_ = static_cast<float>(framesPerPeriod) / (currentTime - this->statisticsTickTimes_.front().tickTime) * 1000000.0f;
            this->avgTickTime_ = static_cast<float>(this->periodTickTime_) / framesPerPeriod / 1000.0f;

            this->periodTime_ -= this->configuration_->statisticsRefreshCycle_;
        }
    }

    void Game::updateFPSLimiter()
    {
        // Why configuration_->fpsLimit_ - 1? No idea, but otherwise the fps rate is always (from 10 to 200!) one frame too high
        uint32_t nextTime = gameClock_->getMicroseconds() - excessSleepTime_ + static_cast<uint32_t>(1000000.0f / (configuration_->fpsLimit_ - 1));
        uint64_t currentRealTime = gameClock_->getRealMicroseconds();
        while (currentRealTime < nextTime - minimumSleepTime_)
        {
            usleep(nextTime - currentRealTime);
            currentRealTime = gameClock_->getRealMicroseconds();
        }
        // Integrate excess to avoid steady state error
        excessSleepTime_ = currentRealTime - nextTime;
        // Anti windup
        if (excessSleepTime_ > 50000) // 20ms is about the maximum time Windows would sleep for too long
            excessSleepTime_ = 50000;
    }

    void Game::stop()
    {
        this->bAbort_ = true;
    }

    void Game::subtractTickTime(int32_t length)
    {
        assert(!this->statisticsTickTimes_.empty());
        this->statisticsTickTimes_.back().tickLength -= length;
        this->periodTickTime_ -= length;
    }


    /***** GameState related *****/

    void Game::requestState(const std::string& name)
    {
        if (!this->checkState(name))
        {
            COUT(2) << "Warning: GameState named '" << name << "' doesn't exist!" << std::endl;
            return;
        }

        if (this->bChangingState_)
        {
            COUT(2) << "Warning: Requesting GameStates while loading/unloading a GameState is illegal! Ignoring." << std::endl;
            return;
        }

        shared_ptr<GameStateTreeNode> lastRequestedNode;
        if (this->requestedStateNodes_.empty())
            lastRequestedNode = this->loadedTopStateNode_;
        else
            lastRequestedNode = this->requestedStateNodes_.back();
        if (name == lastRequestedNode->name_)
        {
            COUT(2) << "Warning: Requesting the currently active state! Ignoring." << std::endl;
            return;
        }

        // Check children first
        std::vector<shared_ptr<GameStateTreeNode> > requestedNodes;
        for (unsigned int i = 0; i < lastRequestedNode->children_.size(); ++i)
        {
            if (lastRequestedNode->children_[i]->name_ == name)
            {
                requestedNodes.push_back(lastRequestedNode->children_[i]);
                break;
            }
        }

        if (requestedNodes.empty())
        {
            // Check parent and all its grand parents
            shared_ptr<GameStateTreeNode> currentNode = lastRequestedNode;
            while (currentNode != NULL)
            {
                if (currentNode->name_ == name)
                    break;
                currentNode = currentNode->parent_.lock();
                requestedNodes.push_back(currentNode);
            }
        }

        if (requestedNodes.empty())
            COUT(1) << "Error: Requested GameState transition is not allowed. Ignoring." << std::endl;
        else
            this->requestedStateNodes_.insert(requestedStateNodes_.end(), requestedNodes.begin(), requestedNodes.end());
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
            lastRequestedNode = this->loadedTopStateNode_;
        else
            lastRequestedNode = this->requestedStateNodes_.back();
        if (lastRequestedNode != this->rootStateNode_)
            this->requestState(lastRequestedNode->parent_.lock()->name_);
        else
            COUT(2) << "Warning: Can't pop the internal dummy root GameState" << std::endl;
    }

    GameState* Game::getState(const std::string& name)
    {
        std::map<std::string, GameState*>::const_iterator it = constructedStates_.find(name);
        if (it != constructedStates_.end())
            return it->second;
        else
        {
            std::map<std::string, GameStateInfo>::const_iterator it = gameStateDeclarations_s.find(name);
            if (it != gameStateDeclarations_s.end())
                COUT(1) << "Error: GameState '" << name << "' has not yet been loaded." << std::endl;
            else
                COUT(1) << "Error: Could not find GameState '" << name << "'." << std::endl;
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
            if (!this->checkState(newStateName))
                ThrowException(GameState, "GameState with name '" << newStateName << "' not found!");
            if (newStateName == this->rootStateNode_->name_)
                ThrowException(GameState, "You shouldn't use 'emptyRootGameState' in the hierarchy...");
            shared_ptr<GameStateTreeNode> newNode(new GameStateTreeNode);
            newNode->name_ = newStateName;

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
            }
            else
                ThrowException(GameState, "Indentation error while parsing the hierarchy.");
            currentNode = newNode;
            currentLevel = newLevel;
        }
    }

    /*** Internal ***/

    void Game::loadGraphics()
    {
        if (!GameMode::bShowsGraphics_s)
        {
            core_->loadGraphics();
            GameMode::bShowsGraphics_s = true;

            // Construct all the GameStates that require graphics
            for (std::map<std::string, GameStateInfo>::const_iterator it = gameStateDeclarations_s.begin();
                it != gameStateDeclarations_s.end(); ++it)
            {
                if (it->second.bGraphicsMode)
                {
                    if (!constructedStates_.insert(std::make_pair(
                        it->second.stateName, GameStateFactory::fabricate(it->second))).second)
                        assert(false); // GameState was already created!
                }
            }
        }
    }

    void Game::unloadGraphics()
    {
        if (GameMode::bShowsGraphics_s)
        {
            // Destroy all the GameStates that require graphics
            for (std::map<std::string, GameState*>::iterator it = constructedStates_.begin(); it != constructedStates_.end();)
            {
                if (it->second->getInfo().bGraphicsMode)
                {
                    delete it->second;
                    constructedStates_.erase(it++);
                }
                else
                    ++it;
            }

            core_->unloadGraphics();
            GameMode::bShowsGraphics_s = false;
        }
    }

    bool Game::checkState(const std::string& name) const
    {
        std::map<std::string, GameStateInfo>::const_iterator it = gameStateDeclarations_s.find(name);
        if (it == gameStateDeclarations_s.end())
            return false;
        else
            return true;
    }

    void Game::loadState(const std::string& name)
    {
        this->bChangingState_ = true;
        // If state requires graphics, load it
        if (gameStateDeclarations_s[name].bGraphicsMode)
            this->loadGraphics();
        GameState* state = this->getState(name);
        state->activate();
        if (!this->loadedStates_.empty())
            this->loadedStates_.back()->activity_.topState = false;
        this->loadedStates_.push_back(state);
        state->activity_.topState = true;
        this->bChangingState_ = false;
    }

    void Game::unloadState(const std::string& name)
    {
        GameState* state = this->getState(name);
        this->bChangingState_ = true;
        state->activity_.topState = false;
        this->loadedStates_.pop_back();
        if (!this->loadedStates_.empty())
            this->loadedStates_.back()->activity_.topState = true;
        try
        {
            state->deactivate();
            // Check if graphis is still required
            bool graphicsRequired = false;
            for (unsigned i = 0; i < loadedStates_.size(); ++i)
                graphicsRequired |= loadedStates_[i]->getInfo().bGraphicsMode;
            if (!graphicsRequired)
                this->unloadGraphics();
        }
        catch (const std::exception& ex)
        {
            COUT(2) << "Warning: Unloading GameState '" << name << "' threw an exception: " << ex.what() << std::endl;
            COUT(2) << "         There might be potential resource leaks involved! To avoid this, improve exception-safety." << std::endl;
        }
        this->bChangingState_ = false;
    }

    std::map<std::string, Game::GameStateFactory*> Game::GameStateFactory::factories_s;

    /*static*/ GameState* Game::GameStateFactory::fabricate(const GameStateInfo& info)
    {
        std::map<std::string, GameStateFactory*>::const_iterator it = factories_s.find(info.className);
        assert(it != factories_s.end());
        return it->second->fabricateInternal(info);
    }

    /*static*/ void Game::GameStateFactory::destroyFactories()
    {
        for (std::map<std::string, GameStateFactory*>::const_iterator it = factories_s.begin(); it != factories_s.end(); ++it)
            delete it->second;
        factories_s.clear();
    }
}
