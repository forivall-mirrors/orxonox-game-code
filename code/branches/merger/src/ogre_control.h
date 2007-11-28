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


#ifndef OGRE_CONTROL_H
#define OGRE_CONTROL_H

#include "OgrePrerequisites.h"

#include "orxonox_prerequisites.h"


namespace orxonox {

  class OgreControl
  {
  public:
	  OgreControl();

	  virtual ~OgreControl();

	  /** Sets up the application - returns false if the user chooses to abandon configuration. */
	  virtual bool initialise();

    virtual Ogre::Root* getRoot();

	  virtual Ogre::RenderWindow* getRenderWindow();

	  virtual Ogre::String getResourcePath();

  protected:
	  /** Configures the application - returns false if the user chooses to abandon configuration. */
	  virtual bool configure(void);

	  /// Method which will define the source of resources (other than current folder)
	  virtual void setupResources(void);

  protected:
	  Ogre::Root         *root_;
	  Ogre::RenderWindow *window_;
	  Ogre::String  resourcePath_;

  };

}

#endif /* OGRE_CONTROL_H */
