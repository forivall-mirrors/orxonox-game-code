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
    @file SphereCollisionShape.cc
    @brief Implementation of the SphereCollisionShape class.
*/

#include "SphereCollisionShape.h"

#include <BulletCollision/CollisionShapes/btSphereShape.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    RegisterClass(SphereCollisionShape);

    /**
    @brief
        Constructor. registers and initializes the object.
    */
    SphereCollisionShape::SphereCollisionShape(Context* context) : CollisionShape(context)
    {
        RegisterObject(SphereCollisionShape);

        this->radius_ = 1.0f;
        updateShape();

        this->registerVariables();
    }

    void SphereCollisionShape::registerVariables()
    {
        registerVariable(this->radius_, VariableDirection::ToClient, new NetworkCallback<CollisionShape>(this, &CollisionShape::updateShape));
    }

    void SphereCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SphereCollisionShape, XMLPort, xmlelement, mode);

        XMLPortParam(SphereCollisionShape, "radius", setRadius, getRadius, xmlelement, mode);
    }

    /**
    @brief
        Is called when the scale of the SphereCollisionShape has changed.
    */
    void SphereCollisionShape::changedScale()
    {
        CollisionShape::changedScale();

        // Resize the internal collision shape
        // TODO: Assuming setLocalScaling works.
        //this->collisionShape_->setLocalScaling(multi_cast<btVector3>(this->getScale3D()));
        if(!this->hasUniformScaling())
        {
            orxout(internal_error) << "SphereCollisionShape: Non-uniform scaling is not yet supported." << endl;
            return;
        }

        this->setRadius(this->radius_*this->getScale());
    }

    /**
    @brief
        Creates a new internal collision shape for the SphereCollisionShape.
    @return
        Returns a pointer to the newly created btSphereShape.
    */
    btCollisionShape* SphereCollisionShape::createNewShape() const
    {
        return new btSphereShape(this->radius_);
    }
}
