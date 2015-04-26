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
#include "TowerDefenseSelecter.h"
#include "towerdefense/TowerDefensePrereqs.h"
#include "gametypes/TeamDeathmatch.h"
#include "TowerDefenseEnemy.h"
#include "util/Output.h"
#include "core/object/WeakPtr.h"
#include "TowerDefenseSelecter.h"


namespace orxonox
{
    class _TowerDefenseExport TowerDefense : public TeamDeathmatch
    {
    public:
        TowerDefense(Context* context);
        virtual ~TowerDefense();

        std::vector<orxonox::WeakPtr<TowerDefenseEnemy> > TowerDefenseEnemyvector;
        Model* towerModelMatrix[16][16];
        TowerDefenseTower* towerTurretMatrix[16][16];
        void addTowerDefenseEnemy(std::vector<TDCoordinate*> path, int templatenr);
        virtual void start(); //<! The function is called when the gametype starts
        virtual void end();
        virtual void tick(float dt);
        //virtual void playerEntered(PlayerInfo* player);
        //virtual bool playerLeft(PlayerInfo* player);
        //Player Stats (set,get, reduce)
        int getCredit(){ return this->credit_; }
        int getLifes(){ return this->lifes_; }
        int getWaveNumber(){ return this->waves_; }
        void setCredit(int credit){ credit_ = credit; }
        void setLifes(int lifes){ lifes_ = lifes; }
        void setWaveNumber(int wavenumber){ waves_=wavenumber; }
        void buyTower(int cost){ credit_ -= cost;}
        void addCredit(int credit) { credit_+=credit; }
        void nextwave(){ TowerDefenseEnemyvector.clear(); waves_++; time=0;}
        int reduceLifes(int NumberofLifes){ return lifes_-=NumberofLifes; }
        TDCoordinate* selectedPos;
        TowerDefenseSelecter* selecter;

        //virtual void pawnKilled(Pawn* victim, Pawn* killer = 0);
        //virtual void playerScored(PlayerInfo* player, int score);


        /*  Called by TowerDefenseCenterpoint upon game start
            The centerpoint is used to create towers
        */
        void setCenterpoint(TowerDefenseCenterpoint *centerpoint);

        /* Adds a tower at x, y in the playfield */
        void addTower(int x, int y);

        void upgradeTower(int x, int y);

        //TODO: void spawnNewWave()
        //TODO: create a timer which regularly calls the spawnNewWave function  (time driven)
        //      or spawn a new wave when the old wave has been killed           (event driven)


    private:
        TowerDefenseCenterpoint *center_;
        float time;
//        float time2;
        int credit_;
        int waves_;
        int lifes_;
        Timer nextwaveTimer_;

        /* handles stats */
        bool hasEnoughCreditForTower(int towerCost);
        bool hasEnoughCreditForUpgrade();



        std::vector<TowerTurret*> towers_;
    };
}

#endif /* _TowerDefense_H__ */
