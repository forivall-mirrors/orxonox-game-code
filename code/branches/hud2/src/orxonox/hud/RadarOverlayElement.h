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


#ifndef _RADAR_H__
#define _RADAR_H__

#include <string.h>
#include <OgreOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePrerequisites.h>
#include "../OrxonoxPrereqs.h"

namespace orxonox
{

  class _OrxonoxExport RadarObject
  {
  private:
    int name_;
    Ogre::Real x_;
    Ogre::Real y_;
    Ogre::Real z_;
  public:
    RadarObject(int name, Ogre::Real x, Ogre::Real y, Ogre::Real z);
    ~RadarObject();
    void reset(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    inline Ogre::Real getX(){return x_;}
    inline Ogre::Real getY(){return y_;}
    inline Ogre::Real getZ(){return z_;}
    inline int getName(){return name_;}
  };


  class _OrxonoxExport Radar
  {
  private:
    int number;


  public:
    Radar();
    ~Radar();





  };
}	

#endif