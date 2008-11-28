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
            ParticleInterface(Ogre::SceneManager* scenemanager, const std::string& templateName, LODParticle::LOD detaillevel);
            virtual ~ParticleInterface();

            inline Ogre::ParticleSystem* getParticleSystem() const
                { return this->particleSystem_; }

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
            inline bool isEnabled() const
                { return this->bEnabled_; }

            void setVisible(bool visible);
            inline bool isVisible() const
                { return this->bVisible_; }

            void detailLevelChanged(unsigned int newlevel);
            void setDetailLevel(unsigned int level);

            inline void storeThisAsCurrentParticleInterface()
                { ParticleInterface::currentParticleInterface_s = this; }
            inline static ParticleInterface* getCurrentParticleInterface()
                { return ParticleInterface::currentParticleInterface_s; }

        private:
            void updateVisibility();

            static ParticleInterface* currentParticleInterface_s;
            static unsigned int       counter_s;

            Ogre::ParticleSystem*     particleSystem_;
            bool                      bVisible_;
            bool                      bEnabled_;
            bool                      bAllowedByLOD_;
            unsigned int              detaillevel_;     //!< Detail level of this particle effect (0: off, 1: low, 2: normal, 3: high)
            Ogre::SceneManager*       scenemanager_;
    };
}

#endif /* _ParticleInterface_H__ */
