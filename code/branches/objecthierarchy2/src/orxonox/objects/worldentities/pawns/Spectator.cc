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

#include "OrxonoxStableHeaders.h"
#include "Spectator.h"

#include "core/CoreIncludes.h"
#include "core/Core.h"
#include "objects/worldentities/Model.h"
#include "objects/Scene.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/gametypes/Gametype.h"
#include "tools/BillboardSet.h"
#include "overlays/OverlayText.h"
#include "overlays/OverlayGroup.h"
#include "util/Convert.h"

namespace orxonox
{
    CreateFactory(Spectator);

    Spectator::Spectator(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(Spectator);

        this->speed_ = 100;
        this->rotationSpeed_ = 3;

        this->yaw_ = 0;
        this->pitch_ = 0;
        this->roll_ = 0;
        this->setHudTemplate("spectatorhud");
        this->hudmode_ = 0;

        this->setDestroyWhenPlayerLeft(true);

        this->greetingFlare_ = new BillboardSet();
        this->greetingFlare_->setBillboardSet(this->getScene()->getSceneManager(), "Examples/Flare", ColourValue(1.0, 1.0, 0.8), Vector3(0, 20, 0), 1);
        if (this->greetingFlare_->getBillboardSet())
            this->getNode()->attachObject(this->greetingFlare_->getBillboardSet());
        this->greetingFlare_->setVisible(false);
        this->bGreetingFlareVisible_ = false;
        this->bGreeting_ = false;

        this->registerVariables();
    }

    Spectator::~Spectator()
    {
        if (this->isInitialized())
        {
            if (this->greetingFlare_)
            {
                if (this->greetingFlare_->getBillboardSet())
                    this->getNode()->detachObject(this->greetingFlare_->getBillboardSet());
                delete this->greetingFlare_;
            }
        }
    }

    void Spectator::registerVariables()
    {
        REGISTERDATA(this->bGreetingFlareVisible_, direction::toclient, new NetworkCallback<Spectator>(this, &Spectator::changedFlareVisibility));
        REGISTERDATA(this->bGreeting_,             direction::toserver, new NetworkCallback<Spectator>(this, &Spectator::changedGreeting));
        REGISTERDATA(this->hudmode_,               direction::toclient);
    }

    void Spectator::changedGreeting()
    {
        this->bGreetingFlareVisible_ = this->bGreeting_;
        this->changedFlareVisibility();
    }

    void Spectator::changedFlareVisibility()
    {
        this->greetingFlare_->setVisible(this->bGreetingFlareVisible_);
    }

    void Spectator::tick(float dt)
    {
        this->updateHUD();

        if (this->isLocallyControlled())
        {
            Vector3 velocity = this->getVelocity();
            velocity.normalise();
            this->setVelocity(velocity * this->speed_);

            this->yaw(Radian(this->yaw_ * this->rotationSpeed_));
            this->pitch(Radian(this->pitch_ * this->rotationSpeed_));
            this->roll(Radian(this->roll_ * this->rotationSpeed_));

            this->yaw_ = this->pitch_ = this->roll_ = 0;
        }

        SUPER(Spectator, tick, dt);

        if (this->isLocallyControlled())
        {
            this->setVelocity(Vector3::ZERO);
        }
    }

    void Spectator::setPlayer(PlayerInfo* player)
    {
        ControllableEntity::setPlayer(player);

//        this->setObjectMode(direction::toclient);
    }

    void Spectator::startLocalControl()
    {
        ControllableEntity::startLocalControl();
//        if (this->isLocallyControlled())
//            this->testmesh_->setVisible(false);
    }

    void Spectator::moveFrontBack(const Vector2& value)
    {
        this->setVelocity(this->getVelocity() + value.y * this->speed_ * WorldEntity::FRONT);
    }

    void Spectator::moveRightLeft(const Vector2& value)
    {
        this->setVelocity(this->getVelocity() + value.y * this->speed_ * WorldEntity::RIGHT);
    }

    void Spectator::moveUpDown(const Vector2& value)
    {
        this->setVelocity(this->getVelocity() + value.y * this->speed_ * WorldEntity::UP);
    }

    void Spectator::rotateYaw(const Vector2& value)
    {
        this->yaw_ = value.y;
    }

    void Spectator::rotatePitch(const Vector2& value)
    {
        this->pitch_ = value.y;
    }

    void Spectator::rotateRoll(const Vector2& value)
    {
        this->roll_ = value.y;
    }

    void Spectator::fire()
    {
        if (this->getPlayer())
            this->getPlayer()->setReadyToSpawn(true);
    }

    void Spectator::greet()
    {
        this->bGreeting_ = !this->bGreeting_;

        if (Core::isMaster())
        {
            this->bGreetingFlareVisible_ = this->bGreeting_;
            this->changedFlareVisibility();
        }
    }

    void Spectator::updateHUD()
    {
        // <hack>
        if (Core::isMaster())
        {
            if (this->getPlayer() && this->getGametype())
            {
                if (!this->getGametype()->hasStarted() && !this->getGametype()->isStartCountdownRunning())
                {
                    if (!this->getPlayer()->isReadyToSpawn())
                        this->hudmode_ = 0;
                    else
                        this->hudmode_ = 1;
                }
                else if (!this->getGametype()->hasEnded())
                {
                    if (this->getGametype()->isStartCountdownRunning())
                        this->hudmode_ = 2 + 10*(int)ceil(this->getGametype()->getStartCountdown());
                    else
                        this->hudmode_ = 3;
                }
                else
                    this->hudmode_ = 4;
            }
            else
                return;
        }

        if (this->getHUD())
        {
            std::string text;
            int hudmode = this->hudmode_ % 10;

            switch (hudmode)
            {
                case 0:
                    text = "Press [Fire] to start the match";
                    break;
                case 1:
                    text = "Waiting for other players";
                    break;
                case 2:
                    text = convertToString((this->hudmode_ - 2) / 10);
                    break;
                case 3:
                    text = "Press [Fire] to respawn";
                    break;
                case 4:
                    text = "Game has ended";
                    break;
                default:;
            }

            std::map<std::string, OrxonoxOverlay*>::const_iterator it = this->getHUD()->getOverlays().begin();
            for (; it != this->getHUD()->getOverlays().end(); ++it)
            {
                if (it->second->isA(Class(OverlayText)) && it->second->getName() == "state")
                {
                    OverlayText* overlay = dynamic_cast<OverlayText*>(it->second);
                    if (overlay)
                        overlay->setCaption(text);
                    break;
                }
            }
        }
        // </hack>
    }
}
