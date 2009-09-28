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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Pong_H__
#define _Pong_H__

#include "pong/PongPrereqs.h"

#include "tools/Timer.h"
#include "gametypes/Deathmatch.h"

namespace orxonox
{
    class _PongExport Pong : public Deathmatch
    {
        public:
            Pong(BaseObject* creator);
            virtual ~Pong() {}

            virtual void start();
            virtual void end();

            virtual void spawnPlayer(PlayerInfo* player);

            virtual void playerScored(PlayerInfo* player);

            void setCenterpoint(PongCenterpoint* center)
                { this->center_ = center; }

            PlayerInfo* getLeftPlayer() const;
            PlayerInfo* getRightPlayer() const;

        protected:
            void startBall();

            PongCenterpoint* center_;
            PongBall* ball_;
            PongBat* bat_[2];
            Timer starttimer_;
    };
}

#endif /* _Pong_H__ */
