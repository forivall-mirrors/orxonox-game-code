/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
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

#include "OrxonoxStableHeaders.h"
#include "WeaponStation.h"

#include "BaseWeapon.h"


namespace orxonox {
  WeaponStation::WeaponStation(int stationSize)
        : slots_(new BaseWeapon*[stationSize]),
          lastActiveSlotIndex_(-1), stationSize_(stationSize)
  {
  	//slots_ = new BaseWeapon*[stationSize];
  }


  WeaponStation::~WeaponStation()
  {
    if (slots_)
      delete slots_;
    // when factory available: delete weapons too
  }


  bool WeaponStation::addWeapon(BaseWeapon *weapon)
  {
    if (lastActiveSlotIndex_ == stationSize_ - 1)
      return false;

    // this is ugly, but for the time being, it has to fit.
    slots_[++lastActiveSlotIndex_] = weapon;
    selectedSlot_ = slots_ + lastActiveSlotIndex_;
    return true;
  }


  BaseWeapon* WeaponStation::selectWeapon(int slotNumber)
  {
    if (slotNumber < lastActiveSlotIndex_)
      return NULL;
    return slots_[slotNumber];
  }

}
