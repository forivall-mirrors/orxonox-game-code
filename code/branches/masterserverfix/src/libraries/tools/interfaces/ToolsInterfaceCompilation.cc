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
    Compiles all the interfaces in the tools library with mostly just a constructor.
*/

#include "Tickable.h"
#include "TimeFactorListener.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"

namespace orxonox
{
    //----------------------------
    // TimeFactorListener
    //----------------------------
    float TimeFactorListener::timefactor_s = 1.0f;

    TimeFactorListener::TimeFactorListener()
    {
        RegisterRootObject(TimeFactorListener);
    }

    /*static*/ void TimeFactorListener::setTimeFactor(float factor)
    {
        if (factor == TimeFactorListener::timefactor_s)
            return;

        float oldFactor = TimeFactorListener::timefactor_s;
        TimeFactorListener::timefactor_s = factor;
        for (ObjectList<TimeFactorListener>::iterator it = ObjectList<TimeFactorListener>::begin(); it != ObjectList<TimeFactorListener>::end(); ++it)
            it->changedTimeFactor(factor, oldFactor);
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
}
