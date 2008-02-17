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


#ifndef _BAR_H__
#define _BAR_H__

#include <string>
#include <OgreColourValue.h>
#include <OgreOverlayElement.h>
#include <OgrePrerequisites.h>
#include <string.h>
#include "../OrxonoxPrereqs.h"



namespace hud
{
  class Bar : public Ogre::OverlayElement
  {
  private:
    float percentage_;
    bool horz_;
    Ogre::ColourValue color_;


  public:
    Bar(void);
    ~Bar(void);
    void setPercentage(Ogre::Real percentage);
    void setColor(Ogre::ColourValue color);
  };
}

#endif


