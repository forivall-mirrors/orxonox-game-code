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

#ifndef _ParticleEmitter_H__
#define _ParticleEmitter_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport ParticleEmitter : public StaticEntity
    {
        public:
            ParticleEmitter(BaseObject* creator);
            ~ParticleEmitter();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            virtual void changedVisibility();
            virtual void changedActivity();

            inline ParticleInterface* getParticleInterface() const
                { return this->particles_; }

            inline void setSource(const std::string& source)
                { this->source_ = source; this->sourceChanged(); }
            inline const std::string& getSource() const
                { return this->source_; }

            inline void setLOD(LODParticle::Value level)
                { this->LOD_ = level; this->LODchanged(); }
            inline LODParticle::Value getLOD() const
                { return this->LOD_; }

        protected:
            inline void setLODxml(unsigned int level)
                { this->LOD_ = static_cast<LODParticle::Value>(level); this->LODchanged(); }
            inline unsigned int getLODxml() const
                { return static_cast<unsigned int>(this->LOD_); }

            void sourceChanged();
            void LODchanged();

            ParticleInterface* particles_;
            std::string        source_;
            LODParticle::Value   LOD_;
    };
}

#endif /* _ParticleEmitter_H__ */
