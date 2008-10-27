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
#include "tools/BillboardSet.h"

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

        this->setDestroyWhenPlayerLeft(true);

        // test test test
        if (this->getScene()->getSceneManager())
        {
            this->testmesh_ = new Mesh();
            this->testnode_ = this->getNode()->createChildSceneNode();
            this->testmesh_->setMeshSource(this->getScene()->getSceneManager(), "assff.mesh");
            if (this->testmesh_->getEntity())
                this->testnode_->attachObject(this->testmesh_->getEntity());
            this->testnode_->pitch(Degree(-90));
            this->testnode_->roll(Degree(+90));
            this->testnode_->scale(10, 10, 10);
        }
        else
        {
            this->testmesh_ = 0;
            this->testnode_ = 0;
        }
        // test test test

        this->greetingFlare_ = new BillboardSet();
        this->greetingFlare_->setBillboardSet(this->getScene()->getSceneManager(), "Examples/Flare", ColourValue(1.0, 1.0, 0.8), Vector3(0, 20, 0), 1);
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
                this->getNode()->detachObject(this->greetingFlare_->getBillboardSet());
                delete this->greetingFlare_;
            }

            // test test test
            {
                if (this->testmesh_ && this->testnode_)
                    this->testnode_->detachObject(this->testmesh_->getEntity());

                if (this->testmesh_)
                    delete this->testmesh_;

                if (this->testnode_)
                    this->getNode()->removeAndDestroyChild(this->testnode_->getName());
            }
            // test test test
        }
    }

    void Spectator::registerVariables()
    {
        REGISTERDATA(this->bGreetingFlareVisible_, network::direction::toclient, new network::NetworkCallback<Spectator>(this, &Spectator::changedFlareVisibility));
        REGISTERDATA(this->bGreeting_,             network::direction::toserver, new network::NetworkCallback<Spectator>(this, &Spectator::changedGreeting));
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

//        this->setObjectMode(network::direction::toclient);
    }

    void Spectator::startLocalControl()
    {
        ControllableEntity::startLocalControl();
        if (this->isLocallyControlled())
            this->testmesh_->setVisible(false);
    }

    void Spectator::moveFrontBack(float value)
    {
        this->setVelocity(this->getVelocity() + value * this->speed_ * WorldEntity::FRONT);
    }

    void Spectator::moveRightLeft(float value)
    {
        this->setVelocity(this->getVelocity() + value * this->speed_ * WorldEntity::RIGHT);
    }

    void Spectator::moveUpDown(float value)
    {
        this->setVelocity(this->getVelocity() + value * this->speed_ * WorldEntity::UP);
    }

    void Spectator::rotateYaw(float value)
    {
        this->yaw_ = value;
    }

    void Spectator::rotatePitch(float value)
    {
        this->pitch_ = value;
    }

    void Spectator::rotateRoll(float value)
    {
        this->roll_ = value;
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
}
