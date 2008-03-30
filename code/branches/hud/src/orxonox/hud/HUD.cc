/*
*   ORXONOX - the hottest 3D action shooter ever to exist
*
*
*   License notice:
*
*   This program is free software; you can redistribute it and/or
*   modify it under the terms of the GNU General Public License
*   as published by the Free Software Foundation; either version 2
*   of the License, or (at your option)asdf any later version.
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

#include "OrxonoxStableHeaders.h"

#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreStringConverter.h>

#include "HUD.h"
#include "Bar.h"


namespace orxonox
{
  using namespace Ogre;

  HUD::HUD(void)
  {
    init();

    setTime(99,55);

    setTargetWindowName("HURRA");
    setTargetWindowDistance(12);
    setTargetWindowStatus("Dead");
    setTargetWindowDistance(30);

    setEnergyValue(60.0);

    setShieldLeftTopValue(true);
    setShieldRightTopValue(false);
    setShieldLeftBottomValue(false);
    setShieldRightBottomValue(true);

    setRocket1(11);
    setRocket2(22);
    setRocket3(33);
    setRocket4(44);

    setEnergyDistr(33,33,34);

    setPrimar(3);

    energyDistrPixelX_ = 100;
    energyDistrPixelY_ = 86;

  }


  HUD::~HUD( void ){

  }

  void HUD::setTime(int i, int j){
    timeMin_=i;
    timeSec_=j;
    timeText_->setCaption(StringConverter::toString(timeMin_) + ":" + StringConverter::toString(timeSec_));
  }

  void HUD::setTargetWindowName(std::string i){
    targetWindowName_=i;
    targetWindowNameText_->setCaption( targetWindowName_ );
  }

  void HUD::setTargetWindowStatus(std::string i){
    targetWindowStatus_=i;
    targetWindowStatusText_->setCaption( targetWindowStatus_ );
  }

  void HUD::setTargetWindowDistance(int i){
    targetWindowDistance_=i;
    targetWindowDistanceText_->setCaption(StringConverter::toString(targetWindowDistance_) + "km" );
  }

  void HUD::setTargetWindowHitRating(int i){
    targetWindowHitRating_=i;
    targetWindowHitRatingText_->setCaption(StringConverter::toString(targetWindowHitRating_) + "%" );
  }

  void HUD::setEnergyValue(int i){
    energyValue_=i;
    energyLength_->setWidth((int)((float)energyValue_/(float)100*200));
  }

  void HUD::setShieldLeftTopValue(bool i){
    shieldLeftTopValue_=i;
    if (shieldLeftTopValue_) shieldLeftTop_->show();
    else shieldLeftTop_->hide();
  }

  void HUD::setShieldRightTopValue(bool i){
    shieldRightTopValue_=i;
    if (shieldRightTopValue_) shieldRightTop_->show();
    else shieldRightTop_->hide();
  }

  void HUD::setShieldLeftBottomValue(bool i){
    shieldLeftBottomValue_=i;
    if (shieldLeftBottomValue_) shieldLeftBottom_->show();
    else shieldLeftBottom_->hide();
  }

  void HUD::setShieldRightBottomValue(bool i){
    shieldRightBottomValue_=i;
    if (shieldRightBottomValue_) shieldRightBottom_->show();
    else shieldRightBottom_->hide();
  }

  void HUD::setRocket1(int i){
    rocket1_=i;
    rocketNum1_->setCaption(StringConverter::toString(rocket1_));
  }

  void HUD::setRocket2(int i){
    rocket2_=i;
    rocketNum2_->setCaption(StringConverter::toString(rocket2_));
  }

  void HUD::setRocket3(int i){
    rocket3_=i;
    rocketNum3_->setCaption(StringConverter::toString(rocket3_));
  }

  void HUD::setRocket4(int i){
    rocket4_=i;
    rocketNum4_->setCaption(StringConverter::toString(rocket4_));
  }

  void HUD::setEnergyDistr(float s, float e, float l){
    energyDistrShield_=s;
    energyDistrEngine_=e;
    energyDistrLaser_=l;

    float energyDistrShieldInv_ = 100 - energyDistrShield_;
    float energyDistrEngineInv_ = 100 - energyDistrEngine_;
    float energyDistrLaserInv_ = 100 - energyDistrLaser_;

    float energyDistrT_ = -energyDistrPixelX_/((energyDistrPixelX_-energyDistrPixelX_/2*energyDistrShieldInv_/(energyDistrShieldInv_+energyDistrEngineInv_))*energyDistrLaserInv_*(energyDistrShieldInv_+energyDistrEngineInv_)/energyDistrShieldInv_/(energyDistrLaserInv_+energyDistrEngineInv_)+energyDistrPixelX_/2+energyDistrPixelX_/2*energyDistrEngineInv_/(energyDistrEngineInv_+energyDistrLaserInv_));
    float energyDistrX_ = (-energyDistrPixelX_/2-energyDistrPixelX_/2*energyDistrEngineInv_/(energyDistrEngineInv_+energyDistrLaserInv_))*energyDistrT_;
    float energyDistrY_ = energyDistrPixelY_+energyDistrPixelY_*energyDistrLaserInv_/(energyDistrLaserInv_+energyDistrEngineInv_)*energyDistrT_;

    energyDistrPoint_->setLeft((int)energyDistrX_-3);
    energyDistrPoint_->setTop((int)energyDistrY_-3);

  }

  void HUD::setPrimar(int i){
    primarValue_=i;
    if(i==1){
      primarChoice_->setLeft(2);
      primarChoice_->setWidth(48);
    }
    if(i==2){
      primarChoice_->setLeft(51);
      primarChoice_->setWidth(49);
    }
    else{
      primarChoice_->setLeft(101);
      primarChoice_->setWidth(48);
    }
  }



  void HUD::init(void)
  {
    timeText_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/Time/Text");

    targetWindowNameText_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/TargetWindow/Name");
    targetWindowDistanceText_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/TargetWindow/DistanceText");
    targetWindowStatusText_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/TargetWindow/StatusText");
    targetWindowHitRatingText_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/TargetWindow/HitRatingText");

    energyLength_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/EnergyBackground");

    shieldLeftTop_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/shieldLeftTop");
    shieldRightTop_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/shieldRightTop");
    shieldLeftBottom_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/shieldLeftBottom");
    shieldRightBottom_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/shieldRightBottom");

    rocketNum1_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/RocketNum1");
    rocketNum2_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/RocketNum2");
    rocketNum3_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/RocketNum3");
    rocketNum4_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/RocketNum4");

    // FIXME: unused vars!
    float energyDistrPixelX_ = 100;
    float energyDistrPixelY_ = 86;
    energyDistrPoint_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/EnergyDistrPoint");

    primarChoice_ = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/PrimarChoice");
  }

}
