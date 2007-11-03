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


#ifndef RUN_MANAGER_H
#define RUN_MANAGER_H

#include "Ogre.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

#include "ogre_control.h"
#include "orxonox_scene.h"
#include "orxonox_ship.h"


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
	RunManager(OgreControl*);

	virtual ~RunManager();

	virtual bool tick(unsigned long, Real);

protected:
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

protected:
	OgreControl  *ogre_;
	SceneManager *sceneMgr_;
	RenderWindow *window_;
	OrxonoxScene *backgroundScene_;
	Camera       *camera_;
	OrxonoxShip  *playerShip_;

	bool statsOn_;
	std::string debugText_;

	unsigned int screenShotCounter_;
	// just to stop toggles flipping too fast
	Real timeUntilNextToggle_;
	bool leftButtonDown_;
	TextureFilterOptions filtering_;
	int aniso_;

	int sceneDetailIndex_;
	Overlay* debugOverlay_;

	//OIS Input devices
	OIS::InputManager* inputManager_;
	OIS::Mouse*    mouse_;
	OIS::Keyboard* keyboard_;
	OIS::JoyStick* joystick_;

  const Real mouseSensitivity_;

  // Bullet array
	Bullet **bullets_;
	int bulletsSize_;
	int bulletsIndex_;

	// previously elapsed render time
	unsigned long totalTime_;

};

#endif /* RUN_MANAGER_H */
