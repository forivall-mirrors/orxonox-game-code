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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    Compiles all the interfaces in the orxonox library with mostly just a constructor.
*/

#include "GametypeMessageListener.h"
#include "PlayerTrigger.h"
#include "RadarListener.h"
#include "Rewardable.h"
#include "TeamColourable.h"
#include "NotificationListener.h"

#include "core/CoreIncludes.h"

namespace orxonox
{
    //----------------------------
    // GametypeMessageListener
    //----------------------------
    GametypeMessageListener::GametypeMessageListener()
    {
        RegisterRootObject(GametypeMessageListener);
    }

    //----------------------------
    // PlayerTrigger
    //----------------------------
    PlayerTrigger::PlayerTrigger()
    {
        RegisterRootObject(PlayerTrigger);

        this->player_ = NULL;
        this->isForPlayer_ = false;
    }

    //----------------------------
    // RadarListener
    //----------------------------
    RadarListener::RadarListener()
    {
        RegisterRootObject(RadarListener);
    }

    //----------------------------
    // TeamColourable
    //----------------------------
    TeamColourable::TeamColourable()
    {
        RegisterRootObject(TeamColourable);
    }

    //----------------------------
    // Rewardable
    //----------------------------
    Rewardable::Rewardable()
    {
        RegisterRootObject(Rewardable);
    }

    //----------------------------
    // NotificationListener
    //----------------------------
    NotificationListener::NotificationListener()
    {
        RegisterRootObject(NotificationListener);
    }
}
