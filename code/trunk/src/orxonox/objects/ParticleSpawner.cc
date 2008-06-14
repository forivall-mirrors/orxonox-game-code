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
#include "ParticleSpawner.h"

#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "tools/ParticleInterface.h"

namespace orxonox
{
    CreateFactory(ParticleSpawner);

    ParticleSpawner::ParticleSpawner()
    {
        RegisterObject(ParticleSpawner);
        this->particle_ = 0;
    }

    ParticleSpawner::ParticleSpawner(const std::string& templateName, LODParticle::LOD detaillevel, float lifetime, float delay, const Vector3& direction)
    {
        RegisterObject(ParticleSpawner);
        this->setParticle(templateName, detaillevel, lifetime, delay, direction);
    }

    void ParticleSpawner::setParticle(const std::string& templateName, LODParticle::LOD detaillevel, float lifetime, float delay, const Vector3& direction)
    {
        ExecutorMember<ParticleSpawner>* executor = createExecutor(createFunctor(&ParticleSpawner::createParticleSpawner));
        executor->setDefaultValues(lifetime);
        this->timer_.setTimer(delay, false, this, executor);
        this->particle_ = new ParticleInterface(templateName, detaillevel);
        this->particle_->addToSceneNode(this->getNode());
        this->particle_->setEnabled(false);
        if (direction != Vector3::ZERO)
        {
            this->particle_->getAllEmitters()->setDirection(direction);
        }
    }

    ParticleSpawner::~ParticleSpawner()
    {
        if (this->isInitialized() && this->particle_)
        {
            this->particle_->detachFromSceneNode();
            delete this->particle_;
        }
    };

    void ParticleSpawner::createParticleSpawner(float lifetime)
    {
        this->particle_->setEnabled(true);
        if (lifetime != 0)
            this->timer_.setTimer(lifetime, false, this, createExecutor(createFunctor(&ParticleSpawner::destroyParticleSpawner)));
    }

    void ParticleSpawner::destroyParticleSpawner()
    {
        delete this;
    }

    void ParticleSpawner::setVisible(bool visible)
    {
        if (this->particle_)
            this->particle_->setEnabled(visible);
    }
}
