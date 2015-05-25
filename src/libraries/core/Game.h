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
@ingroup Management CoreGame
@brief
    Declaration of Game Singleton which is responsible for running the game.
 */

#ifndef _Game_H__
#define _Game_H__

#include "CorePrereqs.h"

#include <cassert>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/preprocessor/cat.hpp>

#include "util/Output.h"
#include "util/DestructionHelper.h"
#include "util/Singleton.h"

/**
@brief
    Adds a new GameState to the Game. The second parameter is the name as string
    and every following parameter is a constructor argument (which is usually non existent)
*/
#define DeclareGameState(className, stateName, bIgnoreTickTime, bGraphicsMode) \
    static bool BOOST_PP_CAT(bGameStateDummy_##className, __UNIQUE_NUMBER__) = orxonox::Game::declareGameState<className>(#className, stateName, bIgnoreTickTime, bGraphicsMode)
// tolua_begin
namespace orxonox
{
// tolua_end

    //! Helper object required before GameStates are being constructed
    struct GameStateInfo
    {
        std::string stateName;
        std::string className;
        bool bIgnoreTickTime;
        bool bGraphicsMode;
    };

    /**
    @brief
        Main class responsible for running the game.
    @remark
        You should only create this singleton once because it owns the Core class! (see remark there)
    */
// tolua_begin
    class _CoreExport Game
// tolua_end
        : public Singleton<Game>
    { // tolua_export
        friend class Singleton<Game>;
        typedef std::vector<shared_ptr<GameState> > GameStateVector;
        typedef std::map<std::string, shared_ptr<GameState> > GameStateMap;
        typedef shared_ptr<GameStateTreeNode> GameStateTreeNodePtr;

    public:
        Game(const std::string& cmdLine);

        //! Leave empty and use cleanup() instead
        ~Game() {}
        /// Destructor that also executes when object fails to construct
        void destroy();

        void setStateHierarchy(const std::string& str);
        shared_ptr<GameState> getState(const std::string& name);

        void run();
        void stop();

        static Game& getInstance(){ return Singleton<Game>::getInstance(); } // tolua_export

        void requestState(const std::string& name); //tolua_export
        void requestStates(const std::string& names); //tolua_export
        void popState(); //tolua_export

        const Clock& getGameClock() { return *this->gameClock_; }

        float getAvgTickTime() { return this->avgTickTime_; }
        float getAvgFPS()      { return this->avgFPS_; }

        void subtractTickTime(int32_t length);

        template <class T>
        static bool declareGameState(const std::string& className, const std::string& stateName, bool bIgnoreTickTime, bool bConsoleMode);

    private:
        class _CoreExport GameStateFactory
        {
        public:
            virtual ~GameStateFactory() { }
            static shared_ptr<GameState> fabricate(const GameStateInfo& info);
            template <class T>
            static void createFactory(const std::string& className)
                { getFactories()[className].reset(new TemplateGameStateFactory<T>()); }

            virtual shared_ptr<GameState> fabricateInternal(const GameStateInfo& info) = 0;
            static std::map<std::string, shared_ptr<GameStateFactory> >& getFactories();
        };
        template <class T>
        class TemplateGameStateFactory : public GameStateFactory
        {
        public:
            shared_ptr<GameState> fabricateInternal(const GameStateInfo& info)
                { return shared_ptr<GameState>(new T(info)); }
        };

        struct StatisticsTickInfo
        {
            uint64_t    tickTime;
            uint32_t    tickLength;
        };

        Game(Game&); // don't mess with singletons

        void loadGraphics();
        void unloadGraphics();

        void parseStates(std::vector<std::pair<std::string, int> >::const_iterator& it, shared_ptr<GameStateTreeNode> currentNode);
        bool checkState(const std::string& name) const;
        void loadState(const std::string& name);
        void unloadState(const std::string& name);

        // Main loop structuring
        void updateGameStateStack();
        void updateGameStates();
        void updateStatistics();
        void updateFPSLimiter();

        // ScopeGuard helper function
        void resetChangingState() { this->bChangingState_ = false; }

        Clock*                             gameClock_;
        Core*                              core_;

        GameStateMap                       constructedStates_;
        GameStateVector                    loadedStates_;
        GameStateTreeNodePtr               rootStateNode_;
        GameStateTreeNodePtr               loadedTopStateNode_;
        std::vector<GameStateTreeNodePtr>  requestedStateNodes_;

        bool                               bChangingState_;
        bool                               bAbort_;

        // variables for time statistics
        uint64_t                           statisticsStartTime_;
        std::list<StatisticsTickInfo>      statisticsTickTimes_;
        uint32_t                           periodTime_;
        uint32_t                           periodTickTime_;
        float                              avgFPS_;
        float                              avgTickTime_;
        int                                excessSleepTime_;
        unsigned int                       minimumSleepTime_;

        /// Helper object that stores the config values
        GameConfig*                        config_;

        /// Helper object that executes the surrogate destructor destroy()
        DestructionHelper<Game>            destructionHelper_;

        static std::map<std::string, GameStateInfo> gameStateDeclarations_s;
        static Game* singletonPtr_s;        //!< Pointer to the Singleton
    }; //tolua_export

    template <class T>
    /*static*/ bool Game::declareGameState(const std::string& className, const std::string& stateName, bool bIgnoreTickTime, bool bGraphicsMode)
    {
        std::map<std::string, GameStateInfo>::const_iterator it = gameStateDeclarations_s.find(stateName);
        if (it == gameStateDeclarations_s.end())
        {
            GameStateInfo& info = gameStateDeclarations_s[stateName];
            info.stateName = stateName;
            info.className = className;
            info.bIgnoreTickTime = bIgnoreTickTime;
            info.bGraphicsMode = bGraphicsMode;
        }
        else
        {
            orxout(internal_warning) << "Cannot declare two GameStates with the same name." << endl;
            orxout(internal_warning) << "Ignoring second one ('" << stateName << "')." << endl;
        }

        // Create a factory to delay GameState creation
        GameStateFactory::createFactory<T>(className);

        // just a required dummy return value
        return true;
    }
} //tolua_export

#endif /* _Game_H__ */
