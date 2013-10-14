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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _MovableEntity_H__
#define _MovableEntity_H__

#include "OrxonoxPrereqs.h"

#include "util/Math.h"
#include "network/ClientConnectionListener.h"
#include "tools/Timer.h"
#include "MobileEntity.h"

namespace orxonox
{
    class _OrxonoxExport MovableEntity : public MobileEntity, public ClientConnectionListener
    {
        public:
            MovableEntity(Context* context);
            virtual ~MovableEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint);

            using WorldEntity::setPosition;
            using WorldEntity::setOrientation;

            inline void setPosition(const Vector3& position)
                { MobileEntity::setPosition(position); this->overwrite_position_ = this->getPosition(); }
            inline void setOrientation(const Quaternion& orientation)
                { MobileEntity::setOrientation(orientation); this->overwrite_orientation_ = this->getOrientation(); }

            inline void setOwner(Pawn* owner)
                { this->owner_ = owner; }
            inline Pawn* getOwner() const
                { return this->owner_; }

            inline void setCollisionDamage(float c)
                { this->collisionDamage_ = c; }

            inline float getCollisionDamage()
                { return this->collisionDamage_; }

            inline void setEnableCollisionDamage(bool c)
            {
                this->enableCollisionDamage_ = c;
                this->enableCollisionCallback();
            }

            inline bool getEnableCollisionDamage()
                { return this->enableCollisionDamage_; }

        private:
            void registerVariables();
            void clientConnected(unsigned int clientID);
            void clientDisconnected(unsigned int clientID);
            void resynchronize();

            inline void processLinearVelocity()
                { this->setVelocity(this->linearVelocity_); }
            inline void processAngularVelocity()
                { this->setAngularVelocity(this->angularVelocity_); }

            inline void overwritePosition()
                { this->setPosition(this->overwrite_position_); }
            inline void overwriteOrientation()
                { this->setOrientation(this->overwrite_orientation_); }

            Vector3    overwrite_position_;
            Quaternion overwrite_orientation_;

            Timer resynchronizeTimer_;
            Timer* continuousResynchroTimer_;

            Pawn* owner_;
            float collisionDamage_;
            bool enableCollisionDamage_;
    };
}

#endif /* _MovableEntity_H__ */
