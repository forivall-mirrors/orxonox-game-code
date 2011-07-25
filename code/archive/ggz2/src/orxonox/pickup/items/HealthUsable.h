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
    @brief Definition of HealthUsable.
*/

#ifndef _HealthUsable_H__
#define _HealthUsable_H__

#include "OrxonoxPrereqs.h"

#include <climits>
#include "pickup/UsableItem.h"

namespace orxonox
{
    /**
        @brief Health-item, enables player recover health when used.
    */
    class _OrxonoxExport HealthUsable : public UsableItem
    {
    public:
        HealthUsable(BaseObject* creator);      //!< Constructor
        virtual ~HealthUsable();                //!< Deconstructor

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);  //!< XMLPort

        virtual int getMaxCarryAmount() const
            { return INT_MAX; }

        virtual void used(Pawn* pawn);          //!< Called when the item is used.

        virtual bool pickedUp(Pawn* pawn);      //!< Called when the item is picked up.
        virtual bool dropped(Pawn* pawn);       //!< Called when the item is dropped.

        /**
            @brief Get the amount of jumps available.
            @return Returns how many times the item can be used.
        */
        inline float getRecoveredHealth() const
            { return this->recoveredHealth_; }
        /**
            @brief Set the amount of jumps available.
            @param num New number of available jumps.
        */
        inline void setRecoveredHealth(float recovery)
            { this->recoveredHealth_ = recovery; }
    private:
        float recoveredHealth_;    //!< Amount of jumps still available.
    };
}

#endif /* _HealthUsable_H__ */
