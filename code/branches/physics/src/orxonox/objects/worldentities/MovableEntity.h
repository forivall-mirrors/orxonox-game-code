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

#ifndef _MovableEntity_H__
#define _MovableEntity_H__

#include "OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "objects/Tickable.h"
#include "network/ClientConnectionListener.h"

namespace orxonox
{
    class _OrxonoxExport MovableEntity : public WorldEntity
    {
        public:
            MovableEntity(BaseObject* creator);
            virtual ~MovableEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

        private:

            //void attachPhysicalObject(WorldEntity* object);

            //// Bullet btMotionState related
            //void setWorldTransform(const btTransform& worldTrans)
            //{
            //    this->node_->setPosition(worldTrans.getOrigin().x(), worldTrans.getOrigin().y(), worldTrans.getOrigin().z());
            //    this->node_->setOrientation(worldTrans.getRotation().w(), worldTrans.getRotation().x(), worldTrans.getRotation().y(), worldTrans.getRotation().z());
            //    //this->velocity_.x = this->physicalBody_->
            //}

            //// Bullet btMotionState related
            //void getWorldTransform(btTransform& worldTrans) const
            //{
            //    worldTrans.setOrigin(btVector3(node_->getPosition().x, node_->getPosition().y, node_->getPosition().z));
            //    worldTrans.setRotation(btQuaternion(node_->getOrientation().w, node_->getOrientation().x, node_->getOrientation().y, node_->getOrientation().z));
            //}
    };
}

#endif /* _MovableEntity_H__ */
