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
#include <OgrePanelOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgrePrerequisites.h>
#include <string.h>
#include "../OrxonoxPrereqs.h"



namespace orxonox
{
  class _OrxonoxExport Bar : public Ogre::PanelOverlayElement
  {
  private:
    float percentage_;
    bool dir_;
    Ogre::ColourValue color_;
	
    static Ogre::String& typeName_s;
	
  public:

    Bar(const Ogre::String& name);

    virtual void initialise();

    virtual ~Bar();
    
//    virtual const Ogre::String& getTypeName() const;
//    virtual ~Bar();




/*    Bar(const Ogre::String& s);
    ~Bar(void);
    void setPercentage(float percentage);
    void setColor(Ogre::ColourValue color);
*/  };
}

#endif


