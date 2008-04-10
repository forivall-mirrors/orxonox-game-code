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
#include <OgreOverlayContainer.h>
#include <OgrePanelOverlayElement.h>
#include <OgreStringConverter.h>
#include <OgreColourValue.h>
#include <string.h>

#include "Bar.h"

namespace orxonox
{
  using namespace Ogre;

//    typeName_s = "Bar";

    Bar::Bar(const String& name):Ogre::PanelOverlayElement(name){
    }


    Bar::~Bar(){}

//    const Ogre::String& Bar::getTypeName() const { return typeName_s;}



    void Bar::initialise(){
	PanelOverlayElement::initialise();
	setDimensions(100,100);
	setPosition(10,10);
	setMaterialName("Orxonox/Green");
	setMetricsMode(Ogre::GMM_PIXELS);
    }




/*  Bar::Bar(const Ogre::String& s){}
  
  Bar::~Bar(void){}
  
  */
/*  void Bar::setPercentage(float percentage){
    percentage_=percentage;
    if(dir_){setWidth(int(percentage_* getWidth()));}
	else {setHeight(int(percentage_ * getHeight()));}
    }


  void Bar::setPercentage(float percentage){
    percentage_ = percentage;
//    setWidth(getWidth());
  }



  
  void Bar::setColor(ColourValue color){
    color_=color;
    setColour(color);
  }
*/	  
	
}


