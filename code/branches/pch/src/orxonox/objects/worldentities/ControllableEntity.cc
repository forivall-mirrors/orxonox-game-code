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

#include "ControllableEntity.h"

#include <OgreSceneManager.h>

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "core/Template.h"

#include "objects/Scene.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/controllers/Controller.h"
#include "objects/worldentities/Camera.h"
#include "objects/worldentities/CameraPosition.h"
#include "overlays/OverlayGroup.h"

namespace orxonox
{
    CreateFactory(ControllableEntity);

    ControllableEntity::ControllableEntity(BaseObject* creator) : MobileEntity(creator)
    {
        RegisterObject(ControllableEntity);

        this->bHasLocalController_ = false;
        this->bHasHumanController_ = false;

        this->server_overwrite_ = 0;
        this->client_overwrite_ = 0;
        this->player_ = 0;
        this->playerID_ = OBJECTID_UNKNOWN;
        this->hud_ = 0;
        this->camera_ = 0;
        this->xmlcontroller_ = 0;
        this->reverseCamera_ = 0;
        this->bDestroyWhenPlayerLeft_ = false;
        this->cameraPositionRootNode_ = this->node_->createChildSceneNode();
        this->bMouseLook_ = false;
        this->mouseLookSpeed_ = 200;

        this->server_position_         = Vector3::ZERO;
        this->client_position_         = Vector3::ZERO;
        this->server_linear_velocity_  = Vector3::ZERO;
        this->client_linear_velocity_  = Vector3::ZERO;
        this->server_orientation_      = Quaternion::IDENTITY;
        this->client_orientation_      = Quaternion::IDENTITY;
        this->server_angular_velocity_ = Vector3::ZERO;
        this->client_angular_velocity_ = Vector3::ZERO;


        this->setConfigValues();
        this->setPriority( priority::very_high );
        this->registerVariables();
    }

    ControllableEntity::~ControllableEntity()
    {
        if (this->isInitialized())
        {
            this->bDestroyWhenPlayerLeft_ = false;

            if (this->bHasLocalController_ && this->bHasHumanController_)
                this->stopLocalHumanControl();

            if (this->getPlayer() && this->getPlayer()->getControllableEntity() == this)
                this->getPlayer()->stopControl();

            if (this->xmlcontroller_)
                delete this->xmlcontroller_;

            if (this->hud_)
                delete this->hud_;

            if (this->camera_)
                delete this->camera_;

            for (std::list<CameraPosition*>::const_iterator it = this->cameraPositions_.begin(); it != this->cameraPositions_.end(); ++it)
                delete (*it);

            if (this->getScene()->getSceneManager())
                this->getScene()->getSceneManager()->destroySceneNode(this->cameraPositionRootNode_->getName());
        }
    }

    void ControllableEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ControllableEntity, XMLPort, xmlelement, mode);

        XMLPortParam(ControllableEntity, "hudtemplate", setHudTemplate, getHudTemplate, xmlelement, mode);
        XMLPortParam(ControllableEntity, "camerapositiontemplate", setCameraPositionTemplate, getCameraPositionTemkplate, xmlelement, mode);

        XMLPortObject(ControllableEntity, CameraPosition, "camerapositions", addCameraPosition, getCameraPosition, xmlelement, mode);
        XMLPortObject(ControllableEntity, Controller,     "controller",      setXMLController,  getXMLController,  xmlelement, mode);
    }

    void ControllableEntity::setConfigValues()
    {
        SetConfigValue(mouseLookSpeed_, 3.0f);
    }

    void ControllableEntity::addCameraPosition(CameraPosition* position)
    {
        if (!position->getIsAbsolute())
        {
            if (position->getAllowMouseLook())
                position->attachToNode(this->cameraPositionRootNode_);
            else
                this->attach(position);
        }
        else
        {
            WorldEntity* parent = this->getParent();
            if (parent)
                parent->attach(position);
        }

        if (!position->getRenderCamera())
            this->cameraPositions_.push_back(position);
        else
            this->setReverseCamera(position);
    }

    CameraPosition* ControllableEntity::getCameraPosition(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<CameraPosition*>::const_iterator it = this->cameraPositions_.begin(); it != this->cameraPositions_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    void ControllableEntity::switchCamera()
    {
        if (this->camera_)
        {
            if (this->camera_->getParent() == this && this->cameraPositions_.size() > 0)
            {
                this->cameraPositions_.front()->attachCamera(this->camera_);
            }
            else if (this->cameraPositions_.size() > 0)
            {
                for (std::list<CameraPosition*>::const_iterator it = this->cameraPositions_.begin(); it != this->cameraPositions_.end(); ++it)
                {
                    if ((*it) == this->camera_->getParent())
                    {
                        ++it;
                        if (it != this->cameraPositions_.end())
                            (*it)->attachCamera(this->camera_);
                        else
                            (*this->cameraPositions_.begin())->attachCamera(this->camera_);
                        break;
                    }
                }
            }
            else
            {
                this->camera_->attachToNode(this->cameraPositionRootNode_);
            }
        }
    }

    void ControllableEntity::mouseLook()
    {
        this->bMouseLook_ = !this->bMouseLook_;

        if (!this->bMouseLook_)
            this->cameraPositionRootNode_->setOrientation(Quaternion::IDENTITY);
    }

    void ControllableEntity::rotateYaw(const Vector2& value)
    {
        if (this->bMouseLook_)
            this->cameraPositionRootNode_->yaw(Radian(value.y * this->mouseLookSpeed_), Ogre::Node::TS_LOCAL);
    }

    void ControllableEntity::rotatePitch(const Vector2& value)
    {
        if (this->bMouseLook_)
            this->cameraPositionRootNode_->pitch(Radian(value.y * this->mouseLookSpeed_), Ogre::Node::TS_LOCAL);
    }

    void ControllableEntity::rotateRoll(const Vector2& value)
    {
        if (this->bMouseLook_)
            this->cameraPositionRootNode_->roll(Radian(value.y * this->mouseLookSpeed_), Ogre::Node::TS_LOCAL);
    }

    void ControllableEntity::setPlayer(PlayerInfo* player)
    {
        if (!player)
        {
            this->removePlayer();
            return;
        }

        this->player_ = player;
        this->playerID_ = player->getObjectID();
        this->bHasLocalController_ = player->isLocalPlayer();
        this->bHasHumanController_ = player->isHumanPlayer();

        if (this->bHasLocalController_ && this->bHasHumanController_)
        {
            this->startLocalHumanControl();

            if (!GameMode::isMaster())
            {
                this->client_overwrite_ = this->server_overwrite_;
                this->setObjectMode(objectDirection::bidirectional);
            }
        }

        this->changedPlayer();
    }

    void ControllableEntity::removePlayer()
    {
        if (this->bHasLocalController_ && this->bHasHumanController_)
            this->stopLocalHumanControl();

        this->player_ = 0;
        this->playerID_ = OBJECTID_UNKNOWN;
        this->bHasLocalController_ = false;
        this->bHasHumanController_ = false;
        this->setObjectMode(objectDirection::toclient);

        this->changedPlayer();

        if (this->bDestroyWhenPlayerLeft_)
            delete this;
    }

    void ControllableEntity::networkcallback_changedplayerID()
    {
        // just do this in case the entity wasn't yet synchronized when the corresponding PlayerInfo got our objectID
        if (this->playerID_ != OBJECTID_UNKNOWN)
        {
            this->player_ = dynamic_cast<PlayerInfo*>(Synchronisable::getSynchronisable(this->playerID_));
            if (this->player_ && (this->player_->getControllableEntity() != this))
                this->player_->startControl(this);
        }
    }

    void ControllableEntity::startLocalHumanControl()
    {
        if (!this->camera_)
        {
            this->camera_ = new Camera(this);
            this->camera_->requestFocus();
            if (this->cameraPositionTemplate_ != "")
                this->addTemplate(this->cameraPositionTemplate_);
            if (this->cameraPositions_.size() > 0)
                this->cameraPositions_.front()->attachCamera(this->camera_);
            else
                this->camera_->attachToNode(this->cameraPositionRootNode_);
        }

        if (!this->hud_)
        {
            if (this->hudtemplate_ != "")
            {
                this->hud_ = new OverlayGroup(this);
                this->hud_->addTemplate(this->hudtemplate_);
                this->hud_->setOwner(this);
            }
        }
    }

    void ControllableEntity::stopLocalHumanControl()
    {
        if (this->camera_)
        {
            this->camera_->detachFromParent();
            delete this->camera_;
            this->camera_ = 0;
        }

        if (this->hud_)
        {
            delete this->hud_;
            this->hud_ = 0;
        }
    }

    void ControllableEntity::setXMLController(Controller* controller)
    {
        if (!this->xmlcontroller_)
        {
            this->xmlcontroller_ = controller;
            this->bHasLocalController_ = true;
            this->xmlcontroller_->setControllableEntity(this);
        }
        else
            COUT(2) << "Warning: ControllableEntity \"" << this->getName() << "\" already has a Controller." << std::endl;
    }

    void ControllableEntity::parentChanged()
    {
        WorldEntity::parentChanged();

        WorldEntity* parent = this->getParent();
        if (parent)
        {
            for (std::list<CameraPosition*>::iterator it = this->cameraPositions_.begin(); it != this->cameraPositions_.end(); ++it)
                if ((*it)->getIsAbsolute())
                    parent->attach((*it));
        }
    }

    void ControllableEntity::tick(float dt)
    {
        MobileEntity::tick(dt);

        if (this->isActive())
        {
            // Check whether Bullet doesn't do the physics for us
            if (!this->isDynamic())
            {
                if (GameMode::isMaster())
                {
                    this->server_position_ = this->getPosition();
                    this->server_orientation_ = this->getOrientation();
                    this->server_linear_velocity_ = this->getVelocity();
                    this->server_angular_velocity_ = this->getAngularVelocity();
                }
                else if (this->bHasLocalController_)
                {
                    this->client_position_ = this->getPosition();
                    this->client_orientation_ = this->getOrientation();
                    this->client_linear_velocity_ = this->getVelocity();
                    this->client_angular_velocity_ = this->getAngularVelocity();
                }
            }
        }
    }

    void ControllableEntity::registerVariables()
    {
        registerVariable(this->cameraPositionTemplate_,  variableDirection::toclient);
        registerVariable(this->hudtemplate_,             variableDirection::toclient);

        registerVariable(this->server_position_,         variableDirection::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processServerPosition));
        registerVariable(this->server_linear_velocity_,  variableDirection::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processServerLinearVelocity));
        registerVariable(this->server_orientation_,      variableDirection::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processServerOrientation));
        registerVariable(this->server_angular_velocity_, variableDirection::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processServerAngularVelocity));

        registerVariable(this->server_overwrite_,        variableDirection::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processOverwrite));
        registerVariable(this->client_overwrite_,        variableDirection::toserver);

        registerVariable(this->client_position_,         variableDirection::toserver, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processClientPosition));
        registerVariable(this->client_linear_velocity_,  variableDirection::toserver, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processClientLinearVelocity));
        registerVariable(this->client_orientation_,      variableDirection::toserver, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processClientOrientation));
        registerVariable(this->client_angular_velocity_, variableDirection::toserver, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processClientAngularVelocity));
        

        registerVariable(this->playerID_,                variableDirection::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::networkcallback_changedplayerID));
    }

    void ControllableEntity::processServerPosition()
    {
        if (!this->bHasLocalController_)
            MobileEntity::setPosition(this->server_position_);
    }

    void ControllableEntity::processServerLinearVelocity()
    {
        if (!this->bHasLocalController_)
            MobileEntity::setVelocity(this->server_linear_velocity_);
    }

    void ControllableEntity::processServerOrientation()
    {
        if (!this->bHasLocalController_)
            MobileEntity::setOrientation(this->server_orientation_);
    }

    void ControllableEntity::processServerAngularVelocity()
    {
        if (!this->bHasLocalController_)
            MobileEntity::setAngularVelocity(this->server_angular_velocity_);
    }

    void ControllableEntity::processOverwrite()
    {
        if (this->bHasLocalController_)
        {
            this->setPosition(this->server_position_);
            this->setOrientation(this->server_orientation_);
            this->setVelocity(this->server_linear_velocity_);
            this->setAngularVelocity(this->server_angular_velocity_);

            this->client_overwrite_ = this->server_overwrite_;
        }
    }

    void ControllableEntity::processClientPosition()
    {
        if (this->server_overwrite_ == this->client_overwrite_)
        {
            MobileEntity::setPosition(this->client_position_);
            this->server_position_ = this->getPosition();
        }
    }

    void ControllableEntity::processClientLinearVelocity()
    {
        if (this->server_overwrite_ == this->client_overwrite_)
        {
            MobileEntity::setVelocity(this->client_linear_velocity_);
            this->server_linear_velocity_ = this->getVelocity();
        }
    }

    void ControllableEntity::processClientOrientation()
    {
        if (this->server_overwrite_ == this->client_overwrite_)
        {
            MobileEntity::setOrientation(this->client_orientation_);
            this->server_orientation_ = this->getOrientation();
        }
    }

    void ControllableEntity::processClientAngularVelocity()
    {
        if (this->server_overwrite_ == this->client_overwrite_)
        {
            MobileEntity::setAngularVelocity(this->client_angular_velocity_);
            this->server_angular_velocity_ = this->getAngularVelocity();
        }
    }

    void ControllableEntity::setPosition(const Vector3& position)
    {
        if (GameMode::isMaster())
        {
            MobileEntity::setPosition(position);
            this->server_position_ = this->getPosition();
            ++this->server_overwrite_;
        }
        else if (this->bHasLocalController_)
        {
            MobileEntity::setPosition(position);
            this->client_position_ = this->getPosition();
        }
    }

    void ControllableEntity::setOrientation(const Quaternion& orientation)
    {
        if (GameMode::isMaster())
        {
            MobileEntity::setOrientation(orientation);
            this->server_orientation_ = this->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bHasLocalController_)
        {
            MobileEntity::setOrientation(orientation);
            this->client_orientation_ = this->getOrientation();
        }
    }

    void ControllableEntity::setVelocity(const Vector3& velocity)
    {
        if (GameMode::isMaster())
        {
            MobileEntity::setVelocity(velocity);
            this->server_linear_velocity_ = this->getVelocity();
            ++this->server_overwrite_;
        }
        else if (this->bHasLocalController_)
        {
            MobileEntity::setVelocity(velocity);
            this->client_linear_velocity_ = this->getVelocity();
        }
    }

    void ControllableEntity::setAngularVelocity(const Vector3& velocity)
    {
        if (GameMode::isMaster())
        {
            MobileEntity::setAngularVelocity(velocity);
            this->server_angular_velocity_ = this->getAngularVelocity();
            ++this->server_overwrite_;
        }
        else if (this->bHasLocalController_)
        {
            MobileEntity::setAngularVelocity(velocity);
            this->client_angular_velocity_ = this->getAngularVelocity();
        }
    }

    void ControllableEntity::setWorldTransform(const btTransform& worldTrans)
    {
        MobileEntity::setWorldTransform(worldTrans);
        if (GameMode::isMaster())
        {
            this->server_position_ = this->getPosition();
            this->server_orientation_ = this->getOrientation();
            this->server_linear_velocity_ = this->getVelocity();
            this->server_angular_velocity_ = this->getAngularVelocity();
        }
        else if (this->bHasLocalController_)
        {
            this->client_position_ = this->getPosition();
            this->client_orientation_ = this->getOrientation();
            this->client_linear_velocity_ = this->getVelocity();
            this->client_angular_velocity_ = this->getAngularVelocity();
        }
    }
}
