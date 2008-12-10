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

#include "MovableEntity.h"

namespace orxonox
{
    class _OrxonoxExport ControllableEntity : public MovableEntity
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

            virtual void fire() {}
            virtual void altFire() {}

            virtual void greet() {}
            virtual void use() {}
            virtual void switchCamera();

            inline const std::string& getHudTemplate() const
                { return this->hudtemplate_; }

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

        protected:
            virtual void startLocalControl();
            virtual void stopLocalControl();

            inline void setHudTemplate(const std::string& name)
                { this->hudtemplate_ = name; }

            inline bool isLocallyControlled() const
                { return this->bControlled_; }

        private:
            void overwrite();
            void processOverwrite();

            void processServerPosition();
            void processServerLinearVelocity();
            void processServerOrientation();
            void processServerAngularVelocity();

            void processClientPosition();
            void processClientLinearVelocity();
            void processClientOrientation();
            void processClientAngularVelocity();

            void positionChanged       (bool bContinuous);
            void orientationChanged    (bool bContinuous);
            void linearVelocityChanged (bool bContinuous);
            void angularVelocityChanged(bool bContinuous);

            void networkcallback_changedplayerID();

            unsigned int server_overwrite_;
            unsigned int client_overwrite_;

            bool bControlled_;
            Vector3 server_position_;
            Vector3 client_position_;
            Vector3 server_linear_velocity_;
            Vector3 client_linear_velocity_;
            Quaternion server_orientation_;
            Quaternion client_orientation_;
            Vector3 server_angular_velocity_;
            Vector3 client_angular_velocity_;

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
