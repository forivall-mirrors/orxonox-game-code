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

#include "OrxonoxStableHeaders.h"
#include "StaticEntity.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(StaticEntity);

    StaticEntity::StaticEntity(BaseObject* creator) : WorldEntity(creator)
    {
        RegisterObject(StaticEntity);

        this->registerVariables();
    }

    StaticEntity::~StaticEntity()
    {
    }

    void StaticEntity::registerVariables()
    {
        REGISTERDATA(this->getPosition().x, network::direction::toclient);
        REGISTERDATA(this->getPosition().y, network::direction::toclient);
        REGISTERDATA(this->getPosition().z, network::direction::toclient);

        REGISTERDATA(this->getOrientation().w, network::direction::toclient);
        REGISTERDATA(this->getOrientation().x, network::direction::toclient);
        REGISTERDATA(this->getOrientation().y, network::direction::toclient);
        REGISTERDATA(this->getOrientation().z, network::direction::toclient);
    }

    bool StaticEntity::isCollisionTypeLegal(WorldEntity::CollisionType type)
    {
        if (type == WorldEntity::Static)
        {
            ThrowException(PhysicsViolation, "Cannot tell a MovableEntity to have static collision type");
            return false;
        }
        else
            return true;
    }

    void StaticEntity::setWorldTransform(const btTransform& worldTrans)
    {
        OrxAssert(false, "Setting world transform of a StaticEntity, which is static!");
        //COUT(0) << "Setting world transform of a StaticEntity, which is static!" << std::endl;
    }

    void StaticEntity::getWorldTransform(btTransform& worldTrans) const
    {
        worldTrans.setOrigin(btVector3(node_->getPosition().x, node_->getPosition().y, node_->getPosition().z));
        worldTrans.setRotation(btQuaternion(node_->getOrientation().w, node_->getOrientation().x, node_->getOrientation().y, node_->getOrientation().z));
    }
}
