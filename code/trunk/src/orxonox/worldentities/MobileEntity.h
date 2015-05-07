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
 *      Martin Stypinski
 *
 */

#ifndef _MobileEntity_H__
#define _MobileEntity_H__

#include "OrxonoxPrereqs.h"

#include "util/Math.h"
#include "tools/interfaces/Tickable.h"
#include "WorldEntity.h"

namespace orxonox
{
    /**
    @brief
        The MobileEntity is a derived class from @ref orxonox::WorldEntity and @ref orxonox::Tickable. It is supposed to be the base class of 
        @ref orxonox::MovableEntity and @ref orxonox::ControllableEntity. You will never need to instanciate a MobileEntity directly.
        Use only its derivatives.
        In addition to the functionalities of the @ref orxonox::WorldEntity this class has a linear and angular velocity and a linear and angular acceleration.
        Every time the @see tick function is called the linear acceleration is multiplied by the time since the last call of tick and then added to the
        linear velocity. Then the linear velocity is multiplied by the time since the last call of tick and then added to the position. The same happens with
        the angular acceleration and velocity. With this procedure MobileEntities can change their position and orientation with time.
    */

    class _OrxonoxExport MobileEntity : public WorldEntity, public Tickable
    {
        public:
            MobileEntity(Context* context);
            virtual ~MobileEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

            virtual void setPosition(const Vector3& position);
            virtual void setOrientation(const Quaternion& orientation);

            virtual void setVelocity(const Vector3& velocity);
            inline void setVelocity(float x, float y, float z)
                { this->setVelocity(Vector3(x, y, z)); }
            inline const Vector3& getVelocity() const
                { return this->linearVelocity_; }
            /**
            @brief Get the velocity in the coordinate-system of the MoblieEntity.
            @return Returns the velocity of the MobileEntity in its coordinate-system.
            */
            inline Vector3 getLocalVelocity() const
                { return (this->getOrientation().Inverse() * this->getVelocity()); }

            virtual void setAngularVelocity(const Vector3& velocity);
            inline void setAngularVelocity(float x, float y, float z)
                { this->setAngularVelocity(Vector3(x, y, z)); }
            inline const Vector3& getAngularVelocity() const
                { return this->angularVelocity_; }

            void setAcceleration(const Vector3& acceleration);
            inline void setAcceleration(float x, float y, float z)
                { this->setAcceleration(Vector3(x, y, z)); }
            inline const Vector3& getAcceleration() const
                { return this->linearAcceleration_; }
            void addAcceleration(const Vector3& acceleration, const Vector3 &relativePosition = Vector3::ZERO); // Adds the input acceleration at the input position to the MobileEntity.

            void setAngularAcceleration(const Vector3& acceleration);
            inline void setAngularAcceleration(float x, float y, float z)
                { this->setAngularAcceleration(Vector3(x, y, z)); }
            inline const Vector3& getAngularAcceleration() const
                { return this->angularAcceleration_; }

            void applyCentralForce(const Vector3& force);
            inline void applyCentralForce(float x, float y, float z)
                { this->applyCentralForce(Vector3(x, y, z)); }

            inline void setRotationRate(Degree rate)
                { this->setAngularVelocity(this->getAngularVelocity().normalisedCopy() * rate.valueRadians()); }
            inline Degree getRotationRate() const
                { return Radian(this->getAngularVelocity().length()); }

            inline void setRotationAxis(const Vector3& axis)
                { this->setAngularVelocity(axis * this->getAngularVelocity().length()); }
            inline Vector3 getRotationAxis() const
                { return this->getAngularVelocity().normalisedCopy(); }

        protected:
            // Bullet btMotionState related
            virtual void setWorldTransform(const btTransform& worldTrans);
            void getWorldTransform(btTransform& worldTrans) const;

            Vector3 linearAcceleration_;
            Vector3 linearVelocity_;
            Vector3 angularAcceleration_;
            Vector3 angularVelocity_;

        private:
            virtual bool isCollisionTypeLegal(WorldEntity::CollisionType type) const;
    };
}

#endif /* _MobileEntity_H__ */
