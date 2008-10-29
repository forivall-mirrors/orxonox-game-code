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
 *      Marian Runo
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Planet_H__
#define _Planet_H__
#include "tools/BillboardSet.h"
#include "OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "../tools/Mesh.h"

namespace orxonox
{

    class _OrxonoxExport Planet : public WorldEntity
    {
        public:
            Planet();
            
            ~Planet();
            
            void init();
            
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            
            virtual void changedVisibility();
            
            inline void setMesh(const std::string& meshname){
                this->meshSrc_ = meshname;
            }
                
            inline const std::string& getMesh() const{
                return this->meshSrc_;
            }
                
            virtual bool create();

        protected:
            void registerAllVariables();

        private:
            std::string meshSrc_;
            Mesh mesh_;
            Ogre::Mesh::LodDistanceList distList;
            BillboardSet billboard_;

    };
}

#endif /* _Planet_H__ */
