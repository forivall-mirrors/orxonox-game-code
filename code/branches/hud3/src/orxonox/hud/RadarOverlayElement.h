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
#include <util/Math.h>
#include <string.h>
#include "core/Tickable.h"
#include <OgrePrerequisites.h>
#include "objects/SpaceShip.h"
#include "../OrxonoxPrereqs.h"

namespace orxonox
{

  class _OrxonoxExport RadarOverlayElement : public Ogre::PanelOverlayElement
  {
  private:

    Ogre::PanelOverlayElement* point;
    Ogre::OverlayContainer* container_;
    Ogre::OverlayManager* om;       //pointer to the one and only overlay manager
    Vector3 initialDir_;	        //initial direction of ship
    Vector3 currentDir_;            //current direction of ship
    Vector3 initialOrth_;
    Vector3 currentOrth_;
    Vector3 targetPos_;             //position of target
    Vector3 shipPos_;               //position of ship
    Vector3 vec_;                   //vector product dir X ort
    double radius_;                  //defines the radius in the radar
    double phi_;                   //defines the angle in the radar
    float left_, top_;
    bool right_;                    //checks whether the object is on the right side (since cos is not bijective)
    int count_;
    int dim_;

  public:

    RadarOverlayElement(const Ogre::String& name);
    virtual ~RadarOverlayElement();
    virtual void initialise();
    void update();
    void initRadarOverlayElement(Real left, Real top, int dim, Ogre::OverlayContainer* container);
    void setMainShipPosition(int dirX, int dirY, int dirZ, int ortX, int ortY, int ortZ);
    int newShip(int X, int Y, int Z);

    void resetShip(int shipID, int Y, int Z);
  };
}

#endif
