/*
*   ORXONOX - the hottest 3D action shooter ever to exist
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
*      Yuning Chai
*   Co-authors:
*      ...
*
*/


#include "OrxonoxStableHeaders.h"
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>ma
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include "HUD.h"
#include "BarOverlayElement.h"
#include "OverlayElementFactories.h"
// ugly hack
#include "Orxonox.h"


namespace orxonox
{
  using namespace Ogre;

  HUD::HUD(int zoom){

    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    
    BarOverlayElementFactory *barOverlayElementFactory = new BarOverlayElementFactory();
    overlayManager.addOverlayElementFactory(barOverlayElementFactory);
    
    SmartBarOverlayElementFactory *smartBarOverlayElementFactory = new SmartBarOverlayElementFactory();
    overlayManager.addOverlayElementFactory(smartBarOverlayElementFactory);
    
    Ogre::Overlay* orxonoxOverlay = overlayManager.create("Orxonox/HUD"); 

    Ogre::OverlayContainer* energyCounterPanel = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "Orxonox/HUD/energyCounterPanel"));
    
    energyCounter = static_cast<SmartBarOverlayElement*>(overlayManager.createOverlayElement("SmartBar", "energyCounter"));
    energyCounter->show();
    
    energyCounterPanel->addChild(energyCounter);
    energyCounterPanel->show();

    orxonoxOverlay->add2D(energyCounterPanel);
    orxonoxOverlay->show();

    // important: don't use SceneManager to create the node! but register the creator scene manager.
/*    ogreNode_ = new Ogre::SceneNode(Orxonox::getSingleton()->getSceneManager(), "hudNoedely");
    
    ogreNode_->setPosition(80,-60,-200);
    ogreNode_->setScale(0.4,0.4,0.4);
    // ugly hack, but I haven't figured out yet how we could change this, since we really need the
    // scene manager..
    ogreNode_->attachObject(Orxonox::getSingleton()->getSceneManager()->createEntity("head", "ogrehead.mesh"));
//    orxonoxOverlay->add3D(ogreNode_);
*/
   
    energyCounterPanel->setLeft(-50);
    energyCounterPanel->setTop(10);
    energyCounterPanel->setWidth(100);
    energyCounterPanel->setHeight(20);
    energyCounterPanel->setHorizontalAlignment(Ogre::GHA_CENTER);
    energyCounterPanel->setMetricsMode(Ogre::GMM_PIXELS);	
        
    energyCounter->initSmartBarOverlayElement(-100,0,200,20,BarOverlayElement::LEFT);
    energyCounter->reset(80);

    
  }

/*  void HUD::tick(float dt)
  {
    if (this->ogreNode_)
    {
      this->ogreNode_->roll(Ogre::Degree(dt*200),Ogre::Node::TS_LOCAL);
      this->ogreNode_->yaw(Ogre::Degree(dt*200),Ogre::Node::TS_LOCAL); 
      this->ogreNode_->pitch(Ogre::Degree(dt*200),Ogre::Node::TS_LOCAL);
    }
  }
*/
  HUD::~HUD(void){}



}






