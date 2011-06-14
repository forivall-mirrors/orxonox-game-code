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
    @file CollisionShape.cc
    @brief Implementation of the CollisionShape class.
*/

#include "CollisionShape.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/WorldEntity.h"
#include "CompoundCollisionShape.h"
#include "WorldEntityCollisionShape.h"

namespace orxonox
{

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    CollisionShape::CollisionShape(BaseObject* creator)
        : BaseObject(creator)
        , Synchronisable(creator)
    {
        RegisterObject(CollisionShape);

        this->parent_ = 0;
        this->parentID_ = OBJECTID_UNKNOWN;
        this->collisionShape_ = 0;
        this->position_ = Vector3::ZERO;
        this->orientation_ = Quaternion::IDENTITY;
        this->scale_ = Vector3::UNIT_SCALE;
        this->uniformScale_ = true;

        this->registerVariables();
    }

    /**
    @brief
        Destructor. Detaches the CollisionShape from its parent.
    */
    CollisionShape::~CollisionShape()
    {
        // Detach from parent CompoundCollisionShape.
        if (this->isInitialized() && this->parent_)
            this->parent_->detach(this);
    }

    void CollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CollisionShape, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(CollisionShape, "position", setPosition, getPosition, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(CollisionShape, "orientation", setOrientation, getOrientation, xmlelement, mode, const Quaternion&);
        XMLPortParamTemplate(CollisionShape, "scale3D", setScale3D, getScale3D, xmlelement, mode, const Vector3&);
        XMLPortParamLoadOnly(CollisionShape, "scale", setScale, xmlelement, mode);
        XMLPortParamLoadOnly(CollisionShape, "yaw",   yaw,   xmlelement, mode);
        XMLPortParamLoadOnly(CollisionShape, "pitch", pitch, xmlelement, mode);
        XMLPortParamLoadOnly(CollisionShape, "roll",  roll,  xmlelement, mode);
    }

    /**
    @brief
        Register variables that need synchronizing over the network.
    */
    void CollisionShape::registerVariables()
    {
        // Keep the shape's parent (can be either a CompoundCollisionShape or a WorldEntity) consistent over the network.
        registerVariable(this->parentID_, VariableDirection::ToClient, new NetworkCallback<CollisionShape>(this, &CollisionShape::parentChanged));
    }

    /**
    @brief
        Notifies the CollisionShape of being attached to a CompoundCollisionShape.
    @param newParent
        A pointer to the CompoundCollisionShape the CollisionShape was attached to.
    @return
        Returns 
    */
    bool CollisionShape::notifyBeingAttached(CompoundCollisionShape* newParent)
    {
        // If the CollisionShape is attached to a CompoundCollisionShapes, detach it.
        if (this->parent_)
            this->parent_->detach(this);

        this->parent_ = newParent;

        // If the new parent is a WorldEntityCollisionShape, the parentID is set to the objectID of the WorldEntity that is its owner.
        // TODO: Why?
        WorldEntityCollisionShape* parentWECCS = orxonox_cast<WorldEntityCollisionShape*>(newParent);
        if (parentWECCS)
            this->parentID_ = parentWECCS->getWorldEntityOwner()->getObjectID();
        // Else it is set to the objectID of the CompoundCollisionShape.
        else
            this->parentID_ = newParent->getObjectID();

        return true;
    }

    /**
    @brief
        Notifies the CollisionShape of being detached from a CompoundCollisionShape.
    */
    void CollisionShape::notifyDetached()
    {
        this->parent_ = 0;
        this->parentID_ = OBJECTID_UNKNOWN;
    }

    /**
    @brief
        Updates the CompoundCollisionShape the CollisionShape belongs to (if it belongs to one), after the CollisionShape has changed.
    */
    void CollisionShape::updateParent()
    {
        if (this->parent_)
            this->parent_->updateAttachedShape(this);
    }

    /**
    @brief
        Is called when the parentID of the CollisionShape has changed.
        Attaches it to the object with the changed parentID, which can either be a CompoundCollisionShape or a WorldEntity.
    */
    void CollisionShape::parentChanged()
    {
        // Get the parent object from the network.
        Synchronisable* parent = Synchronisable::getSynchronisable(this->parentID_);

        // Parent can either be a WorldEntity or a CompoundCollisionShape. The reason is that the
        // internal collision shape (which is compound) of a WE doesn't get synchronised.
        CompoundCollisionShape* parentCCS = orxonox_cast<CompoundCollisionShape*>(parent);

        // If the parent is a CompoundCollisionShape, attach the CollisionShape to it.
        if (parentCCS)
            parentCCS->attach(this);
        else
        {
            // If the parent is a WorldEntity, attach the CollisionShape to its collision shapes.
            WorldEntity* parentWE = orxonox_cast<WorldEntity*>(parent);
            if (parentWE)
                parentWE->attachCollisionShape(this);
        }
    }

    /**
    @brief
        Check whether the CollisionShape has been either moved or rotated or both. (i.e. it doesn't have position zero and identity orientation any more)
    @return
        Returns true if it has been moved.
    */
    bool CollisionShape::hasTransform() const
    {
        return (!this->position_.positionEquals(Vector3(0, 0, 0), 0.001f) ||
                !this->orientation_.equals(Quaternion(1,0,0,0), Degree(0.1f)));
    }

    /**
    @brief
        Set the scale of the CollisionShape.
        Since the scale is a vector the CollisionShape can be scaled independently in each direction, allowing for linear distortions.
        If the scale changes, this causes the parent CompoundCollisionShape (if there is one) to be updated.
        Beware, non-uniform scaling (i.e. distortions) might not be supported by all CollisionShapes.
    @param scale
        The new scale to be set. Vector3::UNIT_SCALE is the initial scale.
    */
    void CollisionShape::setScale3D(const Vector3& scale)
    {
        if(this->scale_ == scale)
            return;

        // If the vectors are not in the same direction, then this is no longer a uniform scaling.
        if(scale_.crossProduct(scale).squaredLength() != 0.0f)
        {
            CCOUT(2) << "Warning: Non-uniform scaling is not yet supported." << endl;
            return;
        }

        this->scale_ = scale;

        this->changedScale();
        this->updateParent();
    }

    /**
    @brief
        Set the (uniform) scale of the CollisionShape.
        If the scale changes, this causes the parent CompoundCollisionShape (if there is one) to be updated.
    @param scale
        The scale to scale the CollisionShape with. 1.0f is the initial scale.
    */
    void CollisionShape::setScale(float scale)
    {
        if(this->scale_.length() == scale)
            return;

        this->scale_ = Vector3::UNIT_SCALE*scale;

        this->changedScale();
        this->updateParent();
    }

    /**
    @brief
        Is called when the scale of the CollisionShape has changed.
    */
    void CollisionShape::changedScale()
    {
        // Adjust the position of the CollisionShape.
        this->position_ *= this->getScale3D();
    }

    /**
    @brief
        Updates the shape.
        Is called when the internal parameters of the shape have changed such that a new shape needs to be created.
    */
    void CollisionShape::updateShape()
    {
        btCollisionShape* oldShape = this->collisionShape_;
        this->collisionShape_ = this->createNewShape();
        // If the CollisionShape has been rescaled, scale the shape to fit the current scale.
        if(this->scale_ != Vector3::UNIT_SCALE)
            this->changedScale();
        // When we recreate the shape, we have to inform the parent about this to update the shape
        this->updateParent();
        if (oldShape)
            delete oldShape;
    }

    /**
    @brief
        Calculates the local inertia of the collision shape.
    @todo
        Document.
    */
    void CollisionShape::calculateLocalInertia(float mass, btVector3& inertia) const
    {
        if (this->collisionShape_)
            this->collisionShape_->calculateLocalInertia(mass, inertia);
        else
            inertia.setValue(0, 0, 0);
    }
}
