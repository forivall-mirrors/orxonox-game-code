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
 *      Florian Zinggeler
 *   Co-authors:
 *      ...
 *
 */

/**
    @file DodgeRace.h
    @brief Gametype.
    @ingroup DodgeRace
*/

#ifndef _DodgeRace_H__
#define _DodgeRace_H__

#include "dodgerace/DodgeRacePrereqs.h"

#include "DodgeRaceCenterPoint.h" // Necessary for WeakPointer??
//#include "DodgeRaceShip.h"        DO NOT include in Header. Will cause forward declaration issues

//#include "DodgeRaceHUDinfo.h"


#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/command/Executor.h"
#include "core/config/ConfigValueIncludes.h"

#include "gamestates/GSLevel.h"
#include "chat/ChatManager.h"
#include <vector>

// ! HACK
#include "infos/PlayerInfo.h"

#include "core/command/ConsoleCommand.h"
#include "worldentities/BigExplosion.h"

#include "gametypes/Deathmatch.h"
#include "tools/Timer.h"

namespace orxonox
{

    class _DodgeRaceExport DodgeRace : public Deathmatch
    {
       public:
            DodgeRace(Context* context);

            void init();

            virtual void start();
            virtual void end();

            virtual void tick(float dt);

            virtual void playerPreSpawn(PlayerInfo* player);

            void levelUp();

            int getLives(){return this->lives;}
            int getLevel(){return this->level;}
            int getPoints(){return this->point;}
            int getMultiplier(){return this->multiplier;}

            void setCenterpoint(DodgeRaceCenterPoint* center)
                       { this->center_ = center; }
            virtual void addBots(unsigned int amount){} //<! overwrite function in order to bypass the addbots command

            // checks if multiplier should be reset.
            void comboControll();
            void costLife();

            bool bEndGame;
            bool bShowLevel;
            int lives;
            int multiplier;
            float counter;
            int pattern;
            float currentPosition;
            float lastPosition;

       private:
            Timer endGameTimer;

            WeakPtr<DodgeRaceShip> getPlayer();
            WeakPtr<DodgeRaceShip> player;
            std::vector<DodgeRaceCube*> cubeList;
            void toggleShowLevel(){bShowLevel = !bShowLevel;}
            void addPoints(int numPoints);

            WeakPtr<DodgeRaceCenterPoint> center_;
            int level;
            int point;
            bool b_combo;

            Timer enemySpawnTimer;
            Timer comboTimer;
            Timer showLevelTimer;


         /*

            //void spawnEnemy();












        private:




            //Context* context;
            */
    };
}

#endif /* _DodgeRace_H__ */
