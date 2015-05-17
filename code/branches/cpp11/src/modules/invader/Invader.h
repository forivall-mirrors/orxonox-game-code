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
    @file Invader.h
    @brief Gametype.
    @ingroup Invader
*/

#ifndef _Invader_H__
#define _Invader_H__

#include "invader/InvaderPrereqs.h"

#include "gametypes/Deathmatch.h"
#include "tools/Timer.h"

namespace orxonox
{

    class _InvaderExport Invader : public Deathmatch
    {
        public:
            Invader(Context* context);

            virtual void start();
            virtual void end();
            virtual void addBots(unsigned int amount){} //<! overwrite function in order to bypass the addbots command

            void spawnEnemy();

            void setCenterpoint(InvaderCenterPoint* center);

            int getLives(){return this->lives;}
            int getLevel(){return this->level;}
            int getPoints(){return this->point;}
            int getMultiplier(){return this->multiplier;}

            void costLife();
            void levelUp();
            void addPoints(int numPoints);
            // checks if multiplier should be reset.
            void comboControll();
            void init();
            int lives;
            int multiplier;
            bool bEndGame;
            bool bShowLevel;
        private:
            void toggleShowLevel(){bShowLevel = !bShowLevel;}
            WeakPtr<InvaderShip> getPlayer();
            WeakPtr<InvaderCenterPoint> center_;
            WeakPtr<InvaderShip> player;

            Timer enemySpawnTimer;
            Timer comboTimer;
            Timer showLevelTimer;
            //Context* context;
            int level;
            int point;
            bool b_combo;
    };
}

#endif /* _Invader_H__ */
