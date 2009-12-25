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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Definition of the Pickupable class.
*/

#ifndef _Pickupable_H__
#define _Pickupable_H__

#include "OrxonoxPrereqs.h"
#include "core/OrxonoxClass.h"

#include "core/Identifier.h"
#include "core/Super.h"
#include "interfaces/PickupCarrier.h"
#include "worldentities/pawns/Pawn.h"
#include <list>

namespace orxonox
{

    //! Enum for the activation type.
    namespace pickupActivationType
    {
        enum Value
        {
            immediate,
            onUse,
        };
    }
    
    //! Enum for the duration tyoe.
    namespace pickupDurationType
    {
        enum Value
        {
            once,
            continuous,
        };
    }
    
    /**
    @brief
        An Interface (or more precisely an abstract Class) to model and manage different (all kinds of) pickups.
    @author
        Damian 'Mozork' Frick
    */
    //TODO: Add stuff like weight/space ?
    class _OrxonoxExport Pickupable : virtual public OrxonoxClass
    {
        public:
            Pickupable(); //!< Default constructor.
            virtual ~Pickupable() {} //!< Default destructor.
            
            /**
            @brief Get the activation type of the pickup.
            @return Returns the activation type of the pickup.
            */
            inline pickupActivationType::Value getActivationType(void)
                { return this->activationType_; }
            /**
            @brief Get the duration type of the pickup.
            @return Returns the duration type of the pickup.
            */
            inline pickupDurationType::Value getDurationType(void)
                { return this->durationType_; }
                
            /**
            @brief Get the owner of the pickup.
            @return Returns a pointer to the owner of the pickup.
            */
            inline PickupCarrier* getOwner(void)
                { return this->owner_; }
                
            /**
            @brief Get whether the pickup is currently in use or not.
            @return Returns true if the pickup is currently in use.
            */
            inline bool isUsed(void)
                { return this->used_; }
                
            /**
            @brief Get whether the given pawn is a target of this pickup.
            @param pawn The Pawn of which it has to be determinde whether it is a target of this pickup.
            @return Retruns true if the given Pawn is a target.
            */
            //TODO: Determine whether Pawn includes all possible cases and if PickupCarrier wouldn't be better.
            inline bool isTarget(Pawn* pawn)
                {
                    Identifier* identifier = pawn->getIdentifier();
                    for(std::list<Identifier*>::iterator it = this->targets_.begin(); it != this->targets_.end(); it++)
                    {
                        if(identifier->isA(*it))
                            return true;
                    }
                    return false;
                }
                
            /**
            @brief  Should be called when the pickup has transitetd from used to unused or the other way around.
                    Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedUsed); to their changeUsed method.
            */
            virtual inline void changedUsed(void)
                {
                    if(this->isUsed())
                        this->used_ = false;
                    else
                        this->used_ = true;
                }

            /**
            @brief  Sets the pickup to used and makes sure the effects of the pickup take effect at the right places.
                    This method needs to be implemented by any Class inheriting from Pickupable.
            @return Returns false if for some reason the method could not take effect, e.g. because it is already in use, or some other circumstance.
            */
            virtual bool use(void) = 0;
            /**
            @brief  Sets the pickup to unused and makes sure the effects of the pickup no longer take effect.
                    This method needs to be implemented by any Class inheriting from Pickupable.
            @return Returns false if for some reason the method could not take effect, e.g. because the pickup is already unused, or some other circumstance.
            */
            virtual bool unuse(void) = 0;
            
            /**
            @brief  Adds the pickup to the input PickupCarrier.
                    This method needs to be implemented by any Class inheriting from Pickupable.
            @return Returns false if, for some reason, the pickup could not be picked up.
            */
            //TODO: Maybe better in PickupCarrier?
            virtual bool pickup(PickupCarrier* carrier) = 0;
            /**
            @brief  Drops the pickup. Creates a PickupSpawner at the place the Pickup has been dropped.
                    This method needs to be implemented by any Class inheriting from Pickupable.
            @return Returns false if the pickup could not be dropped.
            */
            //TODO: Probably could be done here?
            virtual bool drop(void) = 0;
            
            /**
            @brief  Creates a duplicate of the pickup.
                    This method needs to e implemented by any Class inheriting from Pickupable.
            @return Returns the clone of this pickup as a pointer to a Pickupable.
            */
            //TODO: Would be nicer if standard case was implemented here.
            virtual Pickupable* clone(void) = 0;
            
            /**
            @brief Sets the owner of the pickup.
            @param owner Sets the input PickupCarrier as the owner of the pickup.
            */
            //TODO: Protected? Check for NULL and return true/false?
            inline void setOwner(PickupCarrier* owner)
                { this->owner_ = owner; }
            
        private:
            
            pickupActivationType::Value activationType_; //!< The activation type of the Pickup.
            pickupDurationType::Value durationType_; //!< The duration type of the pickup.
            
            bool used_; //!< Whether the pickup is currently in use or not.
            
            PickupCarrier* owner_; //!< The owner of the pickup.
            std::list<Identifier*> targets_; //!< The possible targets of this pickup.

    };
    
    SUPER_FUNCTION(10, Pickupable, changedUsed, true)
}

#endif /* _Pickupable_H__ */
