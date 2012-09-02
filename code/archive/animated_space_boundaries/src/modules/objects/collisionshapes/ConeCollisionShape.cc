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
    @file ConeCollisionShape.cc
    @brief Implementation of the ConeCollisionShape class.
*/

#include "ConeCollisionShape.h"

#include <BulletCollision/CollisionShapes/btConeShape.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    CreateFactory(ConeCollisionShape);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    ConeCollisionShape::ConeCollisionShape(BaseObject* creator) : CollisionShape(creator)
    {
        RegisterObject(ConeCollisionShape);

        this->radius_ = 1.0f;
        this->height_ = 1.0f;
        updateShape();

        this->registerVariables();
    }

    ConeCollisionShape::~ConeCollisionShape()
    {
        if (this->isInitialized())
            delete this->collisionShape_;
    }

    void ConeCollisionShape::registerVariables()
    {
        registerVariable(this->radius_, VariableDirection::ToClient, new NetworkCallback<CollisionShape>(this, &CollisionShape::updateShape));
        registerVariable(this->height_, VariableDirection::ToClient, new NetworkCallback<CollisionShape>(this, &CollisionShape::updateShape));
    }

    void ConeCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ConeCollisionShape, XMLPort, xmlelement, mode);

        XMLPortParam(ConeCollisionShape, "radius", setRadius, getRadius, xmlelement, mode);
        XMLPortParam(ConeCollisionShape, "height", setHeight, getHeight, xmlelement, mode);
    }

    /**
    @brief
        Is called when the scale of the ConeCollisionShape has changed.
    */
    void ConeCollisionShape::changedScale()
    {
        CollisionShape::changedScale();

        // Resize the internal collision shape
        // TODO: Assuming setLocalScaling works.
        //this->collisionShape_->setLocalScaling(multi_cast<btVector3>(this->getScale3D()));
        if(!this->hasUniformScaling())
        {
            CCOUT(1) << "Error: Non-uniform scaling is not yet supported." << endl;
            return;
        }

        this->radius_ *= this->getScale();
        this->height_ *= this->getScale();
        this->updateShape();
    }

    /**
    @brief
        Creates a new internal collision shape for the ConeCollisionShape.
    @return
        Returns a pointer to the newly created btConeShape.
    */
    btCollisionShape* ConeCollisionShape::createNewShape() const
    {
        return new btConeShape(this->radius_, this->height_);
    }
}
