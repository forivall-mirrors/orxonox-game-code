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
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "Scene.h"
#include "graphics/MeshLodInformation.h"
#include "Level.h"

namespace orxonox
{
    CreateFactory(Model);

    Model::Model(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Model);

        this->bCastShadows_ = true;

        this->registerVariables();
        //LoD
        this->lodLevel_=5;
    }

    Model::~Model()
    {
        if (this->isInitialized() && this->mesh_.getEntity())
            this->detachOgreObject(this->mesh_.getEntity());
    }

    void Model::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Model, XMLPort, xmlelement, mode);
        
        XMLPortParam(Model, "lodLevel", setLodLevel, getLodLevel, xmlelement, mode);
        
        XMLPortParam(Model, "mesh", setMeshSource, getMeshSource, xmlelement, mode);
        XMLPortParam(Model, "shadow", setCastShadows, getCastShadows, xmlelement, mode).defaultValues(true);
    }

    void Model::registerVariables()
    {
        registerVariable(this->meshSrc_,    VariableDirection::ToClient, new NetworkCallback<Model>(this, &Model::changedMesh));
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
                this->mesh_.setVisible(this->isVisible());
                
                //LOD
                if(this->mesh_.getEntity()->getMesh()->getNumLodLevels()==1
                    &&this->meshSrc_!="laserbeam.mesh")
                {
                    float scaleFactor = 1;
                    BaseObject* creatorPtr = this;
                    
                    while(creatorPtr!=NULL&&orxonox_cast<WorldEntity*>(creatorPtr))
                    {
                        scaleFactor *= getBiggestScale(((WorldEntity*) creatorPtr)->getScale3D());
                        creatorPtr = creatorPtr->getCreator();
                    }
                    
                    Level* level_ = this->getLevel();
                    
                    MeshLodInformation* lodInfo = level_->getLodInfo(this->meshSrc_);
                    
                    if(lodInfo!=0)
                        setLodLevel(lodInfo->getLodLevel());
                    
                    COUT(4) << "Setting lodLevel for " << this->meshSrc_<< " with lodLevel_: " << this->lodLevel_ <<" and scale: "<< scaleFactor << ":" << std::endl;

#if OGRE_VERSION >= 0x010700
                    Ogre::Mesh::LodValueList distList;
#else
                    Ogre::Mesh::LodDistanceList distList;
#endif

                    if(lodLevel_>0&&lodLevel_<=5)
                    {
                        float factor = scaleFactor*5/lodLevel_;
                        
                        COUT(4)<<"LodLevel set with factor: "<<factor<<std::endl;

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

                        float reductionValue = 0.15f;

                        
                        //Generiert LOD-Levels
                        //this->mesh_.getEntity()->getMesh()->generateLodLevels(distList, Ogre::ProgressiveMesh::VRQ_PROPORTIONAL, reductionValue);
                    }
                    else
                    {
                        std::string what;
                        if(lodLevel_>5)
                            what = ">5";
                        else
                            what = "<0";
                        
                        COUT(4)<<"LodLevel not set because lodLevel("<<lodLevel_<<") was "<<what<<"."<<std::endl;
                    }
                }
            }
        }
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
}
