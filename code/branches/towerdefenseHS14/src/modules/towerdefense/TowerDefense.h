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
 *
 *   Co-authors:
 *      ...
 *
 */

 /**
    @brief
        GameType class for TowerDefense. See TowerDefenseReadme.txt for Information.

    @ingroup TowerDefense
 */


#ifndef _TowerDefense_H__
#define _TowerDefense_H__
#include "TDCoordinate.h"
#include "towerdefense/TowerDefensePrereqs.h"
#include "gametypes/Deathmatch.h"
#include "TowerDefenseEnemy.h"
#include "util/Output.h"


 #include "TowerDefensePlayerStats.h"

namespace orxonox
{
    class _TowerDefenseExport TowerDefense : public Deathmatch
    {
    public:
        TowerDefense(Context* context);
        virtual ~TowerDefense();

        std::vector<TowerDefenseEnemy*> TowerDefenseEnemyvector;

        void addTowerDefenseEnemy(std::vector<TDCoordinate*> path);
        virtual void start(); //<! The function is called when the gametype starts
        virtual void end();
        virtual void tick(float dt);
        //virtual void playerEntered(PlayerInfo* player);
        //virtual bool playerLeft(PlayerInfo* player);

        int credits;
        int life;
        int waves;
        //virtual void pawnKilled(Pawn* victim, Pawn* killer = 0);
        //virtual void playerScored(PlayerInfo* player, int score);


        /*  Called by TowerDefenseCenterpoint upon game start
            The centerpoint is used to create towers
        */
        void setCenterpoint(TowerDefenseCenterpoint *centerpoint);

        /* Adds a tower at x, y in the playfield */
        void addTower(int x, int y);

        /* Part of a temporary hack to allow the player to add towers */
        ConsoleCommand* dedicatedAddTower_;

        //TODO: void spawnNewWave()
        //TODO: create a timer which regularly calls the spawnNewWave function  (time driven)
        //      or spawn a new wave when the old wave has been killed           (event driven)


    private:
        TowerDefenseCenterpoint *center_;
        float time;

        /* handles stats */
        TowerDefensePlayerStats *stats_;
        bool hasEnoughCreditForTower(TowerCost towerCost);

        bool towerExists(int x, int y);


        std::vector<TDCoordinate> addedTowersCoordinates_;
        std::vector<TowerTurret*> towers_;
    };
}

#endif /* _TowerDefense_H__ */
