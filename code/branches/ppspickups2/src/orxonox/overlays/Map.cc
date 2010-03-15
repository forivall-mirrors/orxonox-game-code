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

#include "Map.h"

#include <string>

#include <OgreBorderPanelOverlayElement.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreMaterialManager.h>
#include <OgreMovablePlane.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>
#include <OgrePass.h>
#include <OgreRenderTexture.h>
#include <OgreResourceGroupManager.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreTechnique.h>
#include <OgreTexture.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>

#include "util/StringUtils.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "interfaces/RadarViewable.h"
#include "Scene.h"
#include "controllers/HumanController.h"
#include "worldentities/CameraPosition.h"
#include "worldentities/ControllableEntity.h"

 namespace orxonox
 {
    CreateFactory(Map);
    SetConsoleCommand(Map, openMap, true);
    //SetConsoleCommand(Map, rotateYaw, true).setAsInputCommand();
    //SetConsoleCommand(Map, rotatePitch, true).setAsInputCommand();
    SetConsoleCommand(Map, Zoom, true).setAsInputCommand();


    Map* Map::singletonMap_s = 0;
    Ogre::SceneManager* Map::mapSceneM_s = 0;
    Ogre::Camera* Map::Cam_ = 0;
    Ogre::SceneNode* Map::CamNode_ = 0;
    Ogre::MaterialPtr Map::OverlayMaterial_;// = init();
    Ogre::Overlay* Map::overlay_ = 0;
/*
Ogre::MaterialPtr Map::init()
{
    Ogre::MaterialPtr tmp;
    tmp.setNull();
    return tmp;
}
*/

    //int Map::mouseLookSpeed_ = 200;
    //Ogre::SceneNode* Map::playerShipNode_ = 0;

    const int PITCH=-30;
    const int DISTANCE=200;

    Map::Map(BaseObject* creator) : OrxonoxOverlay(creator)
    {
        RegisterObject(Map);
        Map::singletonMap_s=this;

        //Getting Scene Manager (Hack)
        ObjectList<Scene>::iterator it = ObjectList<Scene>::begin();
        this->sManager_ = it->getSceneManager();
        if( !Map::getMapSceneManager() )
        {
            Map::setMapSceneManager( Ogre::Root::getSingletonPtr()->createSceneManager( Ogre::ST_GENERIC,"MapScene" ) );
        }

        this->playerShipNode_ = 0;
        //this->sNode_ = new Ogre::SceneNode(sManager_);
        //oManager_ = Ogre::OverlayManager::getSingletonPtr();
        //overlay_ = oManager_->create("Map");
        //overlay_ is member of OrxonoxOverlay

        //Not Showing the map as default
        //this->isVisible_=false;
        //overlay_->hide();
        this->mouseLookSpeed_ = 200;

        //TestEntity
        //Ogre::Entity * ent = mapSceneM_s->createEntity("ent", "drone.mesh");

        //Map::getMapSceneManager()->getRootSceneNode()->attachObject( ent );
        /*sNode_->setPosition(0,0,-50);
        overlay_->add3D(sNode_);
        */





        // Alter the camera aspect ratio to match the viewport
        //mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
        if(!Map::Cam_)
            Cam_ = Map::getMapSceneManager()->createCamera("ReflectCam");
        //Cam_->setPosition(200,170, -160);
        //Cam_->lookAt(0,0,0);
        Cam_->setAspectRatio(1);
        //Cam_->setRenderingDistance(0);
        if(!Map::CamNode_)
            CamNode_ = Map::getMapSceneManager()->getRootSceneNode()->createChildSceneNode();


        //Create overlay material
        if(Map::OverlayMaterial_.isNull())
            Map::OverlayMaterial_ = this->createRenderCamera(Cam_, "RttMat");
/*
        Ogre::TexturePtr rttTex = Ogre::TextureManager::getSingleton().createManual("RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 512, 512, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

        Ogre::RenderTexture *renderTexture = rttTex->getBuffer()->getRenderTarget();

        renderTexture->addViewport(Cam_);
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
        if(!this->overlay_)
        {
            this->overlay_ = Ogre::OverlayManager::getSingletonPtr()->create("MapOverlay");
            Ogre::OverlayContainer* m_pOverlayPanel = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","OverlayPanelName%d"));
            //m_pOverlayPanel->setMetricsMode(Ogre::GMM_PIXELS);
            //m_pOverlayPanel->setPosition(10, 10);
            //m_pOverlayPanel->setDimensions(600, 400);
            m_pOverlayPanel->setPosition(0.01f, 0.003f);
            m_pOverlayPanel->setDimensions(0.5f, 0.4f);
            // Give overlay a texture
            m_pOverlayPanel->setMaterialName("RttMat");
            overlay_->add2D(m_pOverlayPanel);

            //Add Borders
            Ogre::BorderPanelOverlayElement* oBorder = static_cast<Ogre::BorderPanelOverlayElement*>(Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("BorderPanel", "MapBorderPanel" + getUniqueNumberString()));
            oBorder->setBorderSize( 0.003f, 0.003f );
            oBorder->setDimensions(0.5f, 0.4f);
            oBorder->setBorderMaterialName("StatsBorder");
            oBorder->setTopBorderUV(0.49f, 0.0f, 0.51f, 0.5f);
            oBorder->setTopLeftBorderUV(0.0f, 0.0f, 0.5f, 0.5f);
            oBorder->setTopRightBorderUV(0.5f, 0.0f, 1.0f, 0.5f);
            oBorder->setLeftBorderUV(0.0f, 0.49f, 0.5f, 0.51f);
            oBorder->setRightBorderUV(0.5f, 0.49f, 1.0f, 0.5f);
            oBorder->setBottomBorderUV(0.49f, 0.5f, 0.51f, 1.0f);
            oBorder->setBottomLeftBorderUV(0.0f, 0.5f, 0.5f, 1.0f);
            oBorder->setBottomRightBorderUV(0.5f, 0.5f, 1.0f, 1.0f);
            //overlay_->add2D(oBorder);
            m_pOverlayPanel->addChild(oBorder);
        }


        //Not Showing the map as default
        this->isVisible_=false;
        overlay_->hide();

        //Create plane to show gridTypeError: blimport() takes no keyword arguments
/*        Ogre::Entity* plane_ent;
        if(Map::getMapSceneManager()->hasEntity("MapPlane"))
            plane_ent = Map::getMapSceneManager()->getEntity("MapPlane");
        else
            plane_ent = Map::getMapSceneManager()->createEntity( "MapPlane", "plane.mesh");
*/
        this->movablePlane_ = new Ogre::MovablePlane( Vector3::UNIT_Y, 0 );
        this->movablePlane_->normalise();

        if(!Map::getMapSceneManager()->hasEntity("MapPlane"))
        {
            Ogre::Entity* plane_ent = Map::getMapSceneManager()->createEntity( "MapPlane", "plane.mesh");
            planeNode_ = Map::getMapSceneManager()->createSceneNode();
        //Create plane for calculations


        //Ogre::MaterialPtr plane_mat = Ogre::MaterialManager::getSingleton().create("mapgrid", "General");
        //plane_mat->getTechnique(0)->getPass(0)->createTextureUnitState("mapgrid.tga");
        //plane_ent->setMaterialName("mapgrid");
            plane_ent->setMaterialName("Map/Grid");
            planeNode_->attachObject(plane_ent);

            planeNode_->scale(160,1,160);
//        planeNode_->attachObject(movablePlane_);
        //Ogre::Material plane_mat = Ogre::MaterialManager::getSingletonPtr()->getByName("rock");


        //ToDo create material script
            Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("Map/Line","General");
            myManualObjectMaterial->setReceiveShadows(false);
            myManualObjectMaterial->getTechnique(0)->setLightingEnabled(true);
            myManualObjectMaterial->getTechnique(0)->getPass(0)->setDiffuse(1,1,0,0);
            myManualObjectMaterial->getTechnique(0)->getPass(0)->setAmbient(1,1,0);
            myManualObjectMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(1,1,0);
        }
    }

    Map::~Map()
    {
        this->singletonMap_s = 0;
        //delete this->overlay_;
        /*if (this->isInitialized())
        {
        //delete sManager_;
        //delete Map::getMapSceneManager()->getRootSceneNode();
        //delete oManager_;
        //delete CamNode_;
        //delete Cam_;
        //delete mapSceneM_s;
        //Map::getMapSceneManager()->destroyAllEntities();
        //Map::getMapSceneManager()->destroyAllCameras();
        delete Map::getMapSceneManager();
        }*/
    }

    Ogre::MaterialPtr Map::createRenderCamera(Ogre::Camera * cam, const std::string& matName)
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

//Ogre::Entity * ent;// = mapSceneM_s->createEntity("ent1", "drone.mesh");
       for(ObjectList<orxonox::RadarViewable>::iterator it = ObjectList<orxonox::RadarViewable>::begin();
            it!=ObjectList<orxonox::RadarViewable>::end();
            ++it)
        {
            //COUT(0) << "Radar_Position: " << it->getRVWorldPosition() << std::endl;
            //Ogre::SceneNode node = it->getMapNode();
            //Ogre::Entity ent = it->getMapEntity();
            if( !(it->MapNode_) )
            {
                it->MapNode_ = Map::getMapSceneManager()->getRootSceneNode()->createChildSceneNode( it->getRVWorldPosition() );
                //it->MapNode_->translate( it->getRVOrientedVelocity(), Ogre::TS_WORLD );
                /*if(it->getRadarObjectShape() == RadarViewable::Dot)
                {
                    //if( !(it->MapEntity_) )//check wether the entity is already attached
                    //{
                        //it->MapEntity_ = Map::getMapSceneManager()->createEntity( getUniqueNumberString(), "drone.mesh");
                        //it->addEntity();
                        //it->MapNode_->attachObject( it->MapEntity_ );
                        //it->MapNode_->attachObject( it->line_ );
                   // }
                }*/
                it->addMapEntity();
            }
            if(it->isHumanShip_)
            {
                this->movablePlane_->redefine(it->MapNode_->getLocalAxes().GetColumn(1) , it->MapNode_->getPosition());
                if(it->isHumanShip_ && it->MapNode_ != this->playerShipNode_)
                {
                    this->playerShipNode_ = it->MapNode_;
                    if(planeNode_ && this->planeNode_->getParent())
                        this->planeNode_->getParent()->removeChild(this->planeNode_);
                    this->playerShipNode_->addChild(this->planeNode_);
                //Movable Plane needs to be attached direcly for calculations
                //this->movablePlane_->detatchFromParent();
                //this->movablePlane_->getParentSceneNode()->detachObject(this->movablePlane_);
                //this->movablePlane_->redefine(it->MapNode_->getLocalAxes().GetColumn(1) , it->MapNode_->getPosition());
                //it->MapNode_->attachObject(this->movablePlane_);
                    if(planeNode_ && this->CamNode_->getParent())
                        this->CamNode_->getParent()->removeChild(this->CamNode_);
                    this->playerShipNode_->addChild(this->CamNode_);
                    this->CamNode_->attachObject(this->Cam_);
                //this->CamNodeHelper_ = this->CamNode_->createChildSceneNode();
                //this->CamNodeHelper_->attachObject(this->Cam_);
                    this->Cam_->setPosition(0, 0, (float)DISTANCE);
                    this->Cam_->pitch( static_cast<Degree>((float)PITCH) );
                    this->Cam_->lookAt(this->playerShipNode_->getPosition());
                //this->Cam_->setAutoTracking(true, this->playerShipNode_);
                }
            }
            it->updateMapPosition();






        }
    }



    void Map::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(Map, XMLPort, xmlElement, mode);
    }

    void Map::changedOwner()
    {
        SUPER(Map, changedOwner);
        //COUT(0) << "shipptr" << this->getOwner()->getReverseCamera() << std::endl;

        ControllableEntity* entity = orxonox_cast<ControllableEntity*>(this->getOwner());
        if(entity && entity->getReverseCamera())
        {
            //COUT(0) << "foo";
            entity->getReverseCamera()->attachCamera(this->Cam_);
        }
    }


    void Map::toggleVisibility()
    {
        if (!(this->isVisible_))
        {
            this->overlay_->show();
            this->isVisible_=1;
            //set mouselook when showing map
            if (HumanController::localController_s && HumanController::localController_s->controllableEntity_ && !HumanController::localController_s->controllableEntity_->isInMouseLook())
            HumanController::localController_s->controllableEntity_->mouseLook();
        }
        else
        {
            this->overlay_->hide();
            this->isVisible_=0;
            if (HumanController::localController_s && HumanController::localController_s->controllableEntity_ && HumanController::localController_s->controllableEntity_->isInMouseLook())
            HumanController::localController_s->controllableEntity_->mouseLook();
        }
    }

    //Static function to toggle visibility of the map
    void Map::openMap()
    {
        for(ObjectList<orxonox::Map>::iterator it = ObjectList<orxonox::Map>::begin();
            it!=ObjectList<orxonox::Map>::end();
            ++it)
        {
        //Map * m = it->getMap();
        //COUT(0) << it->isVisible_ << std::endl;
        it->toggleVisibility();
        //it->updatePositions();
        }
    }

    // HACK!
    void Map::hackDestroyMap()
    {
        Map::OverlayMaterial_.setNull();
    }

    void Map::tick(float dt)
    {
        //Debug
        //COUT(0) << "MovablePlane Position: " << this->movablePlane_->getParentSceneNode()->getName() << this->movablePlane_->getParentSceneNode()->getPosition() << std::endl;
        //COUT(0) << "planeNode_ Position: " << this->planeNode_ ->getName() << this->planeNode_->getPosition() << std::endl;
        //COUT(0) <<  "planeNode_ Parrent Position" << this->planeNode_->getParent()->getName() << this->planeNode_->getParent()->getPosition() << std::endl;
        if( this->isVisible_ )
            updatePositions();
        //Cam_->roll(Degree(1));

    }

    void Map::rotateYaw(const Vector2& value)
    {
        if(!( Map::singletonMap_s && Map::singletonMap_s->CamNode_ ))
            return;

/*
        singletonMap_s->CamNode_->setOrientation(singletonMap_s->CamNode_->getOrientation() * Quaternion( static_cast<Degree>(-value.y * singletonMap_s->mouseLookSpeed_) , singletonMap_s->playerShipNode_->getLocalAxes().GetColumn(1) ));

        Map::singletonMap_s->CamNodeHelper_->setDirection(Vector3::UNIT_Y, Ogre::Node::TS_PARENT, Vector3::UNIT_Y);
        Map::singletonMap_s->CamNodeHelper_->lookAt(Vector3(0,0,0), Ogre::Node::TS_PARENT);
*/
        singletonMap_s->CamNode_->yaw( static_cast<Degree>(-value.y * singletonMap_s->mouseLookSpeed_), Ogre::Node::TS_PARENT);
    }

    void Map::rotatePitch(const Vector2& value)
    {
        if(!( Map::singletonMap_s && Map::singletonMap_s->CamNode_ ))
            return;
            //singletonMap_s->Cam_->setOrientation(singletonMap_s->Cam_->getOrientation() * Quaternion( static_cast<Degree>(-value.y * singletonMap_s->mouseLookSpeed_) , Vector3::UNIT_X));
/*        singletonMap_s->CamNode_->setOrientation(singletonMap_s->CamNode_->getOrientation() * Quaternion( static_cast<Degree>(-value.y * singletonMap_s->mouseLookSpeed_) , singletonMap_s->playerShipNode_->getLocalAxes().GetColumn(0) ));

        Map::singletonMap_s->CamNodeHelper_->setDirection(Vector3::UNIT_Y, Ogre::Node::TS_PARENT, Vector3::UNIT_Y);
        Map::singletonMap_s->CamNodeHelper_->lookAt(Vector3(0,0,0), Ogre::Node::TS_PARENT);
*/
        singletonMap_s->CamNode_->pitch( static_cast<Degree>(value.y * singletonMap_s->mouseLookSpeed_), Ogre::Node::TS_LOCAL);

    }

    void Map::Zoom(const Vector2& value)
    {
        if(!( Map::singletonMap_s && Map::singletonMap_s->CamNode_ ))
            return;
        //COUT(0) << value.y << std::endl;
        Map::singletonMap_s->Cam_->setPosition(0,0, Map::singletonMap_s->Cam_->getPosition().z + value.y * Map::singletonMap_s->mouseLookSpeed_ );
    }
 }
