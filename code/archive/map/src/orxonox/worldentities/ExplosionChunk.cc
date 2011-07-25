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

#include "ExplosionChunk.h"

#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "core/GameMode.h"
#include "util/Exception.h"
#include "Scene.h"
#include "tools/ParticleInterface.h"

namespace orxonox
{
    CreateFactory(ExplosionChunk);

    ExplosionChunk::ExplosionChunk(BaseObject* creator) : MovableEntity(creator)
    {
        RegisterObject(ExplosionChunk);

        if ( GameMode::showsGraphics() && ( !this->getScene() || !this->getScene()->getSceneManager() ) )
            ThrowException(AbortLoading, "Can't create ExplosionChunk, no scene or no scene manager given.");

        this->bStop_ = false;
        this->LOD_ = LODParticle::Normal;

        if ( GameMode::showsGraphics() )
        {
            try
            {
                this->fire_ = new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/fire4", this->LOD_);
                this->attachOgreObject(this->fire_->getParticleSystem());
                this->smoke_ = new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/smoke7", this->LOD_);
                this->attachOgreObject(this->smoke_->getParticleSystem());
            }
            catch (...)
            {
                COUT(1) << "Error: Couln't load particle effect in ExplosionChunk: " << Exception::handleMessage() << std::endl;
                this->fire_ = 0;
                this->smoke_ = 0;
            }
        }
        else
        {
            this->fire_ = 0;
            this->smoke_ = 0;
        }

        if (GameMode::isMaster())
        {
            Vector3 velocity(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1));
            velocity.normalise();
            velocity *= rnd(60, 80);
            this->setVelocity(velocity);

            this->destroyTimer_.setTimer(rnd(1, 2), false, createExecutor(createFunctor(&ExplosionChunk::stop, this)));
        }

        this->registerVariables();
    }

    ExplosionChunk::~ExplosionChunk()
    {
        if (this->isInitialized())
        {
            if (this->fire_)
            {
                this->detachOgreObject(this->fire_->getParticleSystem());
                this->fire_->destroy();
            }
            if (this->smoke_)
            {
                this->detachOgreObject(this->smoke_->getParticleSystem());
                this->smoke_->destroy();
            }
        }
    }

    void ExplosionChunk::registerVariables()
    {
        registerVariable((int&)(this->LOD_), VariableDirection::ToClient, new NetworkCallback<ExplosionChunk>(this, &ExplosionChunk::LODchanged));
        registerVariable(this->bStop_,       VariableDirection::ToClient, new NetworkCallback<ExplosionChunk>(this, &ExplosionChunk::checkStop));
    }

    void ExplosionChunk::LODchanged()
    {
        if (this->fire_)
            this->fire_->setDetailLevel(this->LOD_);
        if (this->smoke_)
            this->smoke_->setDetailLevel(this->LOD_);
    }

    void ExplosionChunk::checkStop()
    {
        if (this->bStop_)
            this->stop();
    }

    void ExplosionChunk::stop()
    {
        if (this->fire_)
            this->fire_->setEnabled(false);
        if (this->smoke_)
            this->smoke_->setEnabled(false);

        if (GameMode::isMaster())
        {
            this->bStop_ = true;
            this->destroyTimer_.setTimer(1.0f, false, createExecutor(createFunctor(&ExplosionChunk::destroy, this)));
        }
    }

    void ExplosionChunk::tick(float dt)
    {
        static const unsigned int CHANGES_PER_SECOND = 5;

        if (GameMode::isMaster() && rnd() < dt*CHANGES_PER_SECOND)
        {
            float length = this->getVelocity().length();

            Vector3 change(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1));
            change.normalise();
            change *= rnd(0.4f, 0.8f);
            Vector3 velocity = this->getVelocity();
            velocity.normalise();
            velocity += change;
            velocity.normalise();
            velocity *= length * rnd(0.8f, 1.0f);

            this->setVelocity(velocity);
        }

        SUPER(ExplosionChunk, tick, dt);
    }
}
