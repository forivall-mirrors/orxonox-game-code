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

/**
* @file ParticleInterface.cc
* @brief class to control praticle effects
*/

#include "OrxonoxStableHeaders.h"

#include "ParticleEmitter.h"

#include "tools/ParticleInterface.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/Scene.h"

namespace orxonox
{
    CreateFactory(ParticleEmitter);

    ParticleEmitter::ParticleEmitter(BaseObject* creator) : PositionableEntity(creator)
    {
        RegisterObject(ParticleEmitter);

        assert(this->getScene());
        assert(this->getScene()->getSceneManager());

        this->particles_ = 0;
        this->LOD_ = LODParticle::normal;

        this->registerVariables();
    }

    ParticleEmitter::~ParticleEmitter()
    {
        if (this->isInitialized() && this->particles_)
        {
            this->detachOgreObject(this->particles_->getParticleSystem());
            delete this->particles_;
        }
    }

    void ParticleEmitter::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ParticleEmitter, XMLPort, xmlelement, mode);

        XMLPortParam(ParticleEmitter, "lod",    setLODxml, getLODxml, xmlelement, mode).defaultValues(LODParticle::normal);
        XMLPortParam(ParticleEmitter, "source", setSource, getSource, xmlelement, mode);
    }

    void ParticleEmitter::registerVariables()
    {
        REGISTERSTRING(this->source_, network::direction::toclient, new network::NetworkCallback<ParticleEmitter>(this, &ParticleEmitter::sourceChanged));
        REGISTERDATA  (this->LOD_,    network::direction::toclient, new network::NetworkCallback<ParticleEmitter>(this, &ParticleEmitter::LODchanged));
    }

    void ParticleEmitter::changedVisibility()
    {
        SUPER(ParticleEmitter, changedVisibility);

        if (this->particles_)
            this->particles_->setVisible(this->isVisible());
    }

    void ParticleEmitter::changedActivity()
    {
        SUPER(ParticleEmitter, changedActivity);

        if (this->particles_)
            this->particles_->setEnabled(this->isActive());
    }

    void ParticleEmitter::sourceChanged()
    {
        if (this->particles_)
            delete this->particles_;

        if (this->getScene() && this->getScene()->getSceneManager())
        {
            try
            {
                this->particles_ = new ParticleInterface(this->getScene()->getSceneManager(), this->source_, this->LOD_);
                this->attachOgreObject(particles_->getParticleSystem());
                this->particles_->setVisible(this->isVisible());
                this->particles_->setEnabled(this->isActive());
            }
            catch (...)
            {
                COUT(1) << "Error: Couln't load particle effect \"" << this->source_ << "\"" << std::endl;
                this->particles_ = 0;
            }
        }
        else
            this->particles_ = 0;
    }

    void ParticleEmitter::LODchanged()
    {
        if (this->particles_)
            this->particles_->setDetailLevel(this->LOD_);
    }
}
