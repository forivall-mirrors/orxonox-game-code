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
 *      Johannes Ritz
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Mission_H__
#define _Mission_H__

#include "OrxonoxPrereqs.h"
//#include "Gametype.h"
#include "TeamGametype.h"

namespace orxonox
{
    class _OrxonoxExport Mission : public TeamGametype
    {
        public:
            Mission(BaseObject* creator);
            virtual ~Mission() {}

            virtual void tick(float dt);

            virtual void start();
            virtual void end();
            virtual void addBots(unsigned int amount){} //<! overwrite function in order to bypass the addbots command
            inline void setLives(unsigned int amount)
                {this->lives_ = amount;}
	    inline unsigned int getLives()
                {return this->lives_;}

        protected:
            virtual void pawnKilled(Pawn* victim, Pawn* killer = 0);
            bool missionAccomplished_; //<! indicates if player successfully finsihed the mission;
            int lives_; //<! amount of player's lives <-> nr. of retries

    };
}

#endif /* _Mission_H__ */

