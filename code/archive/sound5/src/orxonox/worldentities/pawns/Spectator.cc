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

#include "Spectator.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/GameMode.h"

#include "tools/BillboardSet.h"
#include "Scene.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{
    CreateFactory(Spectator);

    Spectator::Spectator(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(Spectator);

        this->speed_ = 200;

        this->yaw_ = 0;
        this->pitch_ = 0;
        this->roll_ = 0;
        this->localVelocity_ = Vector3::ZERO;
        this->setHudTemplate("spectatorhud");
        this->greetingFlare_ = 0;

        this->setDestroyWhenPlayerLeft(true);

        if (GameMode::showsGraphics())
        {
            this->greetingFlare_ = new BillboardSet();
            this->greetingFlare_->setBillboardSet(this->getScene()->getSceneManager(), "Examples/Flare", ColourValue(1.0f, 1.0f, 0.8f), Vector3(0, 20, 0), 1);
            if (this->greetingFlare_->getBillboardSet())
                this->attachOgreObject(this->greetingFlare_->getBillboardSet());
            this->greetingFlare_->setVisible(false);
        }

        this->bGreetingFlareVisible_ = false;
        this->bGreeting_ = false;

        this->setConfigValues();
        this->registerVariables();
    }

    Spectator::~Spectator()
    {
        if (this->isInitialized())
        {
            if (this->greetingFlare_)
            {
                if (this->greetingFlare_->getBillboardSet())
                    this->detachOgreObject(this->greetingFlare_->getBillboardSet());

                delete this->greetingFlare_;
            }
        }
    }

    void Spectator::setConfigValues()
    {
        SetConfigValue(speed_, 200.0f);
    }

    void Spectator::registerVariables()
    {
        registerVariable(this->bGreetingFlareVisible_, VariableDirection::ToClient, new NetworkCallback<Spectator>(this, &Spectator::changedFlareVisibility));
        registerVariable(this->bGreeting_,             VariableDirection::ToServer, new NetworkCallback<Spectator>(this, &Spectator::changedGreeting));
    }

    void Spectator::changedGreeting()
    {
        this->bGreetingFlareVisible_ = this->bGreeting_;
        this->changedFlareVisibility();
    }

    void Spectator::changedFlareVisibility()
    {
        if (this->greetingFlare_)
            this->greetingFlare_->setVisible(this->bGreetingFlareVisible_);
    }

    void Spectator::tick(float dt)
    {
        if (this->hasLocalController())
        {
            float localSpeedSquared = this->localVelocity_.squaredLength();
            float localSpeed;
            if (localSpeedSquared > 1.0)
                localSpeed = this->speed_ / sqrtf(localSpeedSquared);
            else
                localSpeed = this->speed_;

            this->localVelocity_.x *= localSpeed;
            this->localVelocity_.y *= localSpeed;
            this->localVelocity_.z *= localSpeed;
            this->setVelocity(this->getOrientation() * this->localVelocity_);
            this->localVelocity_.x = 0;
            this->localVelocity_.y = 0;
            this->localVelocity_.z = 0;

            if (!this->isInMouseLook())
            {
                this->yaw(Radian(this->yaw_ * this->getMouseLookSpeed()));
                this->pitch(Radian(this->pitch_ * this->getMouseLookSpeed()));
                this->roll(Radian(this->roll_ * this->getMouseLookSpeed()));
            }

            this->yaw_ = this->pitch_ = this->roll_ = 0;
        }

        SUPER(Spectator, tick, dt);
    }

    void Spectator::setPlayer(PlayerInfo* player)
    {
        ControllableEntity::setPlayer(player);

//        this->setSyncMode(ObjectDirection::ToClient);
    }

    void Spectator::startLocalHumanControl()
    {
        ControllableEntity::startLocalHumanControl();
    }

    void Spectator::moveFrontBack(const Vector2& value)
    {
        this->localVelocity_.z -= value.x;
    }

    void Spectator::moveRightLeft(const Vector2& value)
    {
        this->localVelocity_.x += value.x;
    }

    void Spectator::moveUpDown(const Vector2& value)
    {
        this->localVelocity_.y += value.x;
    }

    void Spectator::rotateYaw(const Vector2& value)
    {
        this->yaw_ += value.y;

        ControllableEntity::rotateYaw(value);
    }

    void Spectator::rotatePitch(const Vector2& value)
    {
        this->pitch_ += value.y;

        ControllableEntity::rotatePitch(value);
    }

    void Spectator::rotateRoll(const Vector2& value)
    {
        this->roll_ += value.y;

        ControllableEntity::rotateRoll(value);
    }

    void Spectator::fired(unsigned int firemode)
    {
        if (this->getPlayer())
            this->getPlayer()->setReadyToSpawn(true);
    }

    void Spectator::greet()
    {
        this->bGreeting_ = !this->bGreeting_;

        if (GameMode::isMaster())
        {
            this->bGreetingFlareVisible_ = this->bGreeting_;
            this->changedFlareVisibility();
        }
    }
}
