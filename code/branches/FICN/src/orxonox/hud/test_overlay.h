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
 *      HUD design: Yuning Chai
 *      Implementation: Yuning Chai
 *   Co-authors:
 *      Implementation: Reto Grieder
 *
 */


#ifndef TEST_OVERLAY_H
#define TEST_OVERLAY_H

#include "OgrePrerequisites.h"

#include "../orxonox_prerequisites.h"


namespace orxonox {
namespace hud {

  class TestOverlay
  {
  public:
    TestOverlay(Ogre::RenderWindow*);
	  virtual ~TestOverlay();

    void show();

    void hide();

    void setDebugText(Ogre::String&);

    bool tick(unsigned long, Ogre::Real);

  protected:

  public:

  protected:
    Ogre::Overlay *overlay_;
    Ogre::RenderWindow *window_;

    Ogre::String debugText_;

  };
}
}

#endif /* TEST_OVERLAY_H */
