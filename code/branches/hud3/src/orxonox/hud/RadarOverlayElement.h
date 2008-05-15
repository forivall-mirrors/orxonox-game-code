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
#include <OgrePanelOverlayElement.h>
#include <math.h>
#include <util/Math.h>
#include <string.h>
#include "core/Tickable.h"
#include <OgrePrerequisites.h>
#include "../OrxonoxPrereqs.h"

namespace orxonox
{

  class _OrxonoxExport RadarOverlayElement : public Ogre::PanelOverlayElement, public Tickable
  {
  private:
    
    PanelOverlayElement* point[100];
    
    Vector3 initialDir;		//initial direction
    int dirX_, dirY_, dirZ_;    //flying direction
    int ortX_, ortY_, ortZ_;    //orthogonal direction
    int dX_, dY_, dZ_;          //distance between main ship and the object
    int vecX_, vecY_, vecZ_;    //vector product dir X ort
    double alpha_;              //defines the radius in the radar
    double beta_;               //defines the angle in the radar
    bool right_;                //checks whether the object is on the right side (since cos is not bijective)
    
//    static int ID = 0;

  public:
    
    RadarOverlayElement(const Ogre::String& name);
    virtual ~RadarOverlayElement();
    virtual void initialise();

    void initRadarOverlayElement(Real left, Real top, int dim, Ogre::OverlayContainer* container);
    void setMainShipPosition(int dirX, int dirY, int dirZ, int ortX, int ortY, int ortZ);
    void tick(float dt); 
    int newShip(int X, int Y, int Z);
    
    void resetShip(int shipID, int Y, int Z);
  };
}	

#endif
