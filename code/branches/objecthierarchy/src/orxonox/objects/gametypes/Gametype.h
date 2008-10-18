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

#ifndef _Gametype_H__
#define _Gametype_H__

#include "OrxonoxPrereqs.h"

#include "core/BaseObject.h"
#include "network/ClientConnectionListener.h"

namespace orxonox
{
    class _OrxonoxExport Gametype : public BaseObject, public network::ClientConnectionListener
    {
        public:
            Gametype();
            virtual ~Gametype() {}

            static Gametype* getCurrentGametype();
            void addPlayer(PlayerInfo* player);
            void removePlayer(PlayerInfo* player);

        protected:
            virtual void clientConnected(unsigned int clientID);
            virtual void clientDisconnected(unsigned int clientID);

            virtual void playerJoined(PlayerInfo* player);
            virtual void playerLeft(PlayerInfo* player);

        private:
            std::set<PlayerInfo*> players_;
    };
}

#endif /* _Gametype_H__ */
