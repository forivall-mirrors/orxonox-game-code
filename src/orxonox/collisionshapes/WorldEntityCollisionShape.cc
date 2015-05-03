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

#include "WorldEntityCollisionShape.h"

#include <BulletCollision/CollisionShapes/btCompoundShape.h>

#include "util/OrxAssert.h"
#include "core/CoreIncludes.h"
#include "worldentities/WorldEntity.h"

namespace orxonox
{
    RegisterClass(WorldEntityCollisionShape);

    WorldEntityCollisionShape::WorldEntityCollisionShape(Context* context) : CompoundCollisionShape(context)
    {
        RegisterObject(WorldEntityCollisionShape);

        this->worldEntityOwner_ = NULL;
        // suppress synchronisation
        this->setSyncMode(ObjectDirection::None);
    }

    void WorldEntityCollisionShape::updateParent()
    {
        // Note: This shape may also be attached to a CompoundCollisionShape because it's hosting
        // WorldEntity has been attached to another one.
        CollisionShape::updateParent();

        assert(this->worldEntityOwner_ != 0);
        this->worldEntityOwner_->notifyCollisionShapeChanged();
    }

    void WorldEntityCollisionShape::parentChanged()
    {
        // This object does not get synchronised. This method should never be called then!
        OrxAssert(false, "parentChanged has been called on a non synchronised WorldEntityCollisionShape!");
    }
}
