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
 *      Daniel 'Huty' Haggenmueller
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Definition of BaseItem (base-class for items/pickups).
*/

#ifndef _BaseItem_H__
#define _BaseItem_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "core/BaseObject.h"

// tolua_begin
namespace orxonox
{
    /**
        @brief
            Base class for all items/pickups.

            Provides common methods to be used in derived classes.
        @author
            Daniel 'Huty' Haggenmueller
    */
    class _OrxonoxExport BaseItem : public BaseObject
    {
// tolua_end
    public:
        BaseItem(BaseObject* creator);
        virtual ~BaseItem();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);  //!< XMLPort

        /**
            @brief Checks how many instances of this item can be carried at a time.
            @return How many of this item can be carried.
        */
        virtual int getMaxCarryAmount() const
            { return 1; }

        bool addTo(Pawn* pawn);             //!< Add the item to a pawn.
        bool removeFrom(Pawn* pawn);        //!< Removes the item from a pawn.
        /**
            @brief
                Method invoked when the item gets picked up.

                Has to be overridden for an item to work,
                should contain a call to addTo().

            @param pawn Pawn who picks up the item.
            @return Returns whether the pawn was able to pick up the item.
        */
        virtual bool pickedUp(Pawn* pawn)
            { return false; }
        /**
            @brief
                Method invoked when the item is dropped from a player.

                Should be overridden by derived classes,
                should also contain a call to removeFrom().

            @param pawn Pawn which dropped the item.
            @return Returns whether the item was able to get dropped by the pawn.
        */
        virtual bool dropped(Pawn* pawn)
            { return false; }

        /**
            @brief Gets the current owner of the pickup.
            @return Returns the current owner.
        */
        inline Pawn* getOwner() const
            { return this->owner_; }
        /**
            @brief Sets the owner of the pickup.
            @param owner New owner for the pickup.
        */
        inline void setOwner(Pawn* owner)
            { this->owner_ = owner; }

        /**
            @brief Gets the pickupIdentifier of the item.
            @return Returns the pickupIdentifier of the item.
            @see pickupIdentifier_
        */
        inline const std::string& getPickupIdentifier() const
            { return this->pickupIdentifier_; }
        /**
            @brief Sets the pickupIdentifier for the item.
            @param identifier New pickupIdentifier for the item.
            @see pickupIdentifier_
        */
        inline void setPickupIdentifier(const std::string& identifier)
            { this->pickupIdentifier_ = identifier; }

        // GUI stuff
        virtual const std::string& getGUIText() const; // tolua_export
        inline void setGUIText(const std::string& text)
            { this->guiText_ = text; }

        virtual const std::string& getGUIImage() const
            { return this->guiImage_; }
        inline void setGUIImage(const std::string& image)
            { this->guiImage_ = image; }
    private:
        Pawn* owner_;   //!< The current owner of the item.

        /**
            @brief
                The pickupIdentifier of the item..

                Usually set to the template name used by a PickupSpawner,
                used to index items in the PickupCollection.
        */
        std::string pickupIdentifier_;

        std::string guiText_;
        std::string guiImage_;
    }; // tolua_export
} // tolua_export

#endif /* _BaseItem_H__ */
