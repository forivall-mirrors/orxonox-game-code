/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
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

#ifndef _HUD_H__
#define _HUD_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include <OgrePrerequisites.h>


namespace orxonox
{
  class _OrxonoxExport HUD
  {
  private:
    Ogre::OverlayElement* timeText_;
    Ogre::OverlayElement* targetWindowNameText_;
    Ogre::OverlayElement* targetWindowStatusText_;
    Ogre::OverlayElement* targetWindowDistanceText_;
    Ogre::OverlayElement* targetWindowHitRatingText_;
    Ogre::OverlayElement* energyLength_;
    Ogre::OverlayElement* shieldLeftTop_;
    Ogre::OverlayElement* shieldRightTop_;
    Ogre::OverlayElement* shieldLeftBottom_;
    Ogre::OverlayElement* shieldRightBottom_;
    Ogre::OverlayElement* rocketNum1_;
    Ogre::OverlayElement* rocketNum2_;
    Ogre::OverlayElement* rocketNum3_;
    Ogre::OverlayElement* rocketNum4_;
    Ogre::OverlayElement* energyDistrPoint_;
    Ogre::OverlayElement* primarChoice_;

    int timeMin_;
    int timeSec_;

    std::string targetWindowName_;
    std::string targetWindowStatus_;
    int targetWindowDistance_;
    int targetWindowHitRating_;

    int energyValue_;

    bool shieldLeftTopValue_;
    bool shieldRightTopValue_;
    bool shieldLeftBottomValue_;
    bool shieldRightBottomValue_;

    int rocket1_;
    int rocket2_;
    int rocket3_;
    int rocket4_;

    float energyDistrShield_;
    float energyDistrEngine_;
    float energyDistrLaser_;

    float energyDistrPixelX_;
    float energyDistrPixelY_;

    int primarValue_;

  public:
    HUD( void );
    ~HUD( void );
    void init( void );

    void setTime(int i, int j);
    void setTargetWindowName(std::string i);
    void setTargetWindowStatus(std::string i);
    void setTargetWindowDistance(int i);
    void setTargetWindowHitRating(int i);
    void setEnergyValue(int i);
    void setShieldLeftTopValue(bool i);
    void setShieldRightTopValue(bool i);
    void setShieldLeftBottomValue(bool i);
    void setShieldRightBottomValue(bool i);
    void setRocket1(int i);
    void setRocket2(int i);
    void setRocket3(int i);
    void setRocket4(int i);
    void setEnergyDistr(float s, float e, float l);
    void setPrimar(int i);

  };
}

#endif /* _HUD_H__ */
