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

#ifndef _Model_H__
#define _Model_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "tools/Mesh.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport Model : public StaticEntity
    {
        public:
            Model(BaseObject* creator);
            virtual ~Model();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            virtual void changedVisibility();

            inline const Mesh& getMesh() const
                { return this->mesh_; }

            inline void setMeshSource(const std::string& meshname)
                { this->meshSrc_ = meshname; this->changedMesh(); }
            inline const std::string& getMeshSource() const
                { return this->meshSrc_; }

            inline void setCastShadows(bool bCastShadows)
                { this->bCastShadows_ = bCastShadows; this->changedShadows(); }
            inline bool getCastShadows() const
                { return this->bCastShadows_; }

        private:
            void changedMesh();
            void changedShadows();

            std::string meshSrc_;
            Mesh mesh_;
            bool bCastShadows_;
    };
}

#endif /* _Model_H__ */
