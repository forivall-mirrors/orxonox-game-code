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
*/

#include "OrxonoxStableHeaders.h"
#include "Radar.h"
#include "objects/WorldEntity.h"
#include "overlays/hud/RadarViewable.h"

#include "core/CoreIncludes.h"
//#include "core/ConfigValueIncludes.h"

namespace orxonox
{
    RadarListener::RadarListener()
    {
        RegisterRootObject(RadarListener);
    }


    Radar* Radar::instance_s = 0;

    Radar::Radar()
    {
        assert(instance_s == 0);
        instance_s = this;
    }

    Radar::~Radar()
    {
        instance_s = 0;
    }

    Radar& Radar::getInstance()
    {
        assert(instance_s);
        return *instance_s;
    }

    void Radar::unregisterObject(RadarViewable* object)
    {
        // TODO: check for focus
    }

    void Radar::tick(float dt)
    {
        for (Iterator<RadarListener> itListener = ObjectList<RadarListener>::begin(); itListener; ++itListener)
        {
            for (Iterator<RadarViewable> itElement = ObjectList<RadarViewable>::begin(); itElement; ++itElement)
            {
                if ((*itListener)->getRadarSensitivity() > (*itElement)->getRadarObjectVisibility())
                    (*itListener)->displayObject(*itElement);
            }
        }
    }
}
