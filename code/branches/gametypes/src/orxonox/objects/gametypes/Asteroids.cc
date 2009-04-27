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
 *      Aurelian
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Asteroids.h"

#include "core/CoreIncludes.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/worldentities/pawns/Pawn.h"

#include "network/Host.h"

namespace orxonox
{
    CreateUnloadableFactory(Asteroids);

    Asteroids::Asteroids(BaseObject* creator) : Gametype(creator)
    {
        RegisterObject(Asteroids);
    }

    void Asteroids::start()
    {
        Gametype::start();

        std::string message = "The match has started! Reach the first chekpoint within 60 seconds! But be aware, there may be pirates around...";
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }

    void Asteroids::end()
    {
        Gametype::end();

        std::string message = "The match has ended.";
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }
}
