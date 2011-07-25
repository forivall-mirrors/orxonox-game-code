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

#include <vector>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Debug.h"
#include "objects/worldentities/pawns/Pawn.h"

#include "WeaponSet.h"
#include "WeaponPack.h"

namespace orxonox
{
    CreateFactory(WeaponSet);

    WeaponSet::WeaponSet(BaseObject* creator, int k) : BaseObject(creator)
    {
        RegisterObject(WeaponSet);

        this->parentWeaponSystem_ = 0;
        this->attachedWeaponPack_ = 0;
    }

    WeaponSet::~WeaponSet()
    {
    }

    void WeaponSet::attachWeaponPack(WeaponPack *wPack)
    {
        if ( this->parentWeaponSystem_->getWeaponSlotSize()>0 && wPack->getSize()>0 && ( wPack->getSize() <= this->parentWeaponSystem_->getWeaponSlotSize() ) )
        {
            this->attachedWeaponPack_ = wPack;
            int wPackWeapon = 0;    //WeaponCounter for Attaching
           
            //should be possible to choose which slot to use
            //attach every weapon of the weaponPack to a weaponSlot
            for (  int i=0; i < wPack->getSize() ; i++  )
            {
                //at the moment this function only works for one weaponPack in the entire WeaponSystem...
                //it also takes the first free weaponSlot...
                if ( this->parentWeaponSystem_->getWeaponSlotPointer(i)->getAttachedWeapon() == 0 && this->parentWeaponSystem_->getWeaponSlotPointer(i) != 0) //if slot not full
                {
                    this->setWeaponSlots_.push_back( this->parentWeaponSystem_->getWeaponSlotPointer(i) );
                    this->parentWeaponSystem_->getWeaponSlotPointer(i)->attachWeapon( wPack->getWeaponPointer(wPackWeapon) );
                    this->parentWeaponSystem_->getParentPawn()->attach( wPack->getWeaponPointer(wPackWeapon) );
                    wPackWeapon++;
                }
                else
                {
                    for (int k=0; k < this->parentWeaponSystem_->getWeaponSlotSize(); k++)
                    {
                        if ( this->parentWeaponSystem_->getWeaponSlotPointer(k)->getAttachedWeapon() == 0 )
                        {
                            this->setWeaponSlots_.push_back( this->parentWeaponSystem_->getWeaponSlotPointer(k) );
                            this->parentWeaponSystem_->getWeaponSlotPointer(k)->attachWeapon( wPack->getWeaponPointer(wPackWeapon) );
                            this->parentWeaponSystem_->getParentPawn()->attach( wPack->getWeaponPointer(wPackWeapon) );
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

    void WeaponSet::setFireMode(const unsigned int firemode)
    {   this->firemode_ = firemode; }

    const unsigned int WeaponSet::getFireMode() const
    {   return this->firemode_; }

    void WeaponSet::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WeaponSet, XMLPort, xmlelement, mode);
        XMLPortParam(WeaponSet, "firemode", setFireMode, getFireMode, xmlelement, mode);
    }

}
