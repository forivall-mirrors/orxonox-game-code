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
 *      Martin Stypinski
 *   Co-authors:
 *      ...
 *
 */

/**
    @file PlaneCollisionShape.cc
    @brief Implementation of the PlaneCollisionShape class.
*/

#include "PlaneCollisionShape.h"

#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    RegisterClass(PlaneCollisionShape);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    PlaneCollisionShape::PlaneCollisionShape(Context* context) : CollisionShape(context)
    {
        RegisterObject(PlaneCollisionShape);

        this->normal_ = Vector3(0, 1, 0);
        this->offset_ = 0.0f;
        updateShape();

        this->registerVariables();
    }

    PlaneCollisionShape::~PlaneCollisionShape()
    {
        if (this->isInitialized())
            delete this->collisionShape_;
    }

    void PlaneCollisionShape::registerVariables()
    {
        registerVariable(this->normal_, VariableDirection::ToClient, new NetworkCallback<CollisionShape>(this, &CollisionShape::updateShape));
        registerVariable(this->offset_, VariableDirection::ToClient, new NetworkCallback<CollisionShape>(this, &CollisionShape::updateShape));
    }

    void PlaneCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PlaneCollisionShape, XMLPort, xmlelement, mode);

        XMLPortParam(PlaneCollisionShape, "normal", setNormal, getNormal, xmlelement, mode);
        XMLPortParam(PlaneCollisionShape, "offset", setOffset, getOffset, xmlelement, mode);
    }

    /**
    @brief
        Is called when the scale of the PlaneCollisionShape has changed.
    */
    void PlaneCollisionShape::changedScale()
    {
        CollisionShape::changedScale();

        // Resize the internal collision shape
        // TODO: Assuming setLocalScaling works.
        //this->collisionShape_->setLocalScaling(multi_cast<btVector3>(this->getScale3D()));
        if(!this->hasUniformScaling())
        {
            orxout(internal_error) << "PlaneCollisionShape: Non-uniform scaling is not yet supported." << endl;
            return;
        }

        this->setOffset(this->offset_*this->getScale());
    }

    /**
    @brief
        Creates a new internal collision shape for the PlaneCollisionShape.
    @return
        Returns a pointer to the newly created btStaticPlaneShape.
    */
    btCollisionShape* PlaneCollisionShape::createNewShape() const
    {
        return new btStaticPlaneShape(multi_cast<btVector3>(this->normal_), this->offset_);
    }
}
