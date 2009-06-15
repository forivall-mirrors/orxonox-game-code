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
    Compiles all the interfaces (except RadarViewable) with mostly just a constructor.
*/

#include "GametypeMessageListener.h"
#include "PawnListener.h"
#include "RadarListener.h"
#include "Rewardable.h"
#include "Teamcolourable.h"
#include "Tickable.h"
#include "TimeFactorListener.h"
#include "WindowEventListener.h"

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
    // PawnListener
    //----------------------------
    /**
        @brief Constructor for the PawnListener.
    */
    PawnListener::PawnListener()
    {
        RegisterRootObject(PawnListener);
    }

    //----------------------------
    // RadarListener
    //----------------------------
    RadarListener::RadarListener()
    {
        RegisterRootObject(RadarListener);
    }

    //----------------------------
    // Teamcolourable
    //----------------------------
    Teamcolourable::Teamcolourable()
    {
        RegisterRootObject(Teamcolourable);
    }

    //----------------------------
    // TimeFactorListener
    //----------------------------
    float TimeFactorListener::timefactor_s = 1.0f;

    TimeFactorListener::TimeFactorListener()
    {
        RegisterRootObject(TimeFactorListener);
    }

    //----------------------------
    // Tickable
    //----------------------------
    /**
        @brief Constructor: Registers the object in the Tickable-list
    */
    Tickable::Tickable()
    {
        RegisterRootObject(Tickable);
    }

    //----------------------------
    // WindowEventListener
    //----------------------------
    /**
        @brief Constructor for the WindowEventListener.
    */
    WindowEventListener::WindowEventListener()
    {
        RegisterRootObject(WindowEventListener);
    }

    //----------------------------
    // Rewardable
    //----------------------------
    Rewardable::Rewardable()
    {
        RegisterObject(Rewardable);
    }
}
