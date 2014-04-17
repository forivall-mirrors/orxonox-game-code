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
    @file CompoundCollisionShape.cc
    @brief Implementation of the CompoundCollisionShape class.
*/

#include "CompoundCollisionShape.h"

#include <BulletCollision/CollisionShapes/btCompoundShape.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

#include "collisionshapes/WorldEntityCollisionShape.h"

namespace orxonox
{
    RegisterClass(CompoundCollisionShape);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    CompoundCollisionShape::CompoundCollisionShape(Context* context) : CollisionShape(context)
    {
        RegisterObject(CompoundCollisionShape);

        this->compoundShape_  = new btCompoundShape();
    }

    /**
    @brief
        Destructor.
        Deletes all its children.
    */
    CompoundCollisionShape::~CompoundCollisionShape()
    {
        if (this->isInitialized())
        {
            // Delete all children
            for (std::map<CollisionShape*, btCollisionShape*>::iterator it = this->attachedShapes_.begin();
                it != this->attachedShapes_.end(); ++it)
            {
                // make sure that the child doesn't want to detach itself --> speedup because of the missing update
                it->first->notifyDetached();
                it->first->destroy();
            }

            delete this->compoundShape_;
        }
    }

    void CompoundCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CompoundCollisionShape, XMLPort, xmlelement, mode);
        // Attached collision shapes
        XMLPortObject(CompoundCollisionShape, CollisionShape, "", attach, detach, xmlelement, mode);
    }

    /**
    @brief
        Attach the input CollisionShape to the CompoundCollisionShape.
    @param shape
        A pointer to the CollisionShape that is to be attached.
    */
    void CompoundCollisionShape::attach(CollisionShape* shape)
    {
        // If either the input shape is NULL or we try to attach the CollisionShape to itself.
        if (!shape || static_cast<CollisionShape*>(this) == shape)
            return;

        if (this->attachedShapes_.find(shape) != this->attachedShapes_.end())
        {
            orxout(internal_warning) << "Attaching a CollisionShape twice is not yet supported." << endl;
            return;
        }

        // Notify the CollisionShape that it is being attached to the CompoundCollisionShape.
        if (!shape->notifyBeingAttached(this))
            return;

        // Attach it.
        this->attachedShapes_[shape] = shape->getCollisionShape();

        // Only actually attach if we didn't pick a CompoundCollisionShape with no content.
        if (shape->getCollisionShape())
        {
            btTransform transf(multi_cast<btQuaternion>(shape->getOrientation()), multi_cast<btVector3>(shape->getPosition()));
            // Add the btCollisionShape of the CollisionShape as a child shape to the btCompoundShape of the CompoundCollisionShape.
            this->compoundShape_->addChildShape(transf, shape->getCollisionShape());

            this->updatePublicShape();
        }

        // If the shape to be attached is not a CompoundCollisionShape (thus an actual physical shape) & this is a WorldEntity's CollisionShape,
        // set it's userPointer to the WorldEntity this CompoundCollisionShape belongs to.
        if (!orxonox_cast<CompoundCollisionShape*>(shape) && orxonox_cast<WorldEntityCollisionShape*>(this))
            shape->getCollisionShape()->setUserPointer(orxonox_cast<WorldEntityCollisionShape*>(this)->getWorldEntityOwner());
    }

    /**
    @brief
        Detach the input CollisionShape form the CompoundCollisionShape.
    @param shape
        A pointer to the CollisionShape to be detached.
    */
    void CompoundCollisionShape::detach(CollisionShape* shape)
    {
        // If the input CollisionShape is actually attached.
        if (this->attachedShapes_.find(shape) != this->attachedShapes_.end())
        {
            this->attachedShapes_.erase(shape);
            if (shape->getCollisionShape())
                this->compoundShape_->removeChildShape(shape->getCollisionShape()); // TODO: Apparently this is broken?
            shape->notifyDetached();

            this->updatePublicShape();
        }
        else
            orxout(internal_warning) << "Cannot detach non child collision shape" << endl;
    }

    /**
    @brief
        Detach all attached CollisionShapes.
    */
    void CompoundCollisionShape::detachAll()
    {
        while (this->attachedShapes_.size() > 0)
            this->detach(this->attachedShapes_.begin()->first);
    }

    /**
    @brief
        Update the input CollisionShape that is attached to the CompoundCollisionShape.
        This is called when the input shape's internal collision shape (a btCollisionShape) has changed.
    @param shape
        A pointer to the CollisionShape to be updated.
    */
    void CompoundCollisionShape::updateAttachedShape(CollisionShape* shape)
    {
        if (!shape)
            return;

        std::map<CollisionShape*, btCollisionShape*>::iterator it = this->attachedShapes_.find(shape);
        // Check whether the input shape belongs to this CompoundCollisionShape.
        if (it == this->attachedShapes_.end())
        {
            orxout(internal_warning) << "Cannot update child shape: Instance not a child." << endl;
            return;
        }

        // Remove old btCollisionShape, stored in the children map
        if (it->second)
            this->compoundShape_->removeChildShape(it->second); // TODO: Apparently this is broken?

        // Only actually attach if we didn't pick a CompoundCollisionShape with no content
        if (shape->getCollisionShape())
        {
            btTransform transf(multi_cast<btQuaternion>(shape->getOrientation()), multi_cast<btVector3>(shape->getPosition()));
            this->compoundShape_->addChildShape(transf, shape->getCollisionShape());
            it->second = shape->getCollisionShape();
        }

        this->updatePublicShape();
    }

    /**
    @brief
        Updates the public shape, the collision shape this CompoundCollisionShape has to the outside.
    */
    void CompoundCollisionShape::updatePublicShape()
    {
        btCollisionShape* primitive = 0; // The primitive shape, if there is one.
        bool bPrimitive = true; // Whether the CompoundCollisionShape has just one non-empty CollisionShape. And that shape also has no transformation.
        bool bEmpty = true; // Whether the CompoundCollisionShape is empty.
        // Iterate over all CollisionShapes that belong to this CompoundCollisionShape.
        for (std::map<CollisionShape*, btCollisionShape*>::const_iterator it = this->attachedShapes_.begin(); it != this->attachedShapes_.end(); ++it)
        {
            // TODO: Make sure this is correct.
            if (it->second)
            {
                bEmpty = false;
                if (!it->first->hasTransform() && bPrimitive)
                    primitive = it->second;
                else
                {
                    bPrimitive = false;
                    break;
                }
            }
        }

        // If there is no non-empty CollisionShape.
        if (bEmpty)
        {
            // If there was none all along, nothing needs to be changed.
            if (this->collisionShape_ == 0)
                return;
            this->collisionShape_ = 0;
        }
        // If the CompoundCollisionShape is just a primitive.
        // Only one shape to be added, no transform; return it directly.
        else if (bPrimitive)
            this->collisionShape_ = primitive;
        // Make sure we use the compound shape when returning a btCollisionShape.
        else
            this->collisionShape_ = this->compoundShape_;

        this->updateParent();
    }

    /**
    @brief
        Get the attached CollisionShape at the given index.
    @param index
        The index of the desired CollisionShape.
    @return
        Returns a pointer to the attached CollisionShape at the given index.
    */
    CollisionShape* CompoundCollisionShape::getAttachedShape(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::map<CollisionShape*, btCollisionShape*>::const_iterator it = this->attachedShapes_.begin(); it != this->attachedShapes_.end(); ++it)
        {
            if (i == index)
                return it->first;
            ++i;
        }
        return 0;
    }

    /**
    @brief
        Is called when the scale of the CompoundCollisionShape has changed.
        Iterates over all attached CollisionShapes and scales them, then recomputes their compound shape.
    */
    void CompoundCollisionShape::changedScale()
    {
        CollisionShape::changedScale();

        std::vector<CollisionShape*> shapes;
        // Iterate through all attached CollisionShapes and add them to the list of shapes.
        for(std::map<CollisionShape*, btCollisionShape*>::iterator it = this->attachedShapes_.begin(); it != this->attachedShapes_.end(); it++)
            shapes.push_back(it->first);

        // Delete the compound shape and create a new one.
        delete this->compoundShape_;
        this->compoundShape_ = new btCompoundShape();

        // Re-attach all CollisionShapes.
        for(std::vector<CollisionShape*>::iterator it = shapes.begin(); it != shapes.end(); it++)
        {
            CollisionShape* shape = *it;
            shape->setScale3D(this->getScale3D());
            // Only actually attach if we didn't pick a CompoundCollisionShape with no content.
            if (shape->getCollisionShape())
            {
                btTransform transf(multi_cast<btQuaternion>(shape->getOrientation()), multi_cast<btVector3>(shape->getPosition()));
                // Add the btCollisionShape of the CollisionShape as a child shape to the btCompoundShape of the CompoundCollisionShape.
                this->compoundShape_->addChildShape(transf, shape->getCollisionShape());
            }
        }

        this->updatePublicShape();

        /*
        // Iterate through all attached CollisionShapes
        for(std::map<CollisionShape*, btCollisionShape*>::const_iterator it = this->attachedShapes_.begin(); it != this->attachedShapes_.end(); it++)
        {
            // Rescale the CollisionShape.
            it->first->setScale3D(this->getScale3D());
            this->updateAttachedShape(it->first);
        }

        this->updatePublicShape();*/
    }

    int CompoundCollisionShape::getNumChildShapes()
    {
        return this->compoundShape_->getNumChildShapes();
    }
}
