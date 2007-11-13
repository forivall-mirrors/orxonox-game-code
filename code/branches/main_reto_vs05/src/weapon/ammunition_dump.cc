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

#include "ammunition_dump.h"


namespace orxonox {
namespace weapon {

  AmmunitionDump::AmmunitionDump(int capacity)
        : stock_(0), capacity_(capacity)
  {
  }


  AmmunitionDump::~AmmunitionDump()
  {
  }

  
  void AmmunitionDump::store(int quantity)
  {
    stock_ += quantity;
    if (stock_ > capacity_)
      stock_ = capacity_;
  }


  int AmmunitionDump::getAmmunition(int quantity)
  {
    if (stock_ >= quantity)
    {
      stock_ -= quantity;
      return quantity;
    }
    else
    {
      quantity = stock_;
      stock_ = 0;
      return quantity;
    }
  }


  int AmmunitionDump::getStockSize()
  {
    return stock_;
  }
}
}
