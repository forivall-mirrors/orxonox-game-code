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

#include <string.h>
#include <OgreOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePrerequisites.h>
#include "../OrxonoxPrereqs.h"



namespace orxonox
{
  class _OrxonoxExport Bar
  {
  private:
    int percentage_;
    int dir_;	
    int left_;
    int top_;
    int width_;
    int height_;
	
  public:
    static const int LEFT = 0;
    static const int UP = 1;
    static const int RIGHT = 2;
    static const int DOWN = 3;

    static const int RED = 0;
    static const int YELLOW = 1;
    static const int GREEN = 2;

    Ogre::OverlayElement* element;

    Bar(Ogre::Real left, Ogre::Real top, Ogre::Real width, Ogre::Real height,
        int dir, int colour, std::string name);
    ~Bar(void);
    void reset(int percentage);
    void setColour(int colour);
    void show();
    void hide();
    
  };


  class _OrxonoxExport SmartBar : public Bar
  {
  private:

  public:
    SmartBar(Ogre::Real left, Ogre::Real top, Ogre::Real width, Ogre::Real height,
        int dir, std::string name);
    ~SmartBar(void);
    void reset(int percentage);
  };
}

#endif


