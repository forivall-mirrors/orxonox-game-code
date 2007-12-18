#ifndef MODULE_HUD_H
#define MODULE_HUD_H

#include <Ogre.h>
#include <OIS/OIS.h>
//#include <CEGUI/CEGUI.h>
//#include <CEGUIRenderer.h>


namespace hud
{

class HUD
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

	Ogre::String targetWindowName_;
	Ogre::String targetWindowStatus_;
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
	void setTargetWindowName(Ogre::String i);
	void setTargetWindowStatus(Ogre::String i);
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
