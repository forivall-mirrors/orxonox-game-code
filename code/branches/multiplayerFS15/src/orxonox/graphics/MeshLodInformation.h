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
 *      Jan Bernegger
 *   Co-authors:
 *      ...
 *
 */

#ifndef _MeshLodInformation_H__
#define _MeshLodInformation_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "tools/Mesh.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport MeshLodInformation : public BaseObject
    {
        public:
            MeshLodInformation(Context* context);
            virtual ~MeshLodInformation();

            float getLodLevel();
            std::string getMeshName();
            bool getEnabled(){ return this->bEnabled_; }
            unsigned int getNumLevels(){ return this->numLevels_; }
            float getReductionRate(){ return this->reductionRate_; }

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        private:
            void setLodLevel(float lodLevel);
            void setMeshSource(std::string meshSource);
            void setEnabled( bool enabled ){ this->bEnabled_ = enabled; }
            void setNumLevels( unsigned int num ){ this->numLevels_ = num; }
            void setReductionRate( float rate ){ this->reductionRate_ = rate; }
            std::string getMeshSource();
            std::string meshSource_;
            float lodLevel_;
            bool bEnabled_;
            unsigned int numLevels_;
            float reductionRate_;

    };
}

#endif /* _MeshLodInformation_H__ */
