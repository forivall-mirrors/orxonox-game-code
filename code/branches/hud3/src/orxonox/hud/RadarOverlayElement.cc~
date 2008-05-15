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

/*	local coordinate system of space ship:

			y
			+   z
			|  +
			| /
			|/
	       x +------O
*/

#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include <OgreStringConverter.h>
#include "RadarOverlayElement.h"


namespace orxonox
{
  using namespace Ogre;

  RadarOverlayElement::RadarOverlayElement(const String& name):Ogre::PanelOverlayElement(name){
    initialDir = Vector3(0.0, 0.0, 1.0);
  }

  RadarOverlayElement::~RadarOverlayElement(){}

  void RadarOverlayElement::initialise(){
    PanelOverlayElement::initialise();
  }
  
  void RadarOverlayElement::initRadarOverlayElement(Real left, Real top, int dim, Ogre::OverlayContainer* container){

    dirX_ = 0;	// direction of travel
    dirY_ = 0;
    dirZ_ = 1;
    
    ortX_ = 0;	// normal of ship
    ortY_ = 1;
    ortZ_ = 0;
    
    dX_ = 37;
    dY_ = 0;
    dZ_ = 0;
    
    alpha_ = acos((dirX_*dX_+dirY_*dY_+dirZ_*dZ_)/(sqrt(pow(dX_,2)+pow(dY_,2)+pow(dZ_,2))*sqrt(pow(dirX_,2)+pow(dirY_,2)+pow(dirZ_,2))));
    beta_ = acos((ortX_*dX_+ortY_*dY_+ortZ_*dZ_)/(sqrt(pow(dX_,2)+pow(dY_,2)+pow(dZ_,2))*sqrt(pow(ortX_,2)+pow(ortY_,2)+pow(ortZ_,2))));
    vecX_ = dirY_*ortZ_ - dirZ_*ortY_;
    vecY_ = dirZ_*ortX_ - dirX_*ortZ_;
    vecZ_ = dirX_*ortY_ - dirY_*ortX_;
    
    if((vecX_*dX_+vecY_*dY_+vecZ_*dZ_)>0){right_=true;}
    else right_=false;
    
    setMetricsMode(Ogre::GMM_PIXELS);
    setPosition(left,top);
    setDimensions(dim,dim);
    setMaterialName("Orxonox/Radar");
    
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

    PanelOverlayElement* point = static_cast<PanelOverlayElement*>(overlayManager.createOverlayElement("Panel", "point"));
    point->show();
    
    container->addChild(point);
    
    if (right_){
      point->setPosition(sin(beta_)*alpha_/3.5*dim/2+dim/2+left-2,-cos(beta_)*alpha_/3.5*dim/2+dim/2+top-2);
      point->setMaterialName("Orxonox/RedPoint");
      point->setDimensions(5,5);
      point->setMetricsMode(Ogre::GMM_PIXELS);
    }
  
    else {
      point->setPosition(-sin(beta_)*alpha_/3.5*dim/2+dim/2+left-2,-cos(beta_)*alpha_/3.5*dim/2+dim/2+top-2);
      point->setMaterialName("Orxonox/RedPoint");
      point->setDimensions(5,5);
      point->setMetricsMode(Ogre::GMM_PIXELS);
    }
  } 
   
  void RadarOverlayElement::setMainShipPosition(int dirX, int dirY, int dirZ, int ortX, int ortY, int ortZ){
    dirX_=dirX;
    dirY_=dirY;
    dirZ_=dirZ;
    ortX_=ortX;
    ortY_=ortY;
    ortZ_=ortZ;
  }
  
  
  
}



