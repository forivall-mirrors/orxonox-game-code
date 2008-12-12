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
#include "ExplosionChunk.h"

#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "objects/Scene.h"
#include "tools/ParticleInterface.h"
#include "util/Exception.h"

namespace orxonox
{
    CreateFactory(ExplosionChunk);

    ExplosionChunk::ExplosionChunk(BaseObject* creator) : MovableEntity(creator)
    {
        RegisterObject(ExplosionChunk);

        if (!this->getScene() || !this->getScene()->getSceneManager())
            ThrowException(AbortLoading, "Can't create ExplosionChunk, no scene or no scene manager given.");

        this->LOD_ = LODParticle::normal;

        try
        {
            this->fire_ = new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/fire4", this->LOD_);
            this->fire_->addToSceneNode(this->getNode());
            this->smoke_ = new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/smoke7", this->LOD_);
            this->smoke_->addToSceneNode(this->getNode());
        }
        catch (...)
        {
            COUT(1) << "Error: Couln't load particle effect in ExplosionChunk." << std::endl;
            this->fire_ = 0;
            this->smoke_ = 0;
        }

        Vector3 velocity(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1));
        velocity.normalise();
        velocity *= rnd(40, 60);
        this->setVelocity(velocity);

        this->destroyTimer_.setTimer(rnd(1, 2), false, this, createExecutor(createFunctor(&ExplosionChunk::stop)));

        this->registerVariables();
    }

    ExplosionChunk::~ExplosionChunk()
    {
        if (this->isInitialized())
        {
            if (this->fire_)
                delete this->fire_;
            if (this->smoke_)
                delete this->smoke_;
        }
    }

    void ExplosionChunk::registerVariables()
    {
        REGISTERDATA(this->LOD_, direction::toclient, new NetworkCallback<ExplosionChunk>(this, &ExplosionChunk::LODchanged));
    }

    void ExplosionChunk::LODchanged()
    {
        if (this->fire_)
            this->fire_->setDetailLevel(this->LOD_);
        if (this->smoke_)
            this->smoke_->setDetailLevel(this->LOD_);
    }

    void ExplosionChunk::stop()
    {
        if (this->fire_)
            this->fire_->setEnabled(false);
        if (this->smoke_)
            this->smoke_->setEnabled(false);

        this->destroyTimer_.setTimer(1.0f, false, this, createExecutor(createFunctor(&ExplosionChunk::destroy)));
    }

    void ExplosionChunk::destroy()
    {
        delete this;
    }

    void ExplosionChunk::tick(float dt)
    {
        static const unsigned int CHANGES_PER_SECOND = 5;

        if (rnd() < dt*CHANGES_PER_SECOND)
        {
            float length = this->getVelocity().length();

            Vector3 change(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1));
            change.normalise();
            change *= rnd(0.4, 0.8);
            Vector3 velocity = this->getVelocity();
            velocity.normalise();
            velocity += change;
            velocity.normalise();
            velocity *= length * rnd(0.8, 1.0);

            this->setVelocity(velocity);
        }

        SUPER(ExplosionChunk, tick, dt);
    }
}
