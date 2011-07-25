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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#ifndef _ParticleInterface_H__
#define _ParticleInterface_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <OgreParticleEmitter.h>

#include "core/OrxonoxClass.h"
#include "util/Math.h"

#define getAllEmitters() \
  storeThisAsCurrentParticleInterface(); \
  for (unsigned int i = 0; i < ParticleInterface::getCurrentParticleInterface()->getNumEmitters(); ++i) \
    ParticleInterface::getCurrentParticleInterface()->getEmitter(i)

namespace orxonox
{
  class _OrxonoxExport ParticleInterface : public OrxonoxClass
  {
    public:
      ParticleInterface(const std::string& templateName, LODParticle::LOD detaillevel);
      ~ParticleInterface();

      inline Ogre::ParticleSystem* getParticleSystem() const
        { return this->particleSystem_; }

      void addToSceneNode(Ogre::SceneNode* sceneNode);
      void detachFromSceneNode();

      Ogre::ParticleEmitter* createNewEmitter();
      Ogre::ParticleEmitter* getEmitter(unsigned int emitterNr) const;
      void removeEmitter(unsigned int emitterNr);
      void removeAllEmitters();
      unsigned int getNumEmitters() const;

      Ogre::ParticleAffector* addAffector(const std::string& name);
      Ogre::ParticleAffector* getAffector(unsigned int affectorNr) const;
      void removeAffector(unsigned int affectorNr);
      void removeAllAffectors();
      unsigned int getNumAffectors() const;

      float getSpeedFactor() const;
      void setSpeedFactor(float factor);
      bool getKeepParticlesInLocalSpace() const;
      void setKeepParticlesInLocalSpace(bool keep);

      void setEnabled(bool enable);
      void detailLevelChanged(unsigned int newlevel);

      inline void storeThisAsCurrentParticleInterface()
        { ParticleInterface::currentParticleInterface_s = this; }
      inline static ParticleInterface* getCurrentParticleInterface()
        { return ParticleInterface::currentParticleInterface_s; }

    private:
      void updateVisibility();

      static ParticleInterface* currentParticleInterface_s;
      static unsigned int counter_s;
      Ogre::SceneNode* sceneNode_;
      Ogre::ParticleSystem* particleSystem_;
      bool bVisible_;
      bool bEnabled_;
      unsigned int detaillevel_;                            //!< Detail level of this particle effect (0: off, 1: low, 2: normal, 3: high)
  };
}

#endif /* _ParticleInterface_H__ */
