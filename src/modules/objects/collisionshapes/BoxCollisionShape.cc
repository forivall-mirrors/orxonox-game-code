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
    @file BoxCollisionShape.cc
    @brief Implementation of the BoxCollisionShape class.
*/

#include "BoxCollisionShape.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    RegisterClass(BoxCollisionShape);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    BoxCollisionShape::BoxCollisionShape(Context* context) : CollisionShape(context)
    {
        RegisterObject(BoxCollisionShape);

        this->halfExtents_ = Vector3(1, 1, 1);
        this->updateShape();

        this->registerVariables();
    }

    BoxCollisionShape::~BoxCollisionShape()
    {
        // TODO: Move to CollisionShape?
        if (this->isInitialized())
            delete this->collisionShape_;
    }

    void BoxCollisionShape::registerVariables()
    {
        registerVariable(this->halfExtents_, VariableDirection::ToClient, new NetworkCallback<CollisionShape>(this, &CollisionShape::updateShape));
    }

    void BoxCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(BoxCollisionShape, XMLPort, xmlelement, mode);

        XMLPortParam(BoxCollisionShape, "halfExtents",  setHalfExtents,  getHalfExtents,  xmlelement, mode);
        XMLPortParamLoadOnly(BoxCollisionShape, "width",  setWidth,  xmlelement, mode);
        XMLPortParamLoadOnly(BoxCollisionShape, "height", setHeight, xmlelement, mode);
        XMLPortParamLoadOnly(BoxCollisionShape, "length", setLength, xmlelement, mode);
    }

    /**
    @brief
        Is called when the scale of the BoxCollisionShape has changed.
    */
    void BoxCollisionShape::changedScale()
    {
        CollisionShape::changedScale();

        // Resize the internal collision shape
        // TODO: Assuming setLocalScaling works.
        // this->collisionShape_->setLocalScaling(multi_cast<btVector3>(this->getScale3D()));
        if(!this->hasUniformScaling())
        {
            orxout(internal_error) << "BoxCollisionShape: Non-uniform scaling is not yet supported." << endl;
            return;
        }

        this->setHalfExtents(this->halfExtents_ * this->getScale());
    }

    /**
    @brief
        Creates a new internal collision shape for the BoxCollisionShape.
    @return
        Returns a pointer to the newly created btBoxShape.
    */
    btCollisionShape* BoxCollisionShape::createNewShape() const
    {
        return new btBoxShape(multi_cast<btVector3>(this->halfExtents_));
    }
}
