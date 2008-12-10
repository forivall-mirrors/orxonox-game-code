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

#include "MovableEntity.h"
#include "tools/Mesh.h"
#include "objects/Tickable.h"
#include "Camera.h"

namespace orxonox
{

    class _OrxonoxExport Planet : public MovableEntity
    {
        public:
            Planet(BaseObject* creator);
            
            virtual ~Planet();
            
            void init();
            virtual void tick(float dt);
            
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            
            virtual void changedVisibility();
            
            inline void setMeshSource(const std::string& meshname)
                { this->meshSrc_ = meshname; this->changedMesh(); }
                
            inline const std::string& getMeshSource() const
                { return this->meshSrc_; }

            inline void setCastShadows(bool bCastShadows)
                { this->bCastShadows_ = bCastShadows; this->changedShadows(); }
                
            inline bool getCastShadows() const
                { return this->bCastShadows_; }            
            
            inline const std::string& getMesh() const{
                return this->meshSrc_;
            }
            
            inline void setAtmosphereSize(Real size){
                this->atmosphereSize = size;
            }
            
            inline Real getAtmosphereSize(){
                return this->atmosphereSize;
            }
            
            inline void setAtmosphere(const std::string& atmosphere){
                this->atmosphere_ = atmosphere;
            }
            
            inline const std::string& getAtmosphere(){
                return this->atmosphere_;
            }
            
            inline void setImageSize(Real size){
                this->imageSize = size;
            }
            
            inline Real getImageSize(){
                return this->imageSize;
            }

        protected:
            void registerVariables();

        private:
        
            void changedMesh();
            void changedShadows();
            
            std::string meshSrc_;
            std::string atmosphere_;
            Mesh mesh_;
            Real atmosphereSize;
            Real imageSize;
            Ogre::Mesh::LodDistanceList distList;
            BillboardSet billboard_;
            bool bCastShadows_;
			Camera* activeCamera_;

    };
}

#endif

