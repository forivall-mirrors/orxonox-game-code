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


#ifndef _AmmunitionDump_H__
#define _AmmunitionDump_H__

#include <string>

#include <OgrePrerequisites.h>

#include "../../OrxonoxPrereqs.h"

#include "network/Synchronisable.h"
//#include "../core/CoreIncludes.h"
#include "orxonox/core/BaseObject.h"


namespace orxonox {
  class _OrxonoxExport AmmunitionDump : public BaseObject, public network::Synchronisable
  {
  public:
    AmmunitionDump();
	  ~AmmunitionDump();

    void setDumpSize(const std::string &name, int size);

    int store(const std::string &name, int quantiy);

    int getAmmunition(const std::string &name, int quantity);

    int getStockSize(const std::string &name);

    virtual void loadParams(TiXmlElement* xmlElem) { BaseObject::loadParams(xmlElem); };


  protected:
    inline bool create() { return true; }

    int numberOfAmmos_;
    int *stock_;
    int *capacity_;
  };
  ExportClass(AmmunitionDump, Orxonox);
}

#endif /* _AmmunitionDump_H__ */
