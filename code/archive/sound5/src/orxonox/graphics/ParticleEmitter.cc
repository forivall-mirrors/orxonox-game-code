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
* @file
* @brief class to control praticle effects
*/

#include "ParticleEmitter.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "tools/ParticleInterface.h"
#include "Scene.h"

namespace orxonox
{
    CreateFactory(ParticleEmitter);

    ParticleEmitter::ParticleEmitter(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(ParticleEmitter);

        if (GameMode::showsGraphics() && (!this->getScene() || !this->getScene()->getSceneManager()))
            ThrowException(AbortLoading, "Can't create ParticleEmitter, no scene or no scene manager given.");

        this->particles_ = 0;
        this->LOD_ = LODParticle::Normal;

        this->registerVariables();
    }

    ParticleEmitter::~ParticleEmitter()
    {
        if (this->isInitialized() && this->particles_)
        {
            this->detachOgreObject(this->particles_->getParticleSystem());
            this->particles_->destroy();
        }
    }

    void ParticleEmitter::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ParticleEmitter, XMLPort, xmlelement, mode);

        XMLPortParam(ParticleEmitter, "lod",    setLODxml, getLODxml, xmlelement, mode).defaultValues(LODParticle::Normal);
        XMLPortParam(ParticleEmitter, "source", setSource, getSource, xmlelement, mode);
    }

    void ParticleEmitter::registerVariables()
    {
        registerVariable(this->source_, VariableDirection::ToClient, new NetworkCallback<ParticleEmitter>(this, &ParticleEmitter::sourceChanged));
        registerVariable((int&)(this->LOD_),    VariableDirection::ToClient, new NetworkCallback<ParticleEmitter>(this, &ParticleEmitter::LODchanged));
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
        {
            this->particles_->destroy();
            this->particles_ = 0;
        }

        if (GameMode::showsGraphics() && this->getScene() && this->getScene()->getSceneManager())
        {
            try
            {
                this->particles_ = new ParticleInterface(this->getScene()->getSceneManager(), this->source_, this->LOD_);
                this->attachOgreObject(this->particles_->getParticleSystem());
                this->particles_->setVisible(this->isVisible());
                this->particles_->setEnabled(this->isActive());
            }
            catch (...)
            {
                COUT(1) << "Error: Couln't load particle effect \"" << this->source_ << "\" because:" << std::endl
                        << Exception::handleMessage() << std::endl;
            }
        }
    }

    void ParticleEmitter::LODchanged()
    {
        if (this->particles_)
            this->particles_->setDetailLevel(this->LOD_);
    }
}
