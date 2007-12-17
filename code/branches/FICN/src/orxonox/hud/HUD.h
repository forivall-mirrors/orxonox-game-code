#ifndef MODULE_HUD_H
#define MODULE_HUD_H

#include <Ogre.h>
#include <OIS/OIS.h>
//#include <CEGUI/CEGUI.h>
//#include <CEGUIRenderer.h>

using namespace Ogre;

namespace hud
{

class HUD
{

  private:

	OverlayElement* timeText_;
	OverlayElement* targetWindowNameText_;
	OverlayElement* targetWindowStatusText_;
	OverlayElement* targetWindowDistanceText_;
	OverlayElement* targetWindowHitRatingText_;
	OverlayElement* energyLength_;
	OverlayElement* shieldLeftTop_;
	OverlayElement* shieldRightTop_;
	OverlayElement* shieldLeftBottom_;
	OverlayElement* shieldRightBottom_;
	OverlayElement* rocketNum1_;
	OverlayElement* rocketNum2_;
	OverlayElement* rocketNum3_;
	OverlayElement* rocketNum4_;
	OverlayElement* energyDistrPoint_;
	OverlayElement* primarChoice_;





	int timeMin_;
	int timeSec_;

	String targetWindowName_;
	String targetWindowStatus_;
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

	double energyDistrShield_;
	double energyDistrEngine_;
	double energyDistrLaser_;

	double energyDistrPixelX_;
	double energyDistrPixelY_;

	int primarValue_;






 public:
	HUD( void );
	~HUD( void );
	void init( void );

	void setTime(int i, int j);
	void setTargetWindowName(String i);
	void setTargetWindowStatus(String i);
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
	void setEnergyDistr(double s, double e, double l);
	void setPrimar(int i);






};


}
#endif
