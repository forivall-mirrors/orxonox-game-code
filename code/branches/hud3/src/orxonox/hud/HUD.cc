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
    om = &Ogre::OverlayManager::getSingleton();

    BarOverlayElementFactory *barOverlayElementFactory = new BarOverlayElementFactory();
    om->addOverlayElementFactory(barOverlayElementFactory);

    SmartBarOverlayElementFactory *smartBarOverlayElementFactory = new SmartBarOverlayElementFactory();
    om->addOverlayElementFactory(smartBarOverlayElementFactory);

    RadarOverlayElementFactory *radarOverlayElementFactory = new RadarOverlayElementFactory();
    om->addOverlayElementFactory(radarOverlayElementFactory);

    orxonoxHUD = om->create("Orxonox/HUD");

    container = static_cast<Ogre::OverlayContainer*>(om->createOverlayElement("Panel", "Orxonox/HUD/container"));

    energyCounter = static_cast<SmartBarOverlayElement*>(om->createOverlayElement("SmartBar", "energyCounter"));
    energyCounter->show();

    speedo = static_cast<SmartBarOverlayElement*>(om->createOverlayElement("SmartBar", "speedo"));
    speedo->show();

    radar = static_cast<RadarOverlayElement*>(om->createOverlayElement("Radar", "radar"));
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
    container->addChild(test);

    COUT(0)<<alpha<<" "<<beta<<" "<<right<<std::endl;
*/

    container->addChild(energyCounter);
    container->addChild(speedo);
    container->addChild(radar);
    container->show();

    orxonoxHUD->add2D(container);
    orxonoxHUD->show();



    // important: don't use SceneManager to create the node! but register the creator scene manager.
/*    ogreNode_ = new Ogre::SceneNode(Orxonox::getSingleton()->getSceneManager(), "hudNoedely");

    ogreNode_->setPosition(80,-60,-200);
    ogreNode_->setScale(0.4,0.4,0.4);
    // ugly hack, but I haven't figured out yet how we could change this, since we really need the
    // scene manager..
    ogreNode_->attachObject(Orxonox::getSingleton()->getSceneManager()->createEntity("head", "ogrehead.mesh"));
//    orxonoxOverlay->add3D(ogreNode_);
*/

    container->setLeft(0.0);
    container->setTop(0.0);
    container->setWidth(1.0);
    container->setHeight(1.0);
    container->setMetricsMode(Ogre::GMM_RELATIVE);

    energyCounter->initSmartBarOverlayElement(0.01, 0.01, 0.2, 0.02, BarOverlayElement::LEFT);
    energyCounter->reset(100);

    speedo->initSmartBarOverlayElement(0.01, 0.04, 0.2, 0.02, BarOverlayElement::LEFT);
    speedo->reset(80);

    radar->initRadarOverlayElement(0.5, 0.9, 0.2, container);


  }

  void HUD::tick(float dt)
  {
      float v = SpaceShip::instance_s->getVelocity().length();
      float vmax = SpaceShip::instance_s->getMaxSpeed();
      speedo->reset(100*v/vmax);
      COUT(3) << 100*v/vmax << std::endl;
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






