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


#ifndef AmmunitionDump_H
#define AmmunitionDump_H

#include <OgrePrerequisites.h>

#include "orxonox/OrxonoxPrereqs.h"

#include "network/Synchronisable.h"
//#include "../core/CoreIncludes.h"
#include "../BaseObject.h"


namespace orxonox {
  class AmmunitionDump : public BaseObject, public network::Synchronisable
  {
  public:
    AmmunitionDump();
	  ~AmmunitionDump();

    void setDumpSize(const Ogre::String &name, int size);

    int store(const Ogre::String &name, int quantiy);

    int getAmmunition(const Ogre::String &name, int quantity);

    int getStockSize(const Ogre::String &name);

    virtual void loadParams(TiXmlElement* xmlElem) { BaseObject::loadParams(xmlElem); };


  protected:
    inline bool create() { return true; }

    int numberOfAmmos_;
    int *stock_;
    int *capacity_;
  };
}

#endif /* AmmunitionDump_H */
