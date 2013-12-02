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
    @brief Declaration of the Invader class.
    @ingroup Invader
*/

#ifndef _Invader_H__
#define _Invader_H__

#include "invader/InvaderPrereqs.h"

#include "gametypes/Deathmatch.h"

#include "InvaderCenterPoint.h"

#include "tools/Timer.h"

namespace orxonox
{

    class _InvaderExport Invader : public Deathmatch
    {
        public:
            Invader(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~Invader(); //!< Destructor. Cleans up, if initialized.

            virtual void start(); //!< Starts the Invader minigame.
            virtual void end(); ///!< Ends the Invader minigame.

            //virtual void spawnPlayer(PlayerInfo* player); //!< Spawns the input player.

            void spawnEnemy();

            void setCenterpoint(InvaderCenterPoint* center)
            { this->center_ = center; }

            int getLives(){return this->lives;}
            int getLevel(){return this->level;}
            int getPoints(){return this->point;}

            void costLife(){lives--; if (lives <= 0) end();};
            void levelUp(){level++;}
            void addPoints(int numPoints){point += numPoints * multiplier; b_combo = true;}
            void comboControll();
        private:
            WeakPtr<InvaderCenterPoint> center_;
            WeakPtr<InvaderShip> player;

            ConsoleCommand* console_addEnemy;
            Timer enemySpawnTimer;
            Timer comboTimer;
            //Context* context;
            int lives;
            int level;
            int point;
            bool b_combo;
            int multiplier;
    };
}

#endif /* _Invader_H__ */
