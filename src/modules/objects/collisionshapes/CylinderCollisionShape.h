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

/**
    @file CylinderCollisionShape.h
    @brief Definition of the CylinderCollisionShape class.
    @ingroup Collisionshapes
*/

#ifndef _CylinderCollisionShape_H__
#define _CylinderCollisionShape_H__

#include "objects/ObjectsPrereqs.h"
#include "AbstractRadiusHeightCollisionShape.h"

namespace orxonox
{

    /**
    @brief
        Wrapper for the bullet Cylinder collision shape class btCylinderShape.

    @see btCylinderShape
    @ingroup Collisionshapes
    */
    class _ObjectsExport CylinderCollisionShape : public AbstractRadiusHeightCollisionShape
    {
        public:
            CylinderCollisionShape(Context* context);
            virtual ~CylinderCollisionShape();

        private:
            btCollisionShape* createNewShape() const; // Creates a new internal collision shape for the CylinderCollisionShape.
     };
}

#endif /* _CylinderCollisionShape_H__ */
