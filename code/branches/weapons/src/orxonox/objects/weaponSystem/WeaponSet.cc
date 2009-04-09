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
 *      Martin Polak
 *   Co-authors:
 *      ... *
 */

#include "OrxonoxStableHeaders.h"
#include "WeaponSet.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/worldentities/pawns/Pawn.h"

#include "Weapon.h"
#include "WeaponSlot.h"
#include "WeaponPack.h"
#include "WeaponSystem.h"

namespace orxonox
{
    CreateFactory(WeaponSet);

    WeaponSet::WeaponSet(BaseObject* creator, int k) : BaseObject(creator)
    {
        RegisterObject(WeaponSet);

        this->weaponSystem_ = 0;
        this->attachedWeaponPack_ = 0;

COUT(0) << "+WeaponSet" << std::endl;
    }

    WeaponSet::~WeaponSet()
    {
COUT(0) << "~WeaponSet" << std::endl;
    }

    void WeaponSet::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WeaponSet, XMLPort, xmlelement, mode);

        XMLPortParam(WeaponSet, "firemode", setFireMode, getFireMode, xmlelement, mode);
    }

    void WeaponSet::attachWeaponPack(WeaponPack *wPack)
    {
        if ( this->weaponSystem_->getWeaponSlotSize()>0 && wPack->getSize()>0 && ( wPack->getSize() <= this->weaponSystem_->getWeaponSlotSize() ) )
        {
            this->attachedWeaponPack_ = wPack;
            int wPackWeapon = 0;    //WeaponCounter for Attaching

            //should be possible to choose which slot to use
            //attach every weapon of the weaponPack to a weaponSlot
            for (  int i=0; i < wPack->getSize() ; i++  )
            {
                //at the moment this function only works for one weaponPack in the entire WeaponSystem...
                //it also takes the first free weaponSlot...
                if ( this->weaponSystem_->getWeaponSlot(i)->getAttachedWeapon() == 0 && this->weaponSystem_->getWeaponSlot(i) != 0) //if slot not full
                {
                    this->setWeaponSlots_.push_back( this->weaponSystem_->getWeaponSlot(i) );
                    this->weaponSystem_->getWeaponSlot(i)->attachWeapon( wPack->getWeaponPointer(wPackWeapon) );
                    this->weaponSystem_->getPawn()->attach( wPack->getWeaponPointer(wPackWeapon) );
                    wPackWeapon++;
                }
                else
                {
                    for (int k=0; k < this->weaponSystem_->getWeaponSlotSize(); k++)
                    {
                        if ( this->weaponSystem_->getWeaponSlot(k)->getAttachedWeapon() == 0 )
                        {
                            this->setWeaponSlots_.push_back( this->weaponSystem_->getWeaponSlot(k) );
                            this->weaponSystem_->getWeaponSlot(k)->attachWeapon( wPack->getWeaponPointer(wPackWeapon) );
                            this->weaponSystem_->getPawn()->attach( wPack->getWeaponPointer(wPackWeapon) );
                            wPackWeapon++;
                        }
                    }
                }
            }
        }
    }


    void WeaponSet::fire()
    {
        //fires all WeaponSlots available for this weaponSet attached from the WeaponPack
        if (this->attachedWeaponPack_)
            this->attachedWeaponPack_->fire();
    }
}
