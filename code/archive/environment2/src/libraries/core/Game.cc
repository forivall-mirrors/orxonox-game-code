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
#include <loki/ScopeGuard.h>

#include "util/Clock.h"
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/Sleep.h"
#include "util/SubString.h"
#include "CommandLineParser.h"
#include "Core.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"
#include "GameMode.h"
#include "GameState.h"
#include "GraphicsManager.h"
#include "GUIManager.h"
#include "command/ConsoleCommand.h"

namespace orxonox
{
    static void stop_game()
        { Game::getInstance().stop(); }
    SetConsoleCommand("exit", &stop_game);
    static void printFPS()
        { COUT(0) << Game::getInstance().getAvgFPS() << std::endl; }
    SetConsoleCommand("Stats", "printFPS", &printFPS);
    static void printTickTime()
        { COUT(0) << Game::getInstance().getAvgTickTime() << std::endl; }
    SetConsoleCommand("Stats", "printTickTime", &printTickTime);

    std::map<std::string, GameStateInfo> Game::gameStateDeclarations_s;
    Game* Game::singletonPtr_s = 0;

    //! Represents one node of the game state tree.
    struct GameStateTreeNode
    {
        std::string name_;
        weak_ptr<GameStateTreeNode> parent_;
        std::vector<shared_ptr<GameStateTreeNode> > children_;
    };

    Game::Game(const std::string& cmdLine)
        // Destroy factories before the Core!
        : gsFactoryDestroyer_(Game::GameStateFactory::getFactories(), &std::map<std::string, shared_ptr<GameStateFactory> >::clear)
    {
        this->bAbort_ = false;
        bChangingState_ = false;

#ifdef ORXONOX_PLATFORM_WINDOWS
        minimumSleepTime_ = 1000/*us*/;
#else
        minimumSleepTime_ = 0/*us*/;
#endif

        // reset statistics
        this->statisticsStartTime_ = 0;
        this->statisticsTickTimes_.clear();
        this->periodTickTime_ = 0;
        this->periodTime_ = 0;
        this->avgFPS_ = 0.0f;
        this->avgTickTime_ = 0.0f;
        this->excessSleepTime_ = 0;

        // Create an empty root state
        this->declareGameState<GameState>("GameState", "emptyRootGameState", true, false);

        // Set up a basic clock to keep time
        this->gameClock_.reset(new Clock());

        // Create the Core
        this->core_.reset(new Core(cmdLine));

        // Do this after the Core creation!
        ClassIdentifier<Game>::getIdentifier("Game")->initialiseObject(this, "Game", true);
        this->setConfigValues();

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
    }

    /**
    @brief
        All destruction code is handled by scoped_ptrs and SimpleScopeGuards.
    */
    Game::~Game()
    {
        // Remove us from the object lists again to avoid problems when destroying them
        this->unregisterObject();
    }

    void Game::setConfigValues()
    {
        SetConfigValue(statisticsRefreshCycle_, 250000)
            .description("Sets the time in microseconds interval at which average fps, etc. get updated.");
        SetConfigValue(statisticsAvgLength_, 1000000)
            .description("Sets the time in microseconds interval at which average fps, etc. gets calculated.");

        SetConfigValueExternal(fpsLimit_, "GraphicsSettings", "fpsLimit", 50)
            .description("Sets the desired frame rate (0 for no limit).");
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

            // Core preUpdate
            try
                { this->core_->preUpdate(*this->gameClock_); }
            catch (...)
            {
                COUT(0) << "An exception occurred in the Core preUpdate: " << Exception::handleMessage() << std::endl;
                COUT(0) << "This should really never happen! Closing the program." << std::endl;
                this->stop();
                break;
            }

            // Update the GameStates bottom up in the stack
            this->updateGameStates();

            // Core postUpdate
            try
                { this->core_->postUpdate(*this->gameClock_); }
            catch (...)
            {
                COUT(0) << "An exception occurred in the Core postUpdate: " << Exception::handleMessage() << std::endl;
                COUT(0) << "This should really never happen! Closing the program." << std::endl;
                this->stop();
                break;
            }

            // Evaluate statistics
            this->updateStatistics();

            // Limit frame rate
            static bool hasVSync = GameMode::showsGraphics() && GraphicsManager::getInstance().hasVSyncEnabled(); // can be static since changes of VSync currently require a restart
            if (this->fpsLimit_ > 0 && !hasVSync)
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
                catch (...)
                {
                    COUT(1) << "Error: Loading GameState '" << requestedStateNode->name_ << "' failed: " << Exception::handleMessage() << std::endl;
                    // All scheduled operations have now been rendered inert --> flush them and issue a warning
                    if (this->requestedStateNodes_.size() > 1)
                        COUT(4) << "All " << this->requestedStateNodes_.size() - 1 << " scheduled transitions have been ignored." << std::endl;
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
        for (GameStateVector::const_iterator it = this->loadedStates_.begin() + 1;
            it != this->loadedStates_.end(); ++it)
        {
            try
            {
                // Add tick time for most of the states
                uint64_t timeBeforeTick = 0;
                if ((*it)->getInfo().bIgnoreTickTime)
                    timeBeforeTick = this->gameClock_->getRealMicroseconds();
                (*it)->update(*this->gameClock_);
                if ((*it)->getInfo().bIgnoreTickTime)
                    this->subtractTickTime(static_cast<int32_t>(this->gameClock_->getRealMicroseconds() - timeBeforeTick));
            }
            catch (...)
            {
                COUT(1) << "An exception occurred while updating '" << (*it)->getName() << "': " << Exception::handleMessage() << std::endl;
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
        this->statisticsTickTimes_.back().tickLength += (uint32_t)(currentRealTime - currentTime);
        this->periodTickTime_ += (uint32_t)(currentRealTime - currentTime);
        if (this->periodTime_ > this->statisticsRefreshCycle_)
        {
            std::list<StatisticsTickInfo>::iterator it = this->statisticsTickTimes_.begin();
            assert(it != this->statisticsTickTimes_.end());
            int64_t lastTime = currentTime - this->statisticsAvgLength_;
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
            // Why minus 1? No idea, but otherwise the fps rate is always (from 10 to 200!) one frame too low
            this->avgFPS_ = -1 + static_cast<float>(framesPerPeriod) / (currentTime - this->statisticsTickTimes_.front().tickTime) * 1000000.0f;
            this->avgTickTime_ = static_cast<float>(this->periodTickTime_) / framesPerPeriod / 1000.0f;

            this->periodTime_ -= this->statisticsRefreshCycle_;
        }
    }

    void Game::updateFPSLimiter()
    {
        uint64_t nextTime = gameClock_->getMicroseconds() - excessSleepTime_ + static_cast<uint32_t>(1000000.0f / fpsLimit_);
        uint64_t currentRealTime = gameClock_->getRealMicroseconds();
        while (currentRealTime < nextTime - minimumSleepTime_)
        {
            usleep((unsigned long)(nextTime - currentRealTime));
            currentRealTime = gameClock_->getRealMicroseconds();
        }
        // Integrate excess to avoid steady state error
        excessSleepTime_ = (int)(currentRealTime - nextTime);
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
            if (currentNode == NULL)
                requestedNodes.clear();
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

    shared_ptr<GameState> Game::getState(const std::string& name)
    {
        GameStateMap::const_iterator it = constructedStates_.find(name);
        if (it != constructedStates_.end())
            return it->second;
        else
        {
            std::map<std::string, GameStateInfo>::const_iterator it = gameStateDeclarations_s.find(name);
            if (it != gameStateDeclarations_s.end())
                COUT(1) << "Error: GameState '" << name << "' has not yet been loaded." << std::endl;
            else
                COUT(1) << "Error: Could not find GameState '" << name << "'." << std::endl;
            return shared_ptr<GameState>();
        }
    }

    void Game::setStateHierarchy(const std::string& str)
    {
        // Split string into pieces of the form whitespacesText
        std::vector<std::pair<std::string, int> > stateStrings;
        size_t pos = 0;
        size_t startPos = 0;
        while (pos < str.size())
        {
            int indentation = 0;
            while (pos < str.size() && str[pos] == ' ')
                ++indentation, ++pos;
            startPos = pos;
            while (pos < str.size() && str[pos] != ' ')
                ++pos;
            stateStrings.push_back(std::make_pair(str.substr(startPos, pos - startPos), indentation));
        }
        if (stateStrings.empty())
            ThrowException(GameState, "Emtpy GameState hierarchy provided, terminating.");
        // Add element with large identation to detect the last with just an iterator
        stateStrings.push_back(std::make_pair(std::string(), -1));

        // Parse elements recursively
        std::vector<std::pair<std::string, int> >::const_iterator begin = stateStrings.begin();
        parseStates(begin, this->rootStateNode_);
    }

    /*** Internal ***/

    void Game::parseStates(std::vector<std::pair<std::string, int> >::const_iterator& it, shared_ptr<GameStateTreeNode> currentNode)
    {
        SubString tokens(it->first, ",");
        std::vector<std::pair<std::string, int> >::const_iterator startIt = it;

        for (unsigned int i = 0; i < tokens.size(); ++i)
        {
            it = startIt; // Reset iterator to the beginning of the sub tree
            if (!this->checkState(tokens[i]))
                ThrowException(GameState, "GameState with name '" << tokens[i] << "' not found!");
            if (tokens[i] == this->rootStateNode_->name_)
                ThrowException(GameState, "You shouldn't use 'emptyRootGameState' in the hierarchy...");
            shared_ptr<GameStateTreeNode> node(new GameStateTreeNode());
            node->name_ = tokens[i];
            node->parent_ = currentNode;
            currentNode->children_.push_back(node);

            int currentLevel = it->second;
            ++it;
            while (it->second != -1)
            {
                if (it->second <= currentLevel)
                    break;
                else if (it->second == currentLevel + 1)
                    parseStates(it, node);
                else
                    ThrowException(GameState, "Indentation error while parsing the hierarchy.");
            }
        }
    }

    void Game::loadGraphics()
    {
        if (!GameMode::showsGraphics())
        {
            core_->loadGraphics();
            Loki::ScopeGuard graphicsUnloader = Loki::MakeObjGuard(*this, &Game::unloadGraphics);

            // Construct all the GameStates that require graphics
            for (std::map<std::string, GameStateInfo>::const_iterator it = gameStateDeclarations_s.begin();
                it != gameStateDeclarations_s.end(); ++it)
            {
                if (it->second.bGraphicsMode)
                {
                    // Game state loading failure is serious --> don't catch
                    shared_ptr<GameState> gameState = GameStateFactory::fabricate(it->second);
                    if (!constructedStates_.insert(std::make_pair(
                        it->second.stateName, gameState)).second)
                        assert(false); // GameState was already created!
                }
            }
            graphicsUnloader.Dismiss();
        }
    }

    void Game::unloadGraphics()
    {
        if (GameMode::showsGraphics())
        {
            // Destroy all the GameStates that require graphics
            for (GameStateMap::iterator it = constructedStates_.begin(); it != constructedStates_.end();)
            {
                if (it->second->getInfo().bGraphicsMode)
                    constructedStates_.erase(it++);
                else
                    ++it;
            }

            core_->unloadGraphics();
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
        LOKI_ON_BLOCK_EXIT_OBJ(*this, &Game::resetChangingState);

        // If state requires graphics, load it
        Loki::ScopeGuard graphicsUnloader = Loki::MakeObjGuard(*this, &Game::unloadGraphics);
        if (gameStateDeclarations_s[name].bGraphicsMode && !GameMode::showsGraphics())
            this->loadGraphics();
        else
            graphicsUnloader.Dismiss();

        shared_ptr<GameState> state = this->getState(name);
        state->activateInternal();
        if (!this->loadedStates_.empty())
            this->loadedStates_.back()->activity_.topState = false;
        this->loadedStates_.push_back(state);
        state->activity_.topState = true;

        graphicsUnloader.Dismiss();
    }

    void Game::unloadState(const std::string& name)
    {
        this->bChangingState_ = true;
        try
        {
            shared_ptr<GameState> state = this->getState(name);
            state->activity_.topState = false;
            this->loadedStates_.pop_back();
            if (!this->loadedStates_.empty())
                this->loadedStates_.back()->activity_.topState = true;
            state->deactivateInternal();
        }
        catch (...)
        {
            COUT(2) << "Warning: Unloading GameState '" << name << "' threw an exception: " << Exception::handleMessage() << std::endl;
            COUT(2) << "         There might be potential resource leaks involved! To avoid this, improve exception-safety." << std::endl;
        }
        // Check if graphics is still required
        if (!bAbort_)
        {
            bool graphicsRequired = false;
            for (unsigned i = 0; i < loadedStates_.size(); ++i)
                graphicsRequired |= loadedStates_[i]->getInfo().bGraphicsMode;
            if (!graphicsRequired)
                this->unloadGraphics();
        }
        this->bChangingState_ = false;
    }

    /*static*/ std::map<std::string, shared_ptr<Game::GameStateFactory> >& Game::GameStateFactory::getFactories()
    {
        static std::map<std::string, shared_ptr<GameStateFactory> > factories;
        return factories;
    }

    /*static*/ shared_ptr<GameState> Game::GameStateFactory::fabricate(const GameStateInfo& info)
    {
        std::map<std::string, shared_ptr<Game::GameStateFactory> >::const_iterator it = getFactories().find(info.className);
        assert(it != getFactories().end());
        return it->second->fabricateInternal(info);
    }
}
