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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */


#ifndef __RunManager__
#define __RunManager__

#include "Ogre.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

#include "OgreControl.h"
#include "OrxonoxScene.h"
#include "OrxonoxShip.h"

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
	SceneNode *mShipNode;
	OrxonoxShip *mShip;

	bool mStatsOn;

	std::string mDebugText;

	unsigned int mNumScreenShots;
	// just to stop toggles flipping too fast
	Real mTimeUntilNextToggle ;
	TextureFilterOptions mFiltering;
	int mAniso;

	int mSceneDetailIndex ;
	Overlay* mDebugOverlay;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;

	bool leftButtonDown;
	Bullet **mBullets;
	int mBulletsSize;
	int mBulletsPosition;

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

	virtual void showDebugOverlay(bool);

};

#endif