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

#include "Model.h"

#include <OgreEntity.h>

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "Scene.h"
#include "RenderQueueListener.h"
#include "graphics/MeshLodInformation.h"
#include "Level.h"

namespace orxonox
{
    CreateFactory(Model);

    Model::Model(BaseObject* creator) :
        StaticEntity(creator), bCastShadows_(true), renderQueueGroup_(RENDER_QUEUE_STENCIL_OBJECTS), lodLevel_(5), bLodEnabled_(true), numLodLevels_(10), lodReductionRate_(.15f)
    {
        RegisterObject(Model);

        this->setConfigValues();
        this->registerVariables();
    }

    Model::~Model()
    {
        if (this->isInitialized() && this->mesh_.getEntity())
            this->detachOgreObject(this->mesh_.getEntity());
    }

    void Model::setConfigValues()
    {
        SetConfigValueExternal(bGlobalEnableLod_, "GraphicsSettings", "enableMeshLoD", true)
            .description("Enable level of detail for models");
    }

    void Model::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Model, XMLPort, xmlelement, mode);

        XMLPortParam(Model, "lodLevel", setLodLevel, getLodLevel, xmlelement, mode);

        XMLPortParam(Model, "mesh", setMeshSource, getMeshSource, xmlelement, mode);
        XMLPortParam(Model, "renderQueueGroup", setRenderQueueGroup, getRenderQueueGroup, xmlelement, mode);
        XMLPortParam(Model, "material", setMaterial, getMaterial, xmlelement, mode);
        XMLPortParam(Model, "shadow", setCastShadows, getCastShadows, xmlelement, mode).defaultValues(true);
    }
    
    /**
    @brief
        This function turns a string from XML Port into a usable ID for the rendering system
        It defaults to the main queue if the group isn't recognized.
        
    @param renderQueueGroup
        This is a string representing the render queue group. Accepted values:
        'main', 'stencil glow', 'stencil object'
    */
    const unsigned int Model::getRenderQueueGroupID(const std::string& renderQueueGroup) const
    {
        if(renderQueueGroup.compare("stencil glow")==0)
        {
            return RENDER_QUEUE_STENCIL_GLOW;
        }
        if(renderQueueGroup.compare("stencil object")==0)
        {
            return RENDER_QUEUE_STENCIL_OBJECTS;
        }
        return RENDER_QUEUE_MAIN;
    }

    void Model::registerVariables()
    {
        registerVariable(this->meshSrc_,    VariableDirection::ToClient, new NetworkCallback<Model>(this, &Model::changedMesh));
        registerVariable(this->renderQueueGroup_,    VariableDirection::ToClient, new NetworkCallback<Model>(this, &Model::changedRenderQueueGroup));
        registerVariable(this->materialName_,    VariableDirection::ToClient, new NetworkCallback<Model>(this, &Model::changedMaterial));
        registerVariable(this->bCastShadows_, VariableDirection::ToClient, new NetworkCallback<Model>(this, &Model::changedShadows));
    }

    float Model::getBiggestScale(Vector3 scale3d)
    {
        float scaleFactor = scale3d.x;
        if(scale3d.y>scaleFactor)
            scaleFactor = scale3d.y;
        if(scale3d.z>scaleFactor)
            scaleFactor = scale3d.z;
        return scaleFactor;
    }

    void Model::changedMesh()
    {
        if (GameMode::showsGraphics())
        {
            if (this->mesh_.getEntity())
                this->detachOgreObject(this->mesh_.getEntity());

            this->mesh_.setMeshSource(this->getScene()->getSceneManager(), this->meshSrc_);

            if (this->mesh_.getEntity())
            {
                this->attachOgreObject(this->mesh_.getEntity());
                this->mesh_.getEntity()->setCastShadows(this->bCastShadows_);
                this->mesh_.getEntity()->setRenderQueueGroup(this->renderQueueGroup_);
                this->mesh_.setVisible(this->isVisible());

                if (this->bGlobalEnableLod_)
                    this->enableLod();
            }
        }
    }

    void Model::changedRenderQueueGroup()
    {
        if (GameMode::showsGraphics())
        {
            if (this->mesh_.getEntity())
            {
                this->mesh_.getEntity()->setRenderQueueGroup(this->renderQueueGroup_);
            }
        }
    }

    void Model::changedMaterial()
    {
        this->mesh_.setMaterial(this->materialName_);
    }

    void Model::changedShadows()
    {
        this->mesh_.setCastShadows(this->bCastShadows_);
    }

    void Model::changedVisibility()
    {
        SUPER(Model, changedVisibility);

        this->mesh_.setVisible(this->isVisible());
    }

    void Model::enableLod()
    {
        //LOD
        if( this->mesh_.getEntity()->getMesh()->getNumLodLevels()==1 )
        {
            Level* level = this->getLevel();

            assert( level != 0 );

            MeshLodInformation* lodInfo = level->getLodInfo(this->meshSrc_);
            if( lodInfo )
            {
                setLodLevel(lodInfo->getLodLevel());
                this->bLodEnabled_ = lodInfo->getEnabled();
                this->numLodLevels_ = lodInfo->getNumLevels();
                this->lodReductionRate_ = lodInfo->getReductionRate();
            }
            if( this->numLodLevels_>10 )
            {
                orxout(internal_warning, context::lod) << "More than 10 LoD levels requested. Creating only 10." << endl;
                this->numLodLevels_ = 10;
            }
            if( this->bLodEnabled_ )
            {
                float volume = this->mesh_.getEntity()->getBoundingBox().volume();
/*
                float scaleFactor = 1;

                BaseObject* creatorPtr = this;

                while(creatorPtr!=NULL&&orxonox_cast<WorldEntity*>(creatorPtr))
                {
                    scaleFactor *= getBiggestScale(((WorldEntity*) creatorPtr)->getScale3D());
                    creatorPtr = creatorPtr->getCreator();
                }
                orxout() << "name: " << this->meshSrc_ << "scaleFactor: " << scaleFactor << ", volume: " << volume << endl;
*/
                orxout(verbose, context::lod) << "Setting lodLevel for " << this->meshSrc_<< " with lodLevel_: " << this->lodLevel_ <<" and volume: "<< volume << ":" << endl;

#if OGRE_VERSION >= 0x010700
                Ogre::Mesh::LodValueList distList;
#else
                Ogre::Mesh::LodDistanceList distList;
#endif

                if( lodLevel_>0 )
                {
//                    float factor = scaleFactor*5/lodLevel_;
                    float factor = pow(volume, 2.0f / 3.0f) * 15.0f / lodLevel_;

                    orxout(verbose, context::lod) << "LodLevel set with factor: " << factor << endl;

                    distList.push_back(70.0f*factor);
                    distList.push_back(140.0f*factor);
                    distList.push_back(170.0f*factor);
                    distList.push_back(200.0f*factor);
                    distList.push_back(230.0f*factor);
                    distList.push_back(250.0f*factor);
                    distList.push_back(270.0f*factor);
                    distList.push_back(290.0f*factor);
                    distList.push_back(310.0f*factor);
                    distList.push_back(330.0f*factor);
                    while(distList.size()>this->numLodLevels_)
                        distList.pop_back();


                    //Generiert LOD-Levels
                    this->mesh_.getEntity()->getMesh()->generateLodLevels(distList, Ogre::ProgressiveMesh::VRQ_PROPORTIONAL, this->lodReductionRate_);
                }
                else
                {
                    std::string what;
                    if(lodLevel_>5)
                        what = ">5";
                    else
                        what = "<0";

                    orxout(verbose, context::lod) << "LodLevel not set because lodLevel(" << lodLevel_ << ") was " << what << "." << endl;
                }
            }
            else
                orxout(verbose, context::lod) << "LodLevel for " << this->meshSrc_ << " not set because is disabled." << endl;
        }
    }
}
