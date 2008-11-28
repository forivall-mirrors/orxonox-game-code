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

#ifndef _LinearEntity_H__
#define _LinearEntity_H__

#include "OrxonoxPrereqs.h"

#include "MovableEntity.h"
#include "objects/Tickable.h"
#include "network/ClientConnectionListener.h"

namespace orxonox
{
    class _OrxonoxExport LinearEntity : public MovableEntity, public network::ClientConnectionListener, public Tickable
    {
        public:
            LinearEntity(BaseObject* creator);
            virtual ~LinearEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            void registerVariables();

            inline void setVelocity(const Vector3& velocity);
            inline void setVelocity(float x, float y, float z)
                { this->velocity_.x = x; this->velocity_.y = y; this->velocity_.z = z; }
            inline const Vector3& getVelocity() const
                { return this->velocity_; }

            inline void setAcceleration(const Vector3& acceleration)
                { this->acceleration_ = acceleration; }
            inline void setAcceleration(float x, float y, float z)
                { this->acceleration_.x = x; this->acceleration_.y = y; this->acceleration_.z = z; }
            inline const Vector3& getAcceleration() const
                { return this->acceleration_; }

            inline void setRotationAxis(const Vector3& axis)
                { this->rotationAxis_ = axis; this->rotationAxis_.normalise(); }
            inline void setRotationAxis(float x, float y, float z)
                { this->rotationAxis_.x = x; this->rotationAxis_.y = y; this->rotationAxis_.z = z; rotationAxis_.normalise(); }
            inline const Vector3& getRotationAxis() const
                { return this->rotationAxis_; }

            inline void setRotationRate(const Degree& angle)
                { this->rotationRate_ = angle; }
            inline void setRotationRate(const Radian& angle)
                { this->rotationRate_ = angle; }
            inline const Degree& getRotationRate() const
                { return this->rotationRate_; }

            inline void setMomentum(const Degree& angle)
                { this->momentum_ = angle; }
            inline void setMomentum(const Radian& angle)
                { this->momentum_ = angle; }
            inline const Degree& getMomentum() const
                { return this->momentum_; }

        private:
            void clientConnected(unsigned int clientID);
            void clientDisconnected(unsigned int clientID);
            void resynchronize();

            void overwritePosition();
            void overwriteOrientation();

            void positionChanged();
            void orientationChanged();
            inline void internalSetVelocity(const Vector3& velocity)
                { this->velocity_ = velocity; }

            Vector3 velocity_;
            Vector3 acceleration_;
            Vector3 rotationAxis_;
            Degree rotationRate_;
            Degree momentum_;

            Vector3 overwrite_position_;
            Quaternion overwrite_orientation_;
    };
}

#endif /* _LinearEntity_H__ */
