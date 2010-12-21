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

#include "ParticleInterface.h"

#include <cassert>
#include <string>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreSceneManager.h>

#include "util/Convert.h"
#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/GameMode.h"

namespace orxonox
{
    unsigned int ParticleInterface::counter_s = 0;
    ParticleInterface* ParticleInterface::currentParticleInterface_s = 0;

    ParticleInterface::ParticleInterface(Ogre::SceneManager* scenemanager, const std::string& templateName, LODParticle::Value detaillevel)
    {
        RegisterObject(ParticleInterface);

        assert(scenemanager);

        this->scenemanager_ = scenemanager;
        this->particleSystem_ = 0;

        this->bEnabled_ = true;
        this->bVisible_ = true;
        this->bAllowedByLOD_ = true;
        this->speedFactor_ = 1.0f;

        this->setDetailLevel(static_cast<unsigned int>(detaillevel));

        this->setConfigValues();

        if (GameMode::showsGraphics())
        {
            try
            {
                this->particleSystem_ = this->scenemanager_->createParticleSystem("particles" + multi_cast<std::string>(ParticleInterface::counter_s++), templateName);
                this->setSpeedFactor(1.0f);
            }
            catch (...)
            {
                COUT(1) << "Error: Couln't load particle system \"" << templateName << '"' << std::endl;
                this->particleSystem_ = 0;
            }
        }
    }

    ParticleInterface::~ParticleInterface()
    {
        if (this->particleSystem_)
        {
            this->particleSystem_->removeAllEmitters();
            this->scenemanager_->destroyParticleSystem(this->particleSystem_);
        }
    }

    void ParticleInterface::setConfigValues()
    {
        SetConfigValueExternal(globalDetailLevel_, "GraphicsSettings", "particlesDetailLevel", 2)
            .description("O: off, 1: low, 2: normal, 3: high")
            .callback(this, &ParticleInterface::detailLevelChanged);
    }

    Ogre::ParticleEmitter* ParticleInterface::createNewEmitter()
    {
        if (this->particleSystem_ && this->particleSystem_->getNumEmitters() > 0)
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
        if (this->particleSystem_ && (emitterNr < this->particleSystem_->getNumEmitters()))
            return this->particleSystem_->getEmitter(emitterNr);
        else
            return 0;
    }
    void ParticleInterface::removeEmitter(unsigned int emitterNr)
    {
        if (this->particleSystem_ && (emitterNr < this->particleSystem_->getNumEmitters()))
            this->particleSystem_->removeEmitter(emitterNr);
    }
    void ParticleInterface::removeAllEmitters()
    {
        if (this->particleSystem_)
            this->particleSystem_->removeAllEmitters();
    }
    unsigned int ParticleInterface::getNumEmitters() const
    {
        if (this->particleSystem_)
            return this->particleSystem_->getNumEmitters();
        else
            return 0;
    }

    Ogre::ParticleAffector* ParticleInterface::addAffector(const std::string& name)
    {
        if (this->particleSystem_)
            return this->particleSystem_->addAffector(name);
        else
            return 0;
    }
    Ogre::ParticleAffector* ParticleInterface::getAffector(unsigned int affectorNr)
    {
        if (this->particleSystem_ && (affectorNr < this->particleSystem_->getNumAffectors()))
            return this->particleSystem_->getAffector(affectorNr);
        else
            return 0;
    }
    void ParticleInterface::removeAffector(unsigned int affectorNr)
    {
        if (this->particleSystem_ && (affectorNr < this->particleSystem_->getNumAffectors()))
            this->particleSystem_->removeAffector(affectorNr);
    }
    void ParticleInterface::removeAllAffectors()
    {
        if (this->particleSystem_)
            this->particleSystem_->removeAllAffectors();
    }
    unsigned int ParticleInterface::getNumAffectors() const
    {
        if (this->particleSystem_)
            return this->particleSystem_->getNumAffectors();
        else
            return 0;
    }

    void ParticleInterface::setEnabled(bool enable)
    {
        this->bEnabled_ = enable;

        if (this->particleSystem_)
            for (unsigned int i = 0; i < this->particleSystem_->getNumEmitters(); i++)
                this->particleSystem_->getEmitter(i)->setEnabled(this->bEnabled_ && this->bAllowedByLOD_);
    }

    void ParticleInterface::setVisible(bool visible)
    {
        this->bVisible_ = visible;

        if (this->particleSystem_)
            this->particleSystem_->setVisible(this->bVisible_ && this->bAllowedByLOD_);
    }

    void ParticleInterface::setDetailLevel(unsigned int level)
    {
        this->detaillevel_ = level;
        if (GameMode::showsGraphics())
            this->detailLevelChanged();
    }

    void ParticleInterface::detailLevelChanged()
    {
        if (this->globalDetailLevel_ >= this->detaillevel_)
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
        this->speedFactor_ = factor;

        if (this->particleSystem_)
            this->particleSystem_->setSpeedFactor(factor * this->getTimeFactor());
    }
    void ParticleInterface::changedTimeFactor(float factor_new, float factor_old)
    {
        this->setSpeedFactor(this->speedFactor_);
    }

    bool ParticleInterface::getKeepParticlesInLocalSpace() const
    {
        if (this->particleSystem_)
            return this->particleSystem_->getKeepParticlesInLocalSpace();
        else
            return false;
    }
    void ParticleInterface::setKeepParticlesInLocalSpace(bool keep)
    {
        if (this->particleSystem_)
            this->particleSystem_->setKeepParticlesInLocalSpace(keep);
    }
}
