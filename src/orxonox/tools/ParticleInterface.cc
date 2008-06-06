/*
*   ORXONOX - the hottest 3D action shooter ever to exist
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
*      ...
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

#include "GraphicsEngine.h"
#include "Orxonox.h"
#include "util/Convert.h"

namespace orxonox
{
  unsigned int ParticleInterface::counter_s = 0;
  ParticleInterface* ParticleInterface::currentParticleInterface_s = 0;

  ParticleInterface::ParticleInterface(const std::string& templateName)
  {
    this->sceneNode_ = 0;
    this->particleSystem_ = GraphicsEngine::getSingleton().getSceneManager()->createParticleSystem("particles" + getConvertedValue<unsigned int, std::string>(ParticleInterface::counter_s++), templateName);
    this->particleSystem_->setSpeedFactor(Orxonox::getSingleton()->getTimeFactor());
  }

  ParticleInterface::~ParticleInterface()
  {
    this->particleSystem_->removeAllEmitters();
    GraphicsEngine::getSingleton().getSceneManager()->destroyParticleSystem(particleSystem_);
  }

  void ParticleInterface::addToSceneNode(Ogre::SceneNode* sceneNode)
  {
    this->sceneNode_ = sceneNode;
    this->sceneNode_->attachObject(this->particleSystem_);
  }

  void ParticleInterface::detachFromSceneNode()
  {
    if (this->sceneNode_)
    {
      this->sceneNode_->detachObject(this->particleSystem_);
      this->sceneNode_ = 0;
    }
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
    for (unsigned int i = 0; i < this->particleSystem_->getNumEmitters(); i++)
      this->particleSystem_->getEmitter(i)->setEnabled(enable);
  }

  void ParticleInterface::setSpeedFactor(float factor)
  {
    this->particleSystem_->setSpeedFactor(Orxonox::getSingleton()->getTimeFactor() * factor);
  }
  float ParticleInterface::getSpeedFactor() const
  {
    return this->particleSystem_->getSpeedFactor();
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
