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

#include "tools/ToolsPrereqs.h"

#include "util/OgreForwardRefs.h"
#include "tools/interfaces/TimeFactorListener.h"

namespace orxonox
{
    class _ToolsExport ParticleInterface : public TimeFactorListener
    {
        public:
            ParticleInterface(Ogre::SceneManager* scenemanager, const std::string& templateName, LODParticle::Value detaillevel);
            virtual ~ParticleInterface();
            void setConfigValues();

            inline Ogre::ParticleSystem* getParticleSystem()
                { return this->particleSystem_; }

            Ogre::ParticleEmitter* createNewEmitter();
            Ogre::ParticleEmitter* getEmitter(unsigned int emitterNr) const;
            void removeEmitter(unsigned int emitterNr);
            void removeAllEmitters();
            unsigned int getNumEmitters() const;

            Ogre::ParticleAffector* addAffector(const std::string& name);
            Ogre::ParticleAffector* getAffector(unsigned int affectorNr);
            void removeAffector(unsigned int affectorNr);
            void removeAllAffectors();
            unsigned int getNumAffectors() const;

            inline float getSpeedFactor() const
                { return this->speedFactor_; }
            void setSpeedFactor(float factor);
            bool getKeepParticlesInLocalSpace() const;
            void setKeepParticlesInLocalSpace(bool keep);

            void setEnabled(bool enable);
            inline bool isEnabled() const
                { return this->bEnabled_; }

            void setVisible(bool visible);
            inline bool isVisible() const
                { return this->bVisible_; }

            void setDetailLevel(unsigned int level);

        protected:
            virtual void changedTimeFactor(float factor_new, float factor_old);

        private:
            void updateVisibility();
            void detailLevelChanged();

            Ogre::ParticleSystem*     particleSystem_;
            Ogre::SceneManager*       scenemanager_;
            bool                      bVisible_;
            bool                      bEnabled_;
            bool                      bAllowedByLOD_;
            unsigned int              detaillevel_;       //!< Detail level of this particle effect (0: off, 1: low, 2: normal, 3: high)
            float                     speedFactor_;

            // config values
            unsigned int              globalDetailLevel_; //!< Global maximum detail level of particle effects (0: off, 1: low, 2: normal, 3: high)

            static unsigned int       counter_s;
    };
}

#endif /* _ParticleInterface_H__ */
