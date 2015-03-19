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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
*/

/**
    @file DistanceTriggerBeacon.h
    @brief Definition of the DistanceTriggerBeacon class.
    @ingroup Triggers
*/

#ifndef _DistanceTriggerBeacon_H__
#define _DistanceTriggerBeacon_H__

#include "objects/ObjectsPrereqs.h"
#include "core/XMLPort.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{

    /**
    @brief
        A DistanceTriggerBeacon can be used together with a @ref orxonox::DistanceTrigger "DistanceTrigger" or a @ref orxonox::DistanceMultiTrigger "DistanceMultiTrigger" to make them only react to specific objects.
        This can be done by attaching a DistanceTriggerBeacon to an object, giving it a unique name and setting the targetname in the @ref orxonox::DistanceTrigger "DistanceTrigger" (or @ref orxonox::DistanceMultiTrigger "DistanceMultiTrigger") to that name and the target to DistanceTriggerBeacon.

    @author
        Damian 'Mozork' Frick

    @ingroup Triggers
    */
    class _ObjectsExport DistanceTriggerBeacon : public StaticEntity
    {

        public:
            DistanceTriggerBeacon(Context* context); //!< Constructor.
            virtual ~DistanceTriggerBeacon() {} //!< Destructor.

    };

}

#endif // _DistanceTriggerBeacon_H__
