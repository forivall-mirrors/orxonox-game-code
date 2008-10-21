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

#ifndef _ControllableEntity_H__
#define _ControllableEntity_H__

#include "OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "objects/Tickable.h"

namespace orxonox
{
    class _OrxonoxExport ControllableEntity : public WorldEntity, public Tickable
    {
        public:
            ControllableEntity();
            virtual ~ControllableEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            void registerVariables();

            virtual void setPlayer(PlayerInfo* player);
            virtual void removePlayer();
            inline PlayerInfo* getPlayer() const
                { return this->player_; }

            virtual void startLocalControl();
            virtual void stopLocalControl();

            virtual void moveFrontBack(float value) {}
            virtual void moveRightLeft(float value) {}
            virtual void moveUpDown(float value) {}

            virtual void rotateYaw(float value) {}
            virtual void rotatePitch(float value) {}
            virtual void rotateRoll(float value) {}

            virtual void fire() {}
            virtual void altFire() {}

            virtual void greet() {}
            virtual void use() {}
            virtual void switchCamera() {}

        protected:
            using WorldEntity::setPosition;
            using WorldEntity::translate;
            using WorldEntity::setOrientation;
            using WorldEntity::rotate;
            using WorldEntity::lookAt;
            using WorldEntity::setDirection;

            void setPosition(const Vector3& position);
            void translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
            void setOrientation(const Quaternion& orientation);
            void rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
            void yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
            void pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
            void roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
            void lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z);
            void setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z);

            void setVelocity(const Vector3& velocity);
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

        private:
            inline void setHudTemplate(const std::string& name)
                { this->hudtemplate_ = name; }
            inline const std::string& getHudTemplate() const
                { return this->hudtemplate_; }

            void overwrite();
            void processOverwrite();

            void processServerPosition();
            void processServerVelocity();
            void processServerOrientation();

            void processClientPosition();
            void processClientVelocity();
            void processClientOrientation();

            void updatePlayer();

            unsigned int server_overwrite_;
            unsigned int client_overwrite_;

            Vector3 velocity_;
            Vector3 acceleration_;

            bool bControlled_;
            Vector3 server_position_;
            Vector3 client_position_;
            Vector3 server_velocity_;
            Vector3 client_velocity_;
            Quaternion server_orientation_;
            Quaternion client_orientation_;

            PlayerInfo* player_;
            unsigned int playerID_;
            std::string hudtemplate_;
            OverlayGroup* hud_;
            Camera* camera_;
    };
}

#endif /* _ControllableEntity_H__ */
