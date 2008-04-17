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

#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include <OgrePrerequisites.h>
#include "../OrxonoxPrereqs.h"



namespace orxonox
{
  class _OrxonoxExport BarOverlayElement : public Ogre::PanelOverlayElement
  {
  private:
    int percentage_;
    int dir_;	
    int left_;
    int top_;
    int width_;
    int height_;
	
    static Ogre::String& typeName_s;
	
  public:

    static const int LEFT = 0;
    static const int UP = 1;
    static const int RIGHT = 2;
    static const int DOWN = 3;

    static const int RED = 0;
    static const int YELLOW = 1;
    static const int GREEN = 2;

    BarOverlayElement(const Ogre::String& name);
    virtual ~BarOverlayElement();
    virtual void initialise();

    void initBarOverlayElement(Real left, Real top, Real width, Real height,
          int dir,  int colour);

    void reset(int percentage);
    void setColour(int colour);

  };
  
  
  class _OrxonoxExport SmartBarOverlayElement : public BarOverlayElement
  {
    private:

    public:
      
      SmartBarOverlayElement(const Ogre::String& name);
      virtual ~SmartBarOverlayElement(void);
      virtual void initialise();
      
      void initSmartBarOverlayElement(Ogre::Real left, Ogre::Real top, Ogre::Real width, Ogre::Real height, int dir);
      void reset(int percentage);
  };

  
}

#endif


