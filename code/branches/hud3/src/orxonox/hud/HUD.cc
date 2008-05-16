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
#include <OgreOverlayManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include "core/Debug.h"

#include "HUD.h"
#include "BarOverlayElement.h"
#include "RadarOverlayElement.h"
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

    RadarOverlayElementFactory *radarOverlayElementFactory = new RadarOverlayElementFactory();
    overlayManager.addOverlayElementFactory(radarOverlayElementFactory);

    Ogre::Overlay* orxonoxOverlay = overlayManager.create("Orxonox/HUD");

    Ogre::OverlayContainer* energyCounterPanel = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "Orxonox/HUD/energyCounterPanel"));

    energyCounter = static_cast<SmartBarOverlayElement*>(overlayManager.createOverlayElement("SmartBar", "energyCounter"));
    //energyCounter->show();


    radar = static_cast<RadarOverlayElement*>(overlayManager.createOverlayElement("Radar", "radar"));
    radar->show();


/*    TextAreaOverlayElement* test = static_cast<TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "test"));

    int dirX, dirY, dirZ;      //flying direction
    int ortX, ortY, ortZ;      //orthogonal direction
    int dX, dY, dZ;            //distance between main ship and the object
    int vecX, vecY, vecZ;      //vector product dir X ort
    double alpha;              //defines the radius in the radar
    double beta;               //defines the angle in the radar
    bool right;                //checks whether the object is on the right side (since cos is not bijective)

    dirX = 1;
    dirY = 0;
    dirZ = 0;

    ortX = 0;
    ortY = 0;
    ortZ = 1;

    dX = -2;
    dY = 2;
    dZ = 0;

    alpha = acos((dirX*dX+dirY*dY+dirZ*dZ)/(sqrt(pow(dX,2)+pow(dY,2)+pow(dZ,2))*sqrt(pow(dirX,2)+pow(dirY,2)+pow(dirZ,2))));
    beta = acos((ortX*dX+ortY*dY+ortZ*dZ)/(sqrt(pow(dX,2)+pow(dY,2)+pow(dZ,2))*sqrt(pow(ortX,2)+pow(ortY,2)+pow(ortZ,2))));
    vecX = dirY*ortZ - dirZ*ortY;
    vecY = dirZ*ortX - dirX*ortZ;
    vecZ = dirX*ortY - dirY*ortX;

    if((vecX*dX+vecY*dY+vecZ*dZ)>0){right=true;}
    else right=false;

    test->setCaption("hell");
    test->setPosition(10,10);
    test->setDimensions(20,20);
    test->show();
    test->setMetricsMode(Ogre::GMM_PIXELS);
    energyCounterPanel->addChild(test);

    COUT(0)<<alpha<<" "<<beta<<" "<<right<<std::endl;
*/

    energyCounterPanel->addChild(energyCounter);
    energyCounterPanel->addChild(radar);
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

    energyCounterPanel->setLeft(0.0);
    energyCounterPanel->setTop(0.0);
    energyCounterPanel->setWidth(1.0);
    energyCounterPanel->setHeight(1.0);
    energyCounterPanel->setMetricsMode(Ogre::GMM_RELATIVE);

    energyCounter->initSmartBarOverlayElement(10,10,200,20,BarOverlayElement::LEFT);
    energyCounter->reset(80);

    radar->initRadarOverlayElement(0.5, 0.8, 0.2, energyCounterPanel);


  }

  void HUD::tick(float dt)
  {
      radar->update();
    /*if (this->ogreNode_)
    {
      this->ogreNode_->roll(Ogre::Degree(dt*200),Ogre::Node::TS_LOCAL);
      this->ogreNode_->yaw(Ogre::Degree(dt*200),Ogre::Node::TS_LOCAL);
      this->ogreNode_->pitch(Ogre::Degree(dt*200),Ogre::Node::TS_LOCAL);
    }*/
  }

  HUD::~HUD(void){}

}






