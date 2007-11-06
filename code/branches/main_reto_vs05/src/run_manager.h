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

#include "OgrePrerequisites.h"
#include "OgreWindowEventUtilities.h"
#include "OgreTextureManager.h"

#include <OIS/OISPrereqs.h>

#include "orxonox_prerequisites.h"


namespace orxonox {

  // let the class inherit from WindowEventListener in order for the RunMgr
  // to act as the central point of all the calcuations in Orxonox
  class RunManager : Ogre::WindowEventListener
  {
  public:
	  RunManager(OgreControl*);

	  virtual ~RunManager();

    virtual bool tick(unsigned long, Ogre::Real);

  protected:
	  virtual void createCamera(void);

	  virtual void createViewports(void);


	  /** EVENT HANDLING **/

	  //Adjust mouse clipping area
	  virtual void windowResized(Ogre::RenderWindow*);

	  //Unattach OIS before window shutdown (very important under Linux)
	  virtual void windowClosed(Ogre::RenderWindow*);


	  /** INPUT PROCESSING **/
	  virtual bool processUnbufferedKeyInput();

	  virtual bool processUnbufferedMouseInput();


	  /** OUTPUT **/

	  virtual void updateStats(void);

	  virtual void showDebugOverlay(bool);

  protected:
	  Ogre::SceneManager *sceneMgr_;
	  Ogre::RenderWindow *window_;
	  Ogre::Camera       *camera_;
	  OgreControl  *ogre_;
	  OrxonoxScene *backgroundScene_;
	  OrxonoxShip  *playerShip_;

	  bool statsOn_;
	  std::string debugText_;

	  unsigned int screenShotCounter_;
	  // just to stop toggles flipping too fast
	  Ogre::Real timeUntilNextToggle_;
	  bool leftButtonDown_;
	  Ogre::TextureFilterOptions filtering_;
	  int aniso_;

	  int sceneDetailIndex_;
	  Ogre::Overlay* debugOverlay_;

	  //OIS Input devices
	  OIS::InputManager* inputManager_;
	  OIS::Mouse*    mouse_;
	  OIS::Keyboard* keyboard_;
	  OIS::JoyStick* joystick_;

    const Ogre::Real mouseSensitivity_;

    // Bullet array
	  /*Bullet **bullets_;
	  int bulletsSize_;
	  int bulletsIndex_;*/
    BulletManager *bulletManager_;

	  // previously elapsed render time
	  unsigned long totalTime_;

  };

}

#endif /* RUN_MANAGER_H */
