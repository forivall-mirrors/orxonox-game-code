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

#ifndef _Deathmatch_H__
#define _Deathmatch_H__

#include "OrxonoxPrereqs.h"
#include "Gametype.h"

namespace orxonox
{
    class _OrxonoxExport Deathmatch : public Gametype
    {
        public:
            Deathmatch(BaseObject* creator);
            virtual ~Deathmatch() {}

            virtual void start();
            virtual void end();
            virtual void playerEntered(PlayerInfo* player);
            virtual bool playerLeft(PlayerInfo* player);
            virtual bool playerChangedName(PlayerInfo* player);

            virtual void pawnKilled(Pawn* victim, Pawn* killer = 0);
            virtual void playerScored(PlayerInfo* player, int score = 1);
    };
}

#endif /* _Deathmatch_H__ */
