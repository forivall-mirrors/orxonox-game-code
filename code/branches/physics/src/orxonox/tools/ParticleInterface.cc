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
#include "ParticleInterface.h"

#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreSceneManager.h>
#include <cassert>

#include "GraphicsEngine.h"
#include "core/CoreIncludes.h"
#include "util/Convert.h"

namespace orxonox
{
    unsigned int ParticleInterface::counter_s = 0;
    ParticleInterface* ParticleInterface::currentParticleInterface_s = 0;

    ParticleInterface::ParticleInterface(Ogre::SceneManager* scenemanager, const std::string& templateName, LODParticle::LOD detaillevel)
    {
        RegisterRootObject(ParticleInterface);

        assert(scenemanager);

        this->scenemanager_ = scenemanager;

        this->bEnabled_ = true;
        this->bVisible_ = true;
        this->bAllowedByLOD_ = true;

        this->particleSystem_ = this->scenemanager_->createParticleSystem("particles" + getConvertedValue<unsigned int, std::string>(ParticleInterface::counter_s++), templateName);
        this->particleSystem_->setSpeedFactor(1.0f);
        //this->particleSystem_->setSpeedFactor(Orxonox::getInstance().getTimeFactor());

        this->setDetailLevel((unsigned int)detaillevel);
    }

    ParticleInterface::~ParticleInterface()
    {
        this->particleSystem_->removeAllEmitters();
        this->scenemanager_->destroyParticleSystem(particleSystem_);
    }

    Ogre::ParticleEmitter* ParticleInterface::createNewEmitter()
    {
        if (this->particleSystem_->getNumEmitters() > 0)
        {
            Ogre::ParticleEmitter* newemitter = this->particleSystem_->addEmitter(this->particleSystem_->getEmitter(0)->getType());
            this->particleSystem_->getEmitter(0)->copyParametersTo(newemitter);
            return newemitter;
        }
        else
            return 0;
    }
    Ogre::ParticleEmitter* ParticleInterface::getEmitter(unsigned int emitterNr) const
    {
        if (emitterNr < this->particleSystem_->getNumEmitters())
            return this->particleSystem_->getEmitter(emitterNr);
        else
            return 0;
    }
    void ParticleInterface::removeEmitter(unsigned int emitterNr)
    {
        if (emitterNr < this->particleSystem_->getNumEmitters())
            this->particleSystem_->removeEmitter(emitterNr);
    }
    void ParticleInterface::removeAllEmitters()
    {
        this->particleSystem_->removeAllEmitters();
    }
    unsigned int ParticleInterface::getNumEmitters() const
    {
        return this->particleSystem_->getNumEmitters();
    }

    Ogre::ParticleAffector* ParticleInterface::addAffector(const std::string& name)
    {
        return this->particleSystem_->addAffector(name);
    }
    Ogre::ParticleAffector* ParticleInterface::getAffector(unsigned int affectorNr) const
    {
        if (affectorNr < this->particleSystem_->getNumAffectors())
            return this->particleSystem_->getAffector(affectorNr);
        else
            return 0;
    }
    void ParticleInterface::removeAffector(unsigned int affectorNr)
    {
        if (affectorNr < this->particleSystem_->getNumAffectors())
            this->particleSystem_->removeAffector(affectorNr);
    }
    void ParticleInterface::removeAllAffectors()
    {
        this->particleSystem_->removeAllAffectors();
    }
    unsigned int ParticleInterface::getNumAffectors() const
    {
        return this->particleSystem_->getNumAffectors();
    }

    void ParticleInterface::setEnabled(bool enable)
    {
        this->bEnabled_ = enable;

        for (unsigned int i = 0; i < this->particleSystem_->getNumEmitters(); i++)
            this->particleSystem_->getEmitter(i)->setEnabled(this->bEnabled_ && this->bAllowedByLOD_);
    }

    void ParticleInterface::setVisible(bool visible)
    {
        this->bVisible_ = visible;

        this->particleSystem_->setVisible(this->bVisible_ && this->bAllowedByLOD_);
    }

    void ParticleInterface::setDetailLevel(unsigned int level)
    {
        this->detaillevel_ = level;
        this->detailLevelChanged(GraphicsEngine::getInstance().getDetailLevelParticle());
    }

    void ParticleInterface::detailLevelChanged(unsigned int newlevel)
    {
        if (newlevel >= (unsigned int)this->detaillevel_)
            this->bAllowedByLOD_ = true;
        else
            this->bAllowedByLOD_ = false;

        this->updateVisibility();
    }

    void ParticleInterface::updateVisibility()
    {
        this->setEnabled(this->isEnabled());
        this->setVisible(this->isVisible());
    }

    void ParticleInterface::setSpeedFactor(float factor)
    {
        //this->particleSystem_->setSpeedFactor(Orxonox::getInstance().getTimeFactor() * factor);
        this->particleSystem_->setSpeedFactor(1.0f * factor);
    }
    float ParticleInterface::getSpeedFactor() const
    {
        //return (this->particleSystem_->getSpeedFactor() / Orxonox::getInstance().getTimeFactor());
        return (this->particleSystem_->getSpeedFactor() / 1.0f);
    }

    bool ParticleInterface::getKeepParticlesInLocalSpace() const
    {
        return this->particleSystem_->getKeepParticlesInLocalSpace();
    }
    void ParticleInterface::setKeepParticlesInLocalSpace(bool keep)
    {
        this->particleSystem_->setKeepParticlesInLocalSpace(keep);
    }
}
