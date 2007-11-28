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
//#include "OgreTextureManager.h"

#include <OIS/OISPrereqs.h>

#include "orxonox_prerequisites.h"


namespace orxonox {

  // let the class inherit from WindowEventListener in order for the RunMgr
  // to act as the central point of all the calcuations in Orxonox
  class RunManager : public Ogre::WindowEventListener
  {
  public:
    virtual ~RunManager();
    
    void initialise(OgreControl*);

    bool tick(unsigned long, Ogre::Real);

    Ogre::SceneManager& getSceneManager();

    Ogre::SceneManager* getSceneManagerPtr();

    weapon::BulletManager* getBulletManagerPtr();

    int getAmmunitionID(const Ogre::String&);

    int getNumberOfAmmos();

    static RunManager* createSingleton();

    static void destroySingleton();

    static RunManager& getSingleton(void);

    static RunManager* getSingletonPtr(void);


  protected:
    RunManager();

	  void createCamera(void);

	  void createViewports(void);


	  /** EVENT HANDLING **/

	  //Adjust mouse clipping area
	  void windowResized(Ogre::RenderWindow*);

	  //Unattach OIS before window shutdown (very important under Linux)
	  void windowClosed(Ogre::RenderWindow*);


	  /** INPUT PROCESSING **/
	  bool processUnbufferedKeyInput();

	  bool processUnbufferedMouseInput();


	  /** OUTPUT **/

	  void showDebugOverlay(bool);

  protected:
    // directly Ogre related fields
	  Ogre::SceneManager *sceneMgr_;
	  Ogre::RenderWindow *window_;
	  Ogre::Camera       *camera_;


    // self made orxonox fields
	  OgreControl  *ogre_;
	  OrxonoxScene *backgroundScene_;
	  OrxonoxShip  *playerShip_;
    hud::TestOverlay *hud_;

    // Bullet manager
    weapon::BulletManager *bulletManager_;

    const Ogre::Real mouseSensitivity_;

	  // previously elapsed render time
	  unsigned long totalTime_;


    // fields from the example framework
	  bool statsOn_;

	  unsigned int screenShotCounter_;
	  // just to stop toggles flipping too fast
	  Ogre::Real timeUntilNextToggle_;

	  //OIS Input devices
	  OIS::InputManager* inputManager_;
	  OIS::Mouse*    mouse_;
	  OIS::Keyboard* keyboard_;
	  OIS::JoyStick* joystick_;

    // singleton pointer
    static RunManager *singletonPtr_s;

  };

}

#endif /* RUN_MANAGER_H */
