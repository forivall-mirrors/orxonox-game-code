/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "orxonox/Orxonox.h"
#include "bullet_manager.h"
#include "tinyxml/tinyxml.h"

#include "ammunition_dump.h"


namespace orxonox {
  CreateFactory(AmmunitionDump);

  AmmunitionDump::AmmunitionDump()
    : numberOfAmmos_(Orxonox::getSingleton()->getBulletMgr()->getNumberOfAmmos()),
      stock_(new int[numberOfAmmos_]),
      capacity_(new int[numberOfAmmos_])
  {
    RegisterObject(AmmunitionDump);

    for (int i = 0; i < numberOfAmmos_; i++)
    {
      stock_[i] = 0;
      capacity_[i] = 0;
    }
  }


  AmmunitionDump::~AmmunitionDump()
  {
    if (stock_)
      delete stock_;
    if (capacity_)
      delete capacity_;
  }

  void AmmunitionDump::setDumpSize(const Ogre::String &name, int size)
  {
    if (size < 0)
      return;
    int id = Orxonox::getSingleton()->getBulletMgr()->getAmmunitionID(name);
    if (id == -1)
      return;
    capacity_[id] = size;
  }


  int AmmunitionDump::store(const Ogre::String &name, int quantity)
  {
    int id = Orxonox::getSingleton()->getBulletMgr()->getAmmunitionID(name);
    if (id == -1)
      return quantity;
    stock_[id] += quantity;
    if (stock_[id] > capacity_[id])
    {
      quantity = capacity_[id] - stock_[id];
      stock_[id] = capacity_[id];
      return quantity;
    }
    else
      return 0;
  }


  int AmmunitionDump::getAmmunition(const Ogre::String &name, int quantity)
  {
    int id = Orxonox::getSingleton()->getBulletMgr()->getAmmunitionID(name);
    if (id == -1)
      return 0;
    if (stock_[id] >= quantity)
      stock_[id] -= quantity;
    else
    {
      quantity = stock_[id];
      stock_[id] = 0;
    }
    return quantity;
  }


  int AmmunitionDump::getStockSize(const Ogre::String &name)
  {
    int id = Orxonox::getSingleton()->getBulletMgr()->getAmmunitionID(name);
    if (id = -1)
      return -1;
    return stock_[id];
  }
}
