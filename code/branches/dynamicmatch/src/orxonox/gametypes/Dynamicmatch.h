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

#ifndef _Dynamicmatch_H__
#define _Dynamicmatch_H__

#include "OrxonoxPrereqs.h"
#include "Gametype.h"
#include "tools/Timer.h"

namespace orxonox
{
    class _OrxonoxExport Dynamicmatch : public Gametype
    {
        public:
            Dynamicmatch(BaseObject* creator);
            virtual ~Dynamicmatch() {}

		virtual bool allowPawnDamage(Pawn* victim, Pawn* originator = 0);
            	virtual bool allowPawnDeath(Pawn* victim, Pawn* originator = 0);
            virtual void start();

		  
            virtual void end();
            virtual void playerEntered(PlayerInfo* player);
            virtual bool playerLeft(PlayerInfo* player);
            virtual bool playerChangedName(PlayerInfo* player);

            virtual void pawnKilled(Pawn* victim, Pawn* killer = 0);
            virtual void playerScored(PlayerInfo* player);
		  virtual void showPoints();

		protected:
		  //points for each player
		  int pointsScored[50];//sorry hard coded - each player should be able to score
		  void winPoints();
		  Timer outputTimer_;
		  Timer scoreTimer_;      //?
    };
}

#endif /* _Dynamicmatch_H__ */
