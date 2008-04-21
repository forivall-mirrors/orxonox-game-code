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
#include "Projectile.h"

#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "core/ConfigValueIncludes.h"

#include "SpaceShip.h"
#include "Explosion.h"
#include "Model.h"

namespace orxonox
{
    CreateFactory(Projectile);

    Projectile::Projectile(SpaceShip* owner)
    {
        RegisterObject(Projectile);

        this->setConfigValues();

        this->owner_ = owner;

        this->billboard_.setBillboardSet("Examples/Flare", ColourValue(1.0, 1.0, 0.5), 1);
        this->attachObject(this->billboard_.getBillboardSet());
        this->scale(0.5);

        if (this->owner_)
        {
            this->setStatic(false);
            this->setOrientation(this->owner_->getOrientation());
            this->setPosition(this->owner_->getPosition());
            this->translate(Vector3(55, 0, 0), Ogre::Node::TS_LOCAL);
            this->setVelocity(Vector3(1, 0, 0) * this->speed_);
        }

        this->destroyTimer_.setTimer(this->lifetime_, false, this, createExecutor(createFunctor(&Projectile::destroyObject)));
    }

    Projectile::~Projectile()
    {
    }

    void Projectile::setConfigValues()
    {
        SetConfigValue(lifetime_, 10.0).description("The time in seconds a projectile stays alive");
        SetConfigValue(speed_, 2000.0).description("The speed of a projectile in units per second");

        this->setVelocity(Vector3(1, 0, 0) * this->speed_);
    }

    void Projectile::tick(float dt)
    {
        WorldEntity::tick(dt);

        float radius;
        for (Iterator<Model> it = ObjectList<Model>::start(); it; ++it)
        {
            if ((*it) != this->owner_)
            {
                radius = it->getScale().x * 3.0;

                if (this->getPosition().squaredDistance(it->getPosition()) <= (radius*radius))
                {
                    new Explosion(this);
                    delete this;
                    return;
                }
            }
        }
    }

    void Projectile::destroyObject()
    {
        delete this;
    }
}
