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


#include "OrxonoxStableHeaders.h"
#include "Planet.h"

#include "tinyxml/tinyxml.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"
#include "core/XMLPort.h"
#include "CameraManager.h"
#include "objects/Scene.h"
#include "Camera.h"
#include <math.h>

#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreMeshManager.h>


namespace orxonox
{
    CreateFactory(Planet);    

    /**
     * @brief Constructor
     */
    Planet::Planet(BaseObject* creator): MovableEntity(creator)
    {
        RegisterObject(Planet);
        this->registerVariables();
    }

    /**
     * @brief Destructor
     */
    Planet::~Planet()
    {
        if (this->isInitialized() && this->mesh_.getEntity())
            this->getNode()->detachObject(this->mesh_.getEntity());
    }    
    
    void Planet::tick(float dt)
    {
        if(!this->isVisible())
            return;
            
        Camera* activeCamera = CameraManager::getInstance().getActiveCamera();
	if(activeCamera)
	{
            Real distance = this->getPosition().distance( activeCamera->getWorldPosition() );
            COUT(2) << distance << std::endl;
            Real planetRadius = this->getScale();
        
            Real newScale = 2 * distance / sqrt(distance*distance - planetRadius*planetRadius);
            Real tempTest = newScale*(1+Real(this->atmosphereSize)/Real(this->imageSize));
            newScale = tempTest;
        
            this->billboard_.getBillboardSet()->setDefaultDimensions(newScale, newScale);
	}
                
        SUPER(Planet, tick, dt);
    }
  
    void Planet::init(){
    
        Real scaleFactor = this->getScale();
        
        this->distList.push_back(10.0*scaleFactor);
        this->distList.push_back(19.0*scaleFactor);
        this->distList.push_back(27.0*scaleFactor);
        this->distList.push_back(34.0*scaleFactor);
        this->distList.push_back(40.0*scaleFactor);
        this->distList.push_back(45.0*scaleFactor);
        this->distList.push_back(49.0*scaleFactor);
        this->distList.push_back(52.0*scaleFactor);
        this->distList.push_back(54.0*scaleFactor);
        this->distList.push_back(55.0*scaleFactor);
        
        Real reductionValue = 0.2;
        
        this->mesh_.getEntity()->getMesh()->generateLodLevels(distList, Ogre::ProgressiveMesh::VRQ_PROPORTIONAL, reductionValue);
        billboard_.setBillboardSet(this->getScene()->getSceneManager(), this->atmosphere_, Vector3(0,0,0));
        
        this->getNode()->attachObject(this->billboard_.getBillboardSet());    
        this->billboard_.getBillboardSet()->setUseAccurateFacing(true);
        this->setCastShadows(true);
        this->billboard_.getBillboardSet()->setRenderQueueGroup(this->mesh_.getEntity()->getRenderQueueGroup());
        this->mesh_.setCastShadows(true);
    }
    
    void Planet::changedMesh()
    {
COUT(0) << "changedMesh callback -===================" << endl;
        if (this->mesh_.getEntity())
            this->getNode()->detachObject(this->mesh_.getEntity());

        this->mesh_.setMeshSource(this->getScene()->getSceneManager(), this->meshSrc_);

        if (this->mesh_.getEntity())
        {
            this->getNode()->attachObject(this->mesh_.getEntity());
            this->mesh_.getEntity()->setCastShadows(this->bCastShadows_);
            this->mesh_.setVisible(this->isVisible());
        }
        this->init();
    }

    void Planet::changedShadows()
    {
        this->mesh_.setCastShadows(this->bCastShadows_);
    }

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void Planet::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Planet, XMLPort, xmlelement, mode);
        
        XMLPortParam(Planet, "atmosphere", setAtmosphere, getAtmosphere, xmlelement, mode).defaultValues("planet/Atmosphere");
        XMLPortParam(Planet, "atmospheresize", setAtmosphereSize, getAtmosphereSize, xmlelement,mode);     
        XMLPortParam(Planet, "imagesize", setImageSize, getImageSize, xmlelement,mode);         
        XMLPortParam(Planet, "mesh", setMeshSource, getMeshSource, xmlelement, mode);
        XMLPortParam(Planet, "shadow", setCastShadows, getCastShadows, xmlelement, mode).defaultValues(true);
        
    }

    void Planet::registerVariables(){
	REGISTERSTRING(this->atmosphere_, direction::toclient);
        REGISTERSTRING(this->meshSrc_, direction::toclient, new NetworkCallback<Planet>(this, &Planet::changedMesh));
        REGISTERDATA(this->bCastShadows_, direction::toclient, new NetworkCallback<Planet>(this, &Planet::changedShadows));
	REGISTERDATA(this->atmosphereSize, direction::toclient);
	REGISTERDATA(this->imageSize, direction::toclient);
    }

    void Planet::changedVisibility()
    {
        SUPER(Planet, changedVisibility);
        if (this->isInitialized())
            this->mesh_.setVisible(this->isVisible());
            this->billboard_.setVisible(this->isVisible());
    }
}
