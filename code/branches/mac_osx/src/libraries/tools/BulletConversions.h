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

#ifndef _BulletConversions_H__
#define _BulletConversions_H__

#include "tools/ToolsPrereqs.h"

#include "util/Convert.h"
#include "util/Math.h"
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btVector3.h>

namespace orxonox
{
    // Vector3 to btVector3
    template <>
    struct ConverterExplicit<orxonox::Vector3, btVector3>
    {
        ORX_FORCEINLINE static bool convert(btVector3* output, const orxonox::Vector3& input)
        {
            output->setX(input.x);
            output->setY(input.y);
            output->setZ(input.z);
            return true;
        }
    };

    // btVector3 to Vector3
    template <>
    struct ConverterExplicit<btVector3, orxonox::Vector3>
    {
        ORX_FORCEINLINE static bool convert(orxonox::Vector3* output, const btVector3& input)
        {
            output->x = input.x();
            output->y = input.y();
            output->z = input.z();
            return true;
        }
    };

    // Quaternion to btQuaternion
    template <>
    struct ConverterExplicit<orxonox::Quaternion, btQuaternion>
    {
        ORX_FORCEINLINE static bool convert(btQuaternion* output, const orxonox::Quaternion& input)
        {
            output->setW(input.w);
            output->setX(input.x);
            output->setY(input.y);
            output->setZ(input.z);
            return true;
        }
    };

    // btQuaternion to Vector3
    template <>
    struct ConverterExplicit<btQuaternion, orxonox::Quaternion>
    {
        ORX_FORCEINLINE static bool convert(orxonox::Quaternion* output, const btQuaternion& input)
        {
            output->w = input.w();
            output->x = input.x();
            output->y = input.y();
            output->z = input.z();
            return true;
        }
    };
}

#endif /* _BulletConversions_H__ */
