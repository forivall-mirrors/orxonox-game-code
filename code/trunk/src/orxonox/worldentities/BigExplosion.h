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
 *      Benjamin de Capitani
 *   Co-authors:
 *      ...
 *
 */

#ifndef _BigExplosion_H__
#define _BigExplosion_H__

#include "OrxonoxPrereqs.h"

#include "tools/Timer.h"
#include "MobileEntity.h"

namespace orxonox
{
    class _OrxonoxExport BigExplosion : public MobileEntity
    {
        public:
            BigExplosion(Context* context);
            virtual ~BigExplosion();

            inline void setLOD(LODParticle::Value level)
                { this->LOD_ = level; this->LODchanged(); }
            inline LODParticle::Value getLOD() const
                { return this->LOD_; }

        private:
            void registerVariables();

            void LODchanged();
            void checkStop();
            void stop();
            void init();
            void initZero();


            bool                  bStop_;

            MobileEntity*         explosion_;

            MovableEntity*        debrisEntity1_;
            MovableEntity*        debrisEntity2_;
            MovableEntity*        debrisEntity3_;
            MovableEntity*        debrisEntity4_;

            Model*                debris1_;
            Model*                debris2_;
            Model*                debris3_;
            Model*                debris4_;



            ParticleInterface*    debrisSmoke1_;
            ParticleInterface*    debrisSmoke2_;
            ParticleInterface*    debrisSmoke3_;
            ParticleInterface*    debrisSmoke4_;

            ParticleInterface*    debrisFire1_;
            ParticleInterface*    debrisFire2_;
            ParticleInterface*    debrisFire3_;
            ParticleInterface*    debrisFire4_;

            ParticleInterface*    explosionSmoke_;
            ParticleInterface*    explosionFire_;

            LODParticle::Value    LOD_;
            Timer                 destroyTimer_;
    };
}

#endif /* _BigExplosion_H__ */
