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
 *      Si Sun
 *
 */
#include "OrxonoxStableHeaders.h"
#include "Map.h"
 
#include <string>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreNode.h>

#include <OgreRenderWindow.h>
#include <OgreRenderTexture.h>
#include <OgreTexture.h>
#include <OgreViewport.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgreRoot.h>
#include <OgreHardwarePixelBuffer.h>
#include "objects/worldentities/ControllableEntity.h"
#include "objects/worldentities/CameraPosition.h"

#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "objects/Scene.h"
#include "objects/RadarViewable.h"
 
 namespace orxonox
 {
    CreateFactory(Map);
     SetConsoleCommand(Map, openMap, true);
    
    Map::Map(BaseObject* creator) : OrxonoxOverlay(creator)
    {
        RegisterObject(Map);
        
        //Getting Scene Manager (Hack)
        ObjectList<Scene>::iterator it = ObjectList<Scene>::begin();
        this->sManager_ = it->getSceneManager();

        this->mapSceneM_ = Ogre::Root::getSingletonPtr()->createSceneManager( Ogre::ST_GENERIC,"MapScene" );
        rootNode_ = mapSceneM_->getRootSceneNode();
        
        //this->sNode_ = new Ogre::SceneNode(sManager_);
        //oManager_ = Ogre::OverlayManager::getSingletonPtr();
        //overlay_ = oManager_->create("Map");
        //overlay_ is member of OrxonoxOverlay
        
        //Not Showing the map as default
        //this->isVisible_=false;
        //overlay_->hide();
        

        //TestEntity
        //Ogre::Entity * ent = mapSceneM_->createEntity("ent", "drone.mesh");
        
        //rootNode_->attachObject( ent );
        /*sNode_->setPosition(0,0,-50);
        overlay_->add3D(sNode_);
        */
	
        
        

        
        // Alter the camera aspect ratio to match the viewport
        //mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
        mReflectCam_ = mapSceneM_->createCamera("ReflectCam");
        mReflectCam_->setPosition(200,170, -160);
        mReflectCam_->lookAt(0,0,0);
        mReflectCam_->setAspectRatio(1);

        std::string camMat_id = "RttMat";
        Ogre::MaterialPtr material = this->createRenderCamera(mReflectCam_, camMat_id);
/*
        Ogre::TexturePtr rttTex = Ogre::TextureManager::getSingleton().createManual("RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 512, 512, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

        Ogre::RenderTexture *renderTexture = rttTex->getBuffer()->getRenderTarget();

        renderTexture->addViewport(mReflectCam_);
        renderTexture->getViewport(0)->setClearEveryFrame(true);
        renderTexture->getViewport(0)->setBackgroundColour(ColourValue::Black);
        renderTexture->getViewport(0)->setOverlaysEnabled(false);

        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Ogre::Technique *technique = material->createTechnique();
        technique->createPass();
        material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
        material->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex");
*/


        // create overlay
/*
        Ogre::Overlay* pOverlay = Ogre::OverlayManager::getSingleton().create("Overlay1");
        
        // Create a panel with RenderToTexture texture
        Ogre::OverlayContainer* m_pOverlayPanel = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","OverlayPanelName%d"));
        m_pOverlayPanel->setMetricsMode(Ogre::GMM_PIXELS);
        m_pOverlayPanel->setPosition(10, 10);
        m_pOverlayPanel->setDimensions(500, 300);
        // Give overlay a texture
        m_pOverlayPanel->setMaterialName(camMat_id); 
        pOverlay->add2D(m_pOverlayPanel);
        pOverlay->show();
*/
        overlay_ = Ogre::OverlayManager::getSingleton().create("MapOverlay");
        Ogre::OverlayContainer* m_pOverlayPanel = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","OverlayPanelName%d"));
        m_pOverlayPanel->setMetricsMode(Ogre::GMM_PIXELS);
        m_pOverlayPanel->setPosition(10, 10);
        m_pOverlayPanel->setDimensions(500, 300);
        // Give overlay a texture
        m_pOverlayPanel->setMaterialName(camMat_id); 
        overlay_->add2D(m_pOverlayPanel);
        //Not Showing the map as default
        this->isVisible_=false;
        overlay_->hide();

    }
    
    Ogre::MaterialPtr Map::createRenderCamera(Ogre::Camera * cam, std::string matName)
    {
        Ogre::TexturePtr rttTex = Ogre::TextureManager::getSingleton().createManual(matName+"_tex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 512, 512, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

        Ogre::RenderTexture *renderTexture = rttTex->getBuffer()->getRenderTarget();

        renderTexture->addViewport(cam);
        renderTexture->getViewport(0)->setClearEveryFrame(true);
        renderTexture->getViewport(0)->setBackgroundColour(ColourValue::Black);
        renderTexture->getViewport(0)->setOverlaysEnabled(false);

        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(matName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Ogre::Technique *technique = material->createTechnique();
        technique->createPass();
        material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
        material->getTechnique(0)->getPass(0)->createTextureUnitState(matName+"_tex");
        return material;
    }

    void Map::updatePositions()
    {
//Ogre::Entity * ent;// = mapSceneM_->createEntity("ent1", "drone.mesh");
       int i=0; 
       for(ObjectList<orxonox::RadarViewable>::iterator it = ObjectList<orxonox::RadarViewable>::begin();
            it!=ObjectList<orxonox::RadarViewable>::end();
            it++)
        {
            i++;
            //COUT(0) << "Radar_Position: " << it->getRVWorldPosition() << std::endl;
            //Ogre::SceneNode node = it->getMapNode();
            //Ogre::Entity ent = it->getMapEntity();
            if( !(it->MapNode_) )
            {
                it->MapNode_ = rootNode_->createChildSceneNode( it->getRVWorldPosition() );
                //it->MapNode_->translate( it->getRVOrientedVelocity(), Ogre::TS_WORLD );
            }
            it->MapNode_->setPosition( it->getRVWorldPosition() );
            it->MapNode_->translate( it->getRVOrientedVelocity(), (Ogre::Node::TransformSpace)3 );
            if( !(it->MapEntity_) )
            {
                it->MapEntity_ = mapSceneM_->createEntity( "MapEnt"+i, "drone.mesh");
                it->MapNode_->attachObject( it->MapEntity_ );
            }
            
        
            
        }
    }

    void Map::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(Map, XMLPort, xmlElement, mode);
    }	

    void Map::changedOwner()
    {
        //COUT(0) << "shipptr" << this->getOwner()->getReverseCamera() << std::endl;
        if(this->getOwner()->getReverseCamera())
        {
            //COUT(0) << "foo";
            this->getOwner()->getReverseCamera()->attachCamera(this->mReflectCam_);
        }
    }


    void Map::toggleVisibility()
    {
        if (!(this->isVisible_))
        {
            this->overlay_->show();
            this->isVisible_=1;
            //updatePositions();
        }
        else
        {
            this->overlay_->hide();
            this->isVisible_=0;
        }
    }
    
    //Static function to toggle visibility of the map
    void Map::openMap()
    {
        for(ObjectList<orxonox::Map>::iterator it = ObjectList<orxonox::Map>::begin();
            it!=ObjectList<orxonox::Map>::end();
            it++)
        {
        //Map * m = it->getMap();
        COUT(0) << it->isVisible_ << std::endl;
        it->toggleVisibility();
        //it->updatePositions();
        }
    }
    
    void Map::tick(float dt)
    {
        //sNode_->lookAt(Vector3::NEGATIVE_UNIT_Z, Ogre::Node::TS_WORLD, Vector3::NEGATIVE_UNIT_Z);
        updatePositions();
        
    }
    
 }
