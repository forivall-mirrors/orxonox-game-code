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

#ifndef _ExplosionChunk_H__
#define _ExplosionChunk_H__

#include "OrxonoxPrereqs.h"

#include "tools/Timer.h"
#include "MovableEntity.h"

namespace orxonox
{
    class _OrxonoxExport ExplosionChunk : public MovableEntity
    {
        public:
            ExplosionChunk(Context* context);
            virtual ~ExplosionChunk();

            virtual void tick(float dt);

            inline void setLOD(LODParticle::Value level)
                { this->LOD_ = level; this->LODchanged(); }
            inline LODParticle::Value getLOD() const
                { return this->LOD_; }

        private:
            void registerVariables();
            void LODchanged();
            void checkStop();
            void stop();

            bool                  bStop_;
            ParticleInterface*    fire_;
            ParticleInterface*    smoke_;
            LODParticle::Value    LOD_;
            Timer                 destroyTimer_;
    };
}

#endif /* _ExplosionChunk_H__ */
