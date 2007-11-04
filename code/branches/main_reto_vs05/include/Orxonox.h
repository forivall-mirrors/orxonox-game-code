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


#ifndef ORXONOX_H
#define ORXONOX_H

#include "OgrePrerequisites.h"

#include "Orxonox_prerequisites.h"

namespace Orxonox {

  class Orxonox
  {
  public:
    Orxonox();
    virtual ~Orxonox();
	  virtual void go();

  protected:
	  virtual bool setup(void);

	  virtual void destroy(void);

  protected:
	  OgreControl *ogre_;
	  RunManager  *runMgr_;
	  Ogre::Timer *timer_;

  };

}

#endif /* ORXONOX_H */
