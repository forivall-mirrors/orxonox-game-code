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
    @brief Definition of Jump.
*/

#ifndef _Jump_H__
#define _Jump_H__

#include <climits>

#include "OrxonoxPrereqs.h"

#include <climits>
#include "util/Math.h"
#include "pickup/UsableItem.h"

namespace orxonox
{
    /**
        @brief Jump-item, enables player to "jump" into a direction.
    */
    class _OrxonoxExport Jump : public UsableItem
    {
    public:
        Jump(BaseObject* creator);      //!< Constructor
        virtual ~Jump();                //!< Deconstructor

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);  //!< XMLPort

        virtual int getMaxCarryAmount() const
            { return INT_MAX; }

        virtual void used(Pawn* pawn);          //!< Called when the item is used.

        virtual bool pickedUp(Pawn* pawn);      //!< Called when the item is picked up.
        virtual bool dropped(Pawn* pawn);       //!< Called when the item is dropped.

        /**
            @brief Get the velocity added when the item is used.
            @return Returns the added velocity (relative to the Pawn).
        */
        inline const Vector3& getVelocity() const
            { return this->velocity_; }
        /**
            @brief Set the velocity added when the item is used.
            @param velocity New added velocity (relative to Pawn).
        */
        inline void setVelocity(const Vector3& velocity)
            { this->velocity_ = velocity; }
        /**
            @brief Get the amount of jumps available.
            @return Returns how many times the item can be used.
        */
        inline int getJumpsAvailable() const
            { return this->jumpsAvailable_; }
        /**
            @brief Set the amount of jumps available.
            @param num New number of available jumps.
        */
        inline void setJumpsAvailable(int num)
            { this->jumpsAvailable_ = num; }
    private:
        Vector3 velocity_;      //!< The velocity added when the item is used.
        int jumpsAvailable_;    //!< Amount of jumps still available.
    };
}

#endif /* _Jump_H__ */
