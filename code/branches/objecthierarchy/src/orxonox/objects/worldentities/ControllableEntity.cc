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

#include "ControllableEntity.h"

#include "core/CoreIncludes.h"
#include "core/Core.h"
#include "core/XMLPort.h"
#include "core/Template.h"

#include "objects/infos/PlayerInfo.h"
#include "objects/Camera.h"
#include "overlays/OverlayGroup.h"

namespace orxonox
{
    CreateFactory(ControllableEntity);

    ControllableEntity::ControllableEntity()
    {
        RegisterObject(ControllableEntity);

        this->bControlled_ = false;
        this->server_overwrite_ = 0;
        this->client_overwrite_ = 0;
        this->player_ = 0;
        this->playerID_ = network::OBJECTID_UNKNOWN;
        this->hud_ = 0;
        this->camera_ = 0;
        this->bDestroyWhenPlayerLeft_ = false;

        this->velocity_ = Vector3::ZERO;
        this->acceleration_ = Vector3::ZERO;

        this->server_position_ = Vector3::ZERO;
        this->client_position_ = Vector3::ZERO;
        this->server_velocity_ = Vector3::ZERO;
        this->client_velocity_ = Vector3::ZERO;
        this->server_orientation_ = Quaternion::IDENTITY;
        this->client_orientation_ = Quaternion::IDENTITY;

        this->registerVariables();
    }

    ControllableEntity::~ControllableEntity()
    {
        if (this->isInitialized() && this->bControlled_)
            this->stopLocalControl();
    }

    void ControllableEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ControllableEntity, XMLPort, xmlelement, mode);

        XMLPortParam(ControllableEntity, "hudtemplate", setHudTemplate, getHudTemplate, xmlelement, mode);
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
        this->bControlled_ = player->isLocalPlayer();

        if (this->bControlled_)
        {
            this->startLocalControl();
            this->setObjectMode(network::direction::bidirectional);
        }
    }

    void ControllableEntity::removePlayer()
    {
        if (this->bControlled_)
            this->stopLocalControl();

        this->player_ = 0;
        this->playerID_ = network::OBJECTID_UNKNOWN;
        this->bControlled_ = false;
        this->setObjectMode(network::direction::toclient);

        if (this->bDestroyWhenPlayerLeft_)
            delete this;
    }

    void ControllableEntity::updatePlayer()
    {
        if (this->playerID_ != network::OBJECTID_UNKNOWN)
        {
            this->player_ = dynamic_cast<PlayerInfo*>(network::Synchronisable::getSynchronisable(this->playerID_));
            if (this->player_ && (this->player_->getPawn() != this))
                this->player_->startControl(this);
        }
    }

    void ControllableEntity::startLocalControl()
    {
        std::cout << "###### start local control" << std::endl;
        this->camera_ = new Camera();
        this->camera_->requestFocus();
        this->attach(this->camera_);

        if (this->hudtemplate_ != "")
        {
            this->hud_ = new OverlayGroup();
            this->hud_->addTemplate(this->hudtemplate_);
        }
    }

    void ControllableEntity::stopLocalControl()
    {
        std::cout << "###### stop local control" << std::endl;
        this->detach(this->camera_);
        delete this->camera_;
        this->camera_ = 0;

        delete this->hud_;
        this->hud_ = 0;
    }

    void ControllableEntity::tick(float dt)
    {
        if (this->isActive())
        {
            this->velocity_ += (dt * this->acceleration_);
            this->node_->translate(dt * this->velocity_, Ogre::Node::TS_PARENT);

            if (Core::isMaster())
            {
                this->server_velocity_ = this->velocity_;
                this->server_position_ = this->node_->getPosition();
            }
            else if (this->bControlled_)
            {
                this->client_velocity_ = this->velocity_;
                this->client_position_ = this->node_->getPosition();
            }
        }
    }

    void ControllableEntity::registerVariables()
    {
        REGISTERDATA(this->server_position_,    network::direction::toclient, new network::NetworkCallback<ControllableEntity>(this, &ControllableEntity::processServerPosition));
        REGISTERDATA(this->server_velocity_,    network::direction::toclient, new network::NetworkCallback<ControllableEntity>(this, &ControllableEntity::processServerVelocity));
        REGISTERDATA(this->server_orientation_, network::direction::toclient, new network::NetworkCallback<ControllableEntity>(this, &ControllableEntity::processServerOrientation));

        REGISTERDATA(this->server_overwrite_,   network::direction::toclient, new network::NetworkCallback<ControllableEntity>(this, &ControllableEntity::processOverwrite));

        REGISTERDATA(this->client_position_,    network::direction::toserver, new network::NetworkCallback<ControllableEntity>(this, &ControllableEntity::processClientPosition));
        REGISTERDATA(this->client_velocity_,    network::direction::toserver, new network::NetworkCallback<ControllableEntity>(this, &ControllableEntity::processClientVelocity));
        REGISTERDATA(this->client_orientation_, network::direction::toserver, new network::NetworkCallback<ControllableEntity>(this, &ControllableEntity::processClientOrientation));

        REGISTERDATA(this->client_overwrite_,   network::direction::toserver);

        REGISTERDATA(this->playerID_, network::direction::toclient, new network::NetworkCallback<ControllableEntity>(this, &ControllableEntity::updatePlayer));
    }

    void ControllableEntity::processServerPosition()
    {
        if (!this->bControlled_)
            this->node_->setPosition(this->server_position_);
    }

    void ControllableEntity::processServerVelocity()
    {
        if (!this->bControlled_)
            this->velocity_ = this->server_velocity_;
    }

    void ControllableEntity::processServerOrientation()
    {
        if (!this->bControlled_)
            this->node_->setOrientation(this->server_orientation_);
    }

    void ControllableEntity::processOverwrite()
    {
        if (this->bControlled_)
        {
            this->setPosition(this->server_position_);
            this->setVelocity(this->server_velocity_);
            this->setOrientation(this->server_orientation_);

            this->client_overwrite_ = this->server_overwrite_;
        }
    }

    void ControllableEntity::processClientPosition()
    {
        if (this->server_overwrite_ == this->client_overwrite_)
        {
            this->node_->setPosition(this->client_position_);
            this->server_position_ = this->client_position_;
        }
    }

    void ControllableEntity::processClientVelocity()
    {
        if (this->server_overwrite_ == this->client_overwrite_)
        {
            this->velocity_ = this->client_velocity_;
            this->server_velocity_ = this->client_velocity_;
        }
    }

    void ControllableEntity::processClientOrientation()
    {
        if (this->server_overwrite_ == this->client_overwrite_)
        {
            this->node_->setOrientation(this->client_orientation_);
            this->server_orientation_ = this->client_orientation_;
        }
    }


    void ControllableEntity::setPosition(const Vector3& position)
    {
        if (Core::isMaster())
        {
            this->node_->setPosition(position);
            this->server_position_ = position;
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->setPosition(position);
            this->client_position_ = position;
        }
    }

    void ControllableEntity::setVelocity(const Vector3& velocity)
    {
        if (Core::isMaster())
        {
            this->velocity_ = velocity;
            this->server_velocity_ = velocity;
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->velocity_ = velocity;
            this->client_velocity_ = velocity;
        }
    }

    void ControllableEntity::translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo)
    {
        if (Core::isMaster())
        {
            this->node_->translate(distance, relativeTo);
            this->server_position_ = this->node_->getPosition();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->translate(distance, relativeTo);
            this->client_position_ = this->node_->getPosition();
        }
    }

    void ControllableEntity::setOrientation(const Quaternion& orientation)
    {
        if (Core::isMaster())
        {
            this->node_->setOrientation(orientation);
            this->server_orientation_ = orientation;
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->setOrientation(orientation);
            this->client_orientation_ = orientation;
        }
    }

    void ControllableEntity::rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo)
    {
        if (Core::isMaster())
        {
            this->node_->rotate(rotation, relativeTo);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->rotate(rotation, relativeTo);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }

    void ControllableEntity::yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        if (Core::isMaster())
        {
            this->node_->yaw(angle, relativeTo);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->yaw(angle, relativeTo);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }

    void ControllableEntity::pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        if (Core::isMaster())
        {
            this->node_->pitch(angle, relativeTo);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->pitch(angle, relativeTo);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }

    void ControllableEntity::roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        if (Core::isMaster())
        {
            this->node_->roll(angle, relativeTo);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->roll(angle, relativeTo);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }

    void ControllableEntity::lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        if (Core::isMaster())
        {
            this->node_->lookAt(target, relativeTo, localDirectionVector);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->lookAt(target, relativeTo, localDirectionVector);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }

    void ControllableEntity::setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        if (Core::isMaster())
        {
            this->node_->setDirection(direction, relativeTo, localDirectionVector);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->setDirection(direction, relativeTo, localDirectionVector);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }
}
