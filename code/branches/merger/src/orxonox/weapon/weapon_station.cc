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

#include "OgreMath.h"
#include "OgreVector3.h"
#include "OgreStringConverter.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreSceneManager.h"

#include "base_weapon.h"

#include "weapon_station.h"


namespace orxonox {
namespace weapon {
  using namespace Ogre;

  WeaponStation::WeaponStation(int stationSize)
        : stationSize_(stationSize), lastActiveSlotIndex_(-1),
        slots_(new BaseWeapon*[stationSize])
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
    if (lastActiveSlotIndex_ = stationSize_ - 1)
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
}
