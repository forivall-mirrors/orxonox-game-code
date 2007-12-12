/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      HUD design: Yuning Chai
 *      Implementation: Yuning Chai
 *   Co-authors:
 *      Implementation: Reto Grieder
 *
 */

//#include "OgreMath.h"
//#include "OgreVector3.h"
#include "OgreStringConverter.h"
#include "OgreOverlayManager.h"
#include "OgreOverlay.h"
#include "OgreOverlayElement.h"

#include "hud_overlay.h"


namespace orxonox {
namespace hud {
  using namespace Ogre;

  HUDOverlay::HUDOverlay()
  {
    //TODO: write safe guard
    overlay_ = OverlayManager::getSingleton().getByName("Orxonox/HUD1.2");
  }


  HUDOverlay::~HUDOverlay()
  {
  }


  void HUDOverlay::show()
  {
    //TODO: safe guard
    overlay_->show();
  }


  void HUDOverlay::hide()
  {
    //TODO: safe guard
    overlay_->hide();
  }


  bool HUDOverlay::tick(unsigned long, Ogre::Real)
  {
		// set Time
		int TimeMin = 99;
		int TimeSec = 55;
		OverlayElement* TimeText = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/Time/Text");
		TimeText->setCaption(StringConverter::toString(TimeMin) + ":" + StringConverter::toString(TimeSec));
		
		// set TargetWindow
		String TargetWindowName = "HURRA";
		int TargetWindowDistance = 12;
		String TargetWindowStatus = "Dead";
		int TargetWindowHitRating = 30;
		OverlayElement* TargetWindowNameText = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/TargetWindow/Name");
		TargetWindowNameText->setCaption( TargetWindowName);		
		OverlayElement* TargetWindowDistanceText = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/TargetWindow/DistanceText");
		TargetWindowDistanceText->setCaption(StringConverter::toString(TargetWindowDistance) + "km" );
		OverlayElement* TargetWindowStatusText = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/TargetWindow/StatusText");
		TargetWindowStatusText->setCaption( TargetWindowStatus );
		OverlayElement* TargetWindowHitRatingText = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/TargetWindow/HitRatingText");
		TargetWindowHitRatingText->setCaption(StringConverter::toString(TargetWindowHitRating) + "%" );
		
		// set Energy
		int EnergyValue = 60.0;
		OverlayElement* EnergyLength = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/EnergyBackground");
		EnergyLength->setWidth((int)((double)60/(double)100*200));
		
		// set Shild
		bool ShildLeftTopValue = true;
		bool ShildRightTopValue = false;
		bool ShildLeftBottomValue = false;
		bool ShildRightBottomValue = true;
		OverlayElement* ShildLeftTop = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/ShildLeftTop");
		if (ShildLeftTopValue) ShildLeftTop->show();
		else ShildLeftTop->hide();
		OverlayElement* ShildRightTop = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/ShildRightTop");
		if (ShildRightTopValue) ShildRightTop->show();
		else ShildRightTop->hide();
		OverlayElement* ShildLeftBottom = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/ShildLeftBottom");
		if (ShildLeftBottomValue) ShildLeftBottom->show();
		else ShildLeftBottom->hide();
		OverlayElement* ShildRightBottom = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/ShildRightBottom");
		if (ShildRightBottomValue) ShildRightBottom->show();
		else ShildRightBottom->hide();
		
		// set Rockets
		int Rocket1 = 11;
		int Rocket2 = 22;
		int Rocket3 = 33;
		int Rocket4 = 44;
		OverlayElement* RocketNum1 = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/RocketNum1");
		RocketNum1->setCaption(StringConverter::toString(Rocket1));
		OverlayElement* RocketNum2 = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/RocketNum2");
		RocketNum2->setCaption(StringConverter::toString(Rocket2));
		OverlayElement* RocketNum3 = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/RocketNum3");
		RocketNum3->setCaption(StringConverter::toString(Rocket3));
		OverlayElement* RocketNum4 = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/RocketNum4");
		RocketNum4->setCaption(StringConverter::toString(Rocket4));
		
		// set EnergyDistribution
		double EnergyDistrPixelX = 100;
		double EnergyDistrPixelY = 86;		
		double EnergyDistrShild = 32;
		double EnergyDistrEngine = 50;
		double EnergyDistrLaser = 18;
		double EnergyDistrShildInv = 100 - EnergyDistrShild;
		double EnergyDistrEngineInv = 100 - EnergyDistrEngine;
		double EnergyDistrLaserInv = 100 - EnergyDistrLaser;
		double EnergyDistrY = (double)EnergyDistrEngineInv/((double)EnergyDistrEngineInv+(double)EnergyDistrLaserInv) * EnergyDistrPixelY;
		double EnergyDistrX = EnergyDistrY/1.7321;
		
		
		
		OverlayElement* EnergyDistrPoint = OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/EnergyDistrPoint");
//		EnergyDistrPoint->setLeft();
//		EnergyDistrPoint->setTop();


    // keep rendering
    return true;
  }
}
}
