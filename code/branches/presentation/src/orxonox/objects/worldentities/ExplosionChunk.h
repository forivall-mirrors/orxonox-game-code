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

#include "MovableEntity.h"
#include "tools/Timer.h"

namespace orxonox
{
    class _OrxonoxExport ExplosionChunk : public MovableEntity
    {
        public:
            ExplosionChunk(BaseObject* creator);
            virtual ~ExplosionChunk();

            virtual void tick(float dt);
            void registerVariables();

            inline void setLOD(LODParticle::LOD level)
                { this->LOD_ = level; this->LODchanged(); }
            inline LODParticle::LOD getLOD() const
                { return this->LOD_; }

        private:
            void LODchanged();
            void checkStop();
            void stop();
            void destroy();

            bool                  bStop_;
            ParticleInterface*    fire_;
            ParticleInterface*    smoke_;
            LODParticle::LOD      LOD_;
            Timer<ExplosionChunk> destroyTimer_;
    };
}

#endif /* _ExplosionChunk_H__ */
