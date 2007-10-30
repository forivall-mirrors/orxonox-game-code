#ifndef __RunManager__
#define __RunManager__

#include "Ogre.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

#include "OgreControl.h"
#include "OrxonoxScene.h"
#include "OrxonoxShip.h"
#include "SteerableNode.h"

//Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

using namespace Ogre;


// let the class inherit from WindowEventListener in order for the RunMgr
// to act as the central point of all the calcuations in Orxonox
class RunManager : WindowEventListener
{
public:
	RunManager(OgreControl*, bool = false, bool = false, bool = false);

	virtual ~RunManager();

	virtual bool tick(unsigned long, float);

protected:
	OgreControl  *mOgre;

	SceneManager *mSceneMgr;
	RenderWindow* mWindow;

	OrxonoxScene *mScene;
	Camera* mCamera;
	SteerableNode *mShipNode;
	OrxonoxShip *mShip;

	Vector3 mTranslateVector;
	bool mStatsOn;

	std::string mDebugText;

	unsigned int mNumScreenShots;
	float mMoveScale;
	Degree mRotScale;
	// just to stop toggles flipping too fast
	Real mTimeUntilNextToggle ;
	Radian mRotX, mRotY;
	TextureFilterOptions mFiltering;
	int mAniso;

	int mSceneDetailIndex ;
	Real mMoveSpeed;
	Degree mRotateSpeed;
	Overlay* mDebugOverlay;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;

	// previously elapsed render time
	unsigned long mTime;

	virtual void createCamera(void);

	virtual void createViewports(void);


	/** EVENT HANDLING **/

	//Adjust mouse clipping area
	virtual void windowResized(RenderWindow*);

	//Unattach OIS before window shutdown (very important under Linux)
	virtual void windowClosed(RenderWindow*);


	/** INPUT PROCESSING **/
	virtual bool processUnbufferedKeyInput();

	virtual bool processUnbufferedMouseInput();


	/** OUTPUT **/

	virtual void updateStats(void);

	virtual void moveCamera();

	virtual void showDebugOverlay(bool);

};

#endif