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

#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include <OgreStringConverter.h>
#include <math.h>
#include <string.h>

#include "RadarOverlayElement.h"


namespace orxonox
{
  using namespace Ogre;

  RadarOverlayElement::RadarOverlayElement(const String& name):Ogre::PanelOverlayElement(name){}

  RadarOverlayElement::~RadarOverlayElement(){}

  void RadarOverlayElement::initialise(){
    PanelOverlayElement::initialise();
  }
  
  void RadarOverlayElement::initRadarOverlayElement(Real left, Real top, int dim, Ogre::OverlayContainer* container){
    
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
    
    dX = 0;
    dY = 2;
    dZ = 0;
    
    alpha = acos((dirX*dX+dirY*dY+dirZ*dZ)/(sqrt(pow(dX,2)+pow(dY,2)+pow(dZ,2))*sqrt(pow(dirX,2)+pow(dirY,2)+pow(dirZ,2))));
    beta = acos((ortX*dX+ortY*dY+ortZ*dZ)/(sqrt(pow(dX,2)+pow(dY,2)+pow(dZ,2))*sqrt(pow(ortX,2)+pow(ortY,2)+pow(ortZ,2))));
    vecX = dirY*ortZ - dirZ*ortY;
    vecY = dirZ*ortX - dirX*ortZ;
    vecZ = dirX*ortY - dirY*ortX;
    
    if((vecX*dX+vecY*dY+vecZ*dZ)>0){right=true;}
    else right=false;
    
    setMetricsMode(Ogre::GMM_PIXELS);
    setPosition(left,top);
    setDimensions(dim-15,dim-20);
    setMaterialName("Orxonox/Radar");
    
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

    PanelOverlayElement* point = static_cast<PanelOverlayElement*>(overlayManager.createOverlayElement("Panel", "point"));
    point->show();
    
    container->addChild(point);
    
    if (right){
      point->setPosition(sin(beta)*alpha/3.14*dim/2+dim/2,-cos(beta)*alpha/3.14*dim/2+dim/2);
      point->setMaterialName("Orxonox/RedPoint");
      point->setDimensions(5,5);
      point->setMetricsMode(Ogre::GMM_PIXELS);
    }
  
    else {
      point->setPosition(-sin(beta)*alpha/3.14*dim/2+dim/2,-cos(beta)*alpha/3.14*dim/2+dim/2);
      point->setMaterialName("Orxonox/RedPoint");
      point->setDimensions(5,5);
      point->setMetricsMode(Ogre::GMM_PIXELS);
    }

    

    
  }  
}



