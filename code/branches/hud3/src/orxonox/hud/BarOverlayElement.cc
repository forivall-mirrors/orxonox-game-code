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


#include "BarOverlayElement.h"

namespace orxonox
{
  using namespace Ogre;


    BarOverlayElement::BarOverlayElement(const String& name):Ogre::PanelOverlayElement(name){}

    BarOverlayElement::~BarOverlayElement(){}

    void BarOverlayElement::initialise(){
	PanelOverlayElement::initialise();
/*	setDimensions(100,100);
	setPosition(10,10);
	setMaterialName("Orxonox/Green");
	setMetricsMode(Ogre::GMM_PIXELS);
*/  }


    void BarOverlayElement::initBarOverlayElement(Real left, Real top, Real width, Real height,
       int dir,  int colour){
	 setMetricsMode(Ogre::GMM_PIXELS);
	 dir_ = dir;
	 left_ = left;
	 top_ = top;
	 width_ = width;
	 height_ = height;
	 setPosition(left_,top_);
	 setDimensions(width_,height_);
	 setColour(colour);
    }


   void BarOverlayElement::reset(int percentage){
      switch(dir_){
        case 1:
	  setPosition(left_,top_);
	  setDimensions(width_,height_*percentage/100);
	  break;
	case 2:
	  setPosition(left_+width_-width_*percentage/100,top_);
	  setDimensions(width_*percentage/100,height_);
	  break;
	case 3:
	  setPosition(left_,top_+height_-height_*percentage/100);
	  setDimensions(width_,height_*percentage/100);
	  break;
	default:
	  setPosition(left_,top_);
	  setDimensions(width_*percentage/100,height_);	
      }
    }
       
       
    void BarOverlayElement::setColour(int colour){
      switch(colour){
	case 0:
	  setMaterialName("Orxonox/Red");
	  break;
	case 1:
	  setMaterialName("Orxonox/Yellow");
	  break;
	case 2:
	  setMaterialName("Orxonox/Green");
      }
    }


    SmartBarOverlayElement::SmartBarOverlayElement(const String& name):BarOverlayElement(name){}

    SmartBarOverlayElement::~SmartBarOverlayElement(void){}
    
    
    void SmartBarOverlayElement::initialise(){
      PanelOverlayElement::initialise();
/*	setDimensions(100,100);
      setPosition(10,10);
      setMaterialName("Orxonox/Green");
      setMetricsMode(Ogre::GMM_PIXELS);
*/  }
    
    void SmartBarOverlayElement::initSmartBarOverlayElement(Real left, Real top, Real width, Real height, int dir)
    {
      BarOverlayElement::initBarOverlayElement(left, top, width, height, dir, BarOverlayElement::GREEN);
    }


    void SmartBarOverlayElement::reset(int percentage){
      if (percentage>50) {setColour(BarOverlayElement::GREEN);}
      else if (percentage>25) {setColour(BarOverlayElement::YELLOW);}
      else setColour(BarOverlayElement::RED);
      BarOverlayElement::reset(percentage);
    }

}


