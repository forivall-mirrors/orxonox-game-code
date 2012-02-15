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
 *      Reto Grieder
 *
 */

#ifndef _ControllableEntity_H__
#define _ControllableEntity_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include <string>
#include "util/Math.h"
#include "MobileEntity.h"

namespace orxonox
{
    class _OrxonoxExport ControllableEntity : public MobileEntity
    {
        friend class PlayerInfo; // PlayerInfo uses setPlayer and removePlayer

        public:
            ControllableEntity(BaseObject* creator);
            virtual ~ControllableEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            void setConfigValues();

            virtual void changedPlayer() {}

            inline PlayerInfo* getPlayer() const
                { return this->player_; }
            /**
            @brief Get the player even after the ControllableEntity has stopped being the players ControllableEntity.
            @return Returns the most recent PlayerInfo.
            */
            inline PlayerInfo* getFormerPlayer() const
                { return this->formerPlayer_; }

            inline void setDestroyWhenPlayerLeft(bool bDestroy)
                { this->bDestroyWhenPlayerLeft_ = bDestroy; }
            inline bool getDestroyWhenPlayerLeft() const
                { return this->bDestroyWhenPlayerLeft_; }

            virtual void moveFrontBack(const Vector2& value) {}
            virtual void moveRightLeft(const Vector2& value) {}
            virtual void moveUpDown(const Vector2& value) {}

            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);

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

            void fire(unsigned int firemode);
            virtual void fired(unsigned int firemode) {}
            virtual void reload() {}

            /**
            @brief Tells the ControllableEntity to either start or stop boosting.
                   This doesn't mean, that the ControllableEntity will do so, there might be additional restrictions on boosting, but if it can, then it will.
            @param bBoost If true the ControllableEntity is told to start boosting, if false it is told to stop.
            */
            virtual void boost(bool bBoost) {}

            virtual void greet() {}
            virtual void switchCamera();
            virtual void mouseLook();

            inline const std::string& getHudTemplate() const
                { return this->hudtemplate_; }

            inline Camera* getCamera() const
                { return this->camera_; }
            inline OverlayGroup* getHUD() const
                { return this->hud_; }

            void addCameraPosition(CameraPosition* position);
            CameraPosition* getCameraPosition(unsigned int index) const;
            inline const std::list<SmartPtr<CameraPosition> >& getCameraPositions() const
                { return this->cameraPositions_; }
            unsigned int getCurrentCameraIndex() const;
            bool setCameraPosition(unsigned int index);

            inline void setCameraPositionTemplate(const std::string& name)
                { this->cameraPositionTemplate_ = name; }
            inline const std::string& getCameraPositionTemplate() const
                { return this->cameraPositionTemplate_; }

            inline void setReverseCamera(CameraPosition* camera)
                { this->reverseCamera_ = camera; }
            inline CameraPosition* getReverseCamera() const
                { return this->reverseCamera_; }

            using WorldEntity::setPosition;
            using WorldEntity::setOrientation;
            using MobileEntity::setVelocity;
            using MobileEntity::setAngularVelocity;

            void setPosition(const Vector3& position);
            void setOrientation(const Quaternion& orientation);
            void setVelocity(const Vector3& velocity);
            void setAngularVelocity(const Vector3& velocity);

            inline bool hasLocalController() const
                { return this->bHasLocalController_; }
            inline bool hasHumanController() const
                { return this->bHasHumanController_; }

            inline bool isInMouseLook() const
                { return this->bMouseLook_; }
            inline float getMouseLookSpeed() const
                { return this->mouseLookSpeed_; }
            inline CameraPosition* getCurrentCameraPosition()
                { return this->currentCameraPosition_; }

            inline Controller* getXMLController() const
                { return this->xmlcontroller_; }

            inline Controller* getController() const
                { return this->controller_.get(); }
            void setController(Controller* val);


            virtual void setTarget( WorldEntity* target );
            virtual WorldEntity* getTarget()
                { return this->target_.get(); }
            void setTargetInternal( uint32_t targetID );

	    inline void setTeam(int team)
	        { this->team_ = team; }
	    inline float getTeam() const
	        { return this->team_; }

        protected:
            virtual void preDestroy();

            virtual void setPlayer(PlayerInfo* player); // don't call this directly, use friend class PlayerInfo instead
            virtual void removePlayer();                // don't call this directly, use friend class PlayerInfo instead

            virtual void startLocalHumanControl();
            virtual void stopLocalHumanControl();
            virtual void parentChanged();

            inline void setHudTemplate(const std::string& name)
                { this->hudtemplate_ = name; }
            // HACK-ish
            void createHud(void);
            void destroyHud(void);

            Ogre::SceneNode* cameraPositionRootNode_;

        private:
            void registerVariables();
            void setXMLController(Controller* controller);

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

            void networkcallback_changedplayerID();

            // Bullet btMotionState related
            void setWorldTransform(const btTransform& worldTrans);

            unsigned int server_overwrite_;
            unsigned int client_overwrite_;

            bool bHasLocalController_;
            bool bHasHumanController_;
            bool bDestroyWhenPlayerLeft_;

            Vector3 server_position_;
            Vector3 client_position_;
            Vector3 server_linear_velocity_;
            Vector3 client_linear_velocity_;
            Quaternion server_orientation_;
            Quaternion client_orientation_;
            Vector3 server_angular_velocity_;
            Vector3 client_angular_velocity_;

            PlayerInfo* player_;
            PlayerInfo* formerPlayer_;
            unsigned int playerID_;

            std::string hudtemplate_;
            OverlayGroup* hud_;

            Camera* camera_;
            bool bMouseLook_;
            float mouseLookSpeed_;
            std::list<SmartPtr<CameraPosition> > cameraPositions_;
            CameraPosition* currentCameraPosition_;
            std::string cameraPositionTemplate_;
            Controller* xmlcontroller_;
            WeakPtr<Controller> controller_;
            CameraPosition* reverseCamera_;
            WeakPtr<WorldEntity> target_;

	    int team_ ; //<! teamnumber
    };
}

#endif /* _ControllableEntity_H__ */
