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
#include "Explosion.h"

#include <OgreParticleSystem.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include "core/CoreIncludes.h"
#include "core/Executor.h"

#include "util/Math.h"
#include "GraphicsEngine.h"
#include "particle/ParticleInterface.h"

namespace orxonox
{
    CreateFactory(Explosion);

    Explosion::Explosion(WorldEntity* owner) :
      lifetime_(0.4),
      particle_(0)
    {
        RegisterObject(Explosion);

        if (owner)
        {
            this->destroyTimer_.setTimer(this->lifetime_, false, this, createExecutor(createFunctor(&Explosion::destroyObject)));

            Vector3 position = owner->getNode()->getWorldPosition();

            this->particle_ = new ParticleInterface(GraphicsEngine::getSingleton().getSceneManager(), "explosion" + this->getName(), "Orxonox/treibwerk");
            this->particle_->getParticleSystem()->setParameter("local_space", "true");
            this->particle_->newEmitter();
            this->particle_->setPositionOfEmitter(0, Vector3::ZERO);
            this->particle_->setPositionOfEmitter(1, Vector3::ZERO);
            this->particle_->setColour(ColourValue(1.0, 0.8, 0.2));

            this->setPosition(position);
            this->scale(2);

            this->particle_->addToSceneNode(this->getNode());
        }
    }

    Explosion::~Explosion()
    {
        if (this->particle_)
        {
            this->particle_->detachFromSceneNode();
            delete this->particle_;
        }
    };

    void Explosion::destroyObject()
    {
        delete this;
    }
}
