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
            ControllableEntity(BaseObject* creator);
            virtual ~ControllableEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            void registerVariables();

            virtual void setPlayer(PlayerInfo* player);
            virtual void removePlayer();
            inline PlayerInfo* getPlayer() const
                { return this->player_; }

            inline void setDestroyWhenPlayerLeft(bool bDestroy)
                { this->bDestroyWhenPlayerLeft_ = bDestroy; }
            inline bool getDestroyWhenPlayerLeft() const
                { return this->bDestroyWhenPlayerLeft_; }

            virtual void moveFrontBack(const Vector2& value) {}
            virtual void moveRightLeft(const Vector2& value) {}
            virtual void moveUpDown(const Vector2& value) {}

            virtual void rotateYaw(const Vector2& value) {}
            virtual void rotatePitch(const Vector2& value) {}
            virtual void rotateRoll(const Vector2& value) {}

            inline void moveFrontBack(float value)
                { this->moveFrontBack(Vector2(value, 0)); }
            inline void moveRightLeft(float value)
                { this->moveRightLeft(Vector2(value, 0)); }
            inline void moveUpDown(float value)
                { this->moveUpDown(Vector2(value, 0)); }

            inline void rotateYaw(float value)
                { this->rotateYaw(Vector2(value, 0)); }
            inline void rotatePitch(float value)
                { this->rotatePitch(Vector2(value, 0)); }
            inline void rotateRoll(float value)
                { this->rotateRoll(Vector2(value, 0)); }

            virtual void fire() {}
            virtual void altFire() {}

            virtual void boost() {}
            virtual void greet() {}
            virtual void use() {}
            virtual void switchCamera();

            inline const Vector3& getVelocity() const
                { return this->velocity_; }
            inline const Vector3& getAcceleration() const
                { return this->acceleration_; }
            inline const std::string& getHudTemplate() const
                { return this->hudtemplate_; }

            using WorldEntity::setPosition;
            using WorldEntity::translate;
            using WorldEntity::setOrientation;
            using WorldEntity::rotate;
            using WorldEntity::yaw;
            using WorldEntity::pitch;
            using WorldEntity::roll;
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

            inline void setAcceleration(const Vector3& acceleration)
                { this->acceleration_ = acceleration; }
            inline void setAcceleration(float x, float y, float z)
                { this->acceleration_.x = x; this->acceleration_.y = y; this->acceleration_.z = z; }

            inline Camera* getCamera() const
                { return this->camera_; }
            inline OverlayGroup* getHUD() const
                { return this->hud_; }

            void addCameraPosition(CameraPosition* position);
            CameraPosition* getCameraPosition(unsigned int index) const;
            inline const std::list<CameraPosition*>& getCameraPositions() const
                { return this->cameraPositions_; }

            inline void setCameraPositionTemplate(const std::string& name)
                { this->cameraPositionTemplate_ = name; }
            inline const std::string& getCameraPositionTemkplate() const
                { return this->cameraPositionTemplate_; }

            inline bool hasLocalController() const
                { return this->bHasLocalController_; }
            inline bool hasHumanController() const
                { return this->bHasHumanController_; }

        protected:
            virtual void startLocalHumanControl();
            virtual void stopLocalHumanControl();

            inline void setHudTemplate(const std::string& name)
                { this->hudtemplate_ = name; }

            Vector3 acceleration_;

        private:
            void overwrite();
            void processOverwrite();

            void processServerPosition();
            void processServerVelocity();
            void processServerOrientation();

            void processClientPosition();
            void processClientVelocity();
            void processClientOrientation();

            void networkcallback_changedplayerID();

            unsigned int server_overwrite_;
            unsigned int client_overwrite_;

            Vector3 velocity_;

            bool bHasLocalController_;
            bool bHasHumanController_;

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
            bool bDestroyWhenPlayerLeft_;

            std::list<CameraPosition*> cameraPositions_;
            std::string cameraPositionTemplate_;
    };
}

#endif /* _ControllableEntity_H__ */
