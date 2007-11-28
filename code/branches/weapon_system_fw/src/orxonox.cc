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

/**
* Basic part of the game.
* It sets up Ogre and most important of all: Orxonox is the master of the
* main loop and therefore time itself.
*/

#include "OgreRoot.h"
#include "OgreTimer.h"
#include "OgreWindowEventUtilities.h"

#include "ogre_control.h"
#include "run_manager.h"
#include "orxonox.h"


namespace orxonox {

  /**
  * Empty Constructor.
  */
  Orxonox::Orxonox()
  {
  }


  /**
  * Empty Destructor.
  */
  Orxonox::~Orxonox()
  {
  }


  /**
  * Starts and runs the game
  */
  void Orxonox::go(void)
  {
	  if (!setup())
		  return;

    timer_ = new Ogre::Timer();

	  unsigned long lastTime = timer_->getMilliseconds();

	  while (true)
	  {
		  //Pump messages in all registered RenderWindow windows
      Ogre::WindowEventUtilities::messagePump();

		  ogre_->getRoot()->renderOneFrame();

		  if (!runMgr_->tick(timer_->getMilliseconds(),
              (timer_->getMilliseconds() - lastTime) / 1000.0))
			  break;
		  lastTime = timer_->getMilliseconds();
	  }

	  // clean up
	  destroy();
  }


  /**
  * Create render engine, render window and the Run manager.
  * @return False if failed.
  */
  bool Orxonox::setup(void)
  {
	  // create new 3D ogre render engine
	  ogre_ = new OgreControl();
	  ogre_->initialise();

    runMgr_ = RunManager::createSingleton();
    runMgr_->initialise(ogre_);

	  return true;
  }


  /**
  * Clean everything up.
  */
  void Orxonox::destroy()
  {
	  if (timer_)
		  delete timer_;
	  if (runMgr_)
      RunManager::destroySingleton();
	  if (ogre_)
		  delete ogre_;
  }

}
