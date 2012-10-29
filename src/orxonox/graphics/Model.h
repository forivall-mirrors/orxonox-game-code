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

/**
  @file Model.h
  @brief Definition of Model Class
*/

#ifndef _Model_H__
#define _Model_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "tools/Mesh.h"
#include "RenderQueueListener.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport Model : public StaticEntity
    {
        /**
        @brief
            The class Model stores a Mesh and some additional properties, so you can easily render any Model and apply different effects to it.
            
            You can assign any Material to any Mesh to completely change the way it looks, to further add versatility you can also assign the Model
            to a render queue group, to enable proper rendering of fancy effect like glowing edges around objects with alpha blending.
        */
        public:
            Model(BaseObject* creator);
            virtual ~Model();

            void setConfigValues();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void changedVisibility();

            inline const Mesh& getMesh() const
                { return this->mesh_; }

            inline void setMeshSource(const std::string& meshname)
                { this->meshSrc_ = meshname; this->changedMesh(); }
            inline const std::string& getMeshSource() const
                { return this->meshSrc_; }

            inline void setRenderQueueGroup(const std::string& renderQueueGroup)
                { this->renderQueueGroup_ = getRenderQueueGroupID(renderQueueGroup); this->changedRenderQueueGroup(); }
            inline const int getRenderQueueGroup() const
                { return this->renderQueueGroup_; }

            inline void setCastShadows(bool bCastShadows)
                { this->bCastShadows_ = bCastShadows; this->changedShadows(); }
            inline bool getCastShadows() const
                { return this->bCastShadows_; }

            inline void setMaterial(const std::string& materialname)
                { this->materialName_ = materialname; this->changedMaterial(); }
            inline const std::string& getMaterial() const
                { return this->materialName_; }

        protected:
            /**
            @brief
                This function turns a string from XML Port into a usable ID for the rendering system
                It defaults to the main queue if the group isn't recognized.
                
            @param renderQueueGroup
                This is a string representing the render queue group. Accepted values:
                main, stencil glow, stencil object
            */
            const unsigned int getRenderQueueGroupID(const std::string& renderQueueGroup) const;

            void registerVariables();
            void changedMesh();
            void changedRenderQueueGroup();
            void changedMaterial();
            void changedShadows();

            //LoD
            void enableLod();

            inline void setLodLevel(float lodLevel)
                { this->lodLevel_ =  lodLevel; }
            inline float getLodLevel() const
                { return this->lodLevel_; }
            float getBiggestScale(Vector3 scale3d);

            std::string meshSrc_; //!< This string stores the path where the mesh is stored
            Mesh mesh_; //!< This is the mesh object linked to this Object, it stores the data from the mesh file in a usable format for the Ogre engine
            bool bCastShadows_; //!< This value determines whether a Model is casting a shadow or not, turn it off to save performance, when not needed
            unsigned int renderQueueGroup_; //!< This variable stores which render queue group this object is assigned to
            std::string materialName_; //!< This string stores the name of the material to be applied to the mesh/model

            //LoD
            bool bGlobalEnableLod_; //!< Has LoD been turned on in the graphics configuration?
            float lodLevel_; //!< Standard LoD Level
            bool bLodEnabled_; //!< Is LoD to be used on this model?
            unsigned int numLodLevels_; //!< How many LoD does this model feature
            float lodReductionRate_; //!< How fast should be switched to lower LoDs

    };
}

#endif /* _Model_H__ */
