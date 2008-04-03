/*
*   ORXONOX - the hottest 3D action shooter ever to exist
*
*
*   License notice:asdf
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
#include <OgreStringConverter.h>
#include <OgreColourValue.h>
#include <string.h>

#include "Bar.h"

namespace orxonox
{
  using namespace Ogre;

  Bar::Bar(void){}

  Bar::~Bar(void){}


/*  void Bar::setPercentage(Ogre::Real percentage){
    percentage_=percentage;
    if(horz_){setWidth(int(percentage_* maxLength_));}
	else {setHeight(int(percentage_ * maxHeight_));}
    }
*/

  void Bar::setPercentage(Ogre::Real percentage){
    percentage_ = percentage;
//    setWidth(getWidth());
  }




  void Bar::setColor(ColourValue color){
    color_=color;
    setColour(color);
  }


}
