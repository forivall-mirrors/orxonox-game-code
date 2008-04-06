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

#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreStringConverter.h>
#include <string.h>

#include "Bar.h"

namespace orxonox
{
  using namespace Ogre;
    
  Bar::Bar(Real left, Real top, Real width, Real height,
          int dir,  int colour, std::string name){
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    element = overlayManager.createOverlayElement("Panel",name);
    element->setMetricsMode(Ogre::GMM_PIXELS);
    dir_ = dir;
    left_ = left;
    top_ = top;
    width_ = width;
    height_ = height;
    element->setPosition(left_,top_);
    element->setDimensions(width_,height_);
    setColour(colour);
  }
  
  Bar::~Bar(void){}
  
  
  void Bar::reset(int percentage){
    switch(dir_){
      case 1:
	element->setPosition(left_,top_);
	element->setDimensions(width_,height_*percentage/100);
	break;
      case 2:
	element->setPosition(left_+width_-width_*percentage/100,top_);
	element->setDimensions(width_*percentage/100,height_);
	break;
      case 3:
	element->setPosition(left_,top_+height_-height_*percentage/100);
	element->setDimensions(width_,height_*percentage/100);
	break;
      default:
	element->setPosition(left_,top_);
	element->setDimensions(width_*percentage/100,height_);	
    }
  }

  void Bar::setColour(int colour){
    switch(colour){
     case 0:
	element->setMaterialName("Orxonox/Red");
	break;
     case 1:
	element->setMaterialName("Orxonox/Yellow");
	break;
     case 2:
	element->setMaterialName("Orxonox/Green");
    }
  }

  void Bar::show(){element->show();}

  void Bar::hide(){element->hide();}

  SmartBar::SmartBar(Ogre::Real left, Ogre::Real top, Ogre::Real width, Ogre::Real height,
        int dir, std::string name) : Bar(left, top, width, height, dir, Bar::YELLOW, name){
  }

  SmartBar::~SmartBar(void){}


  void SmartBar::reset(int percentage){
    if (percentage>50) {setColour(Bar::GREEN);}
    else if (percentage>25) {setColour(Bar::YELLOW);}
    else setColour(Bar::RED);
    Bar::reset(percentage);
  }

}


