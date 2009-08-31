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
    @brief Definition of HealthImmediate.
*/

#ifndef _HealthImmediate_H__
#define _HealthImmediate_H__

#include "OrxonoxPrereqs.h"
#include "pickup/PassiveItem.h"

namespace orxonox
{
    /**
        @brief Health-item, immediatly recovers health when picked up.
    */
    class _OrxonoxExport HealthImmediate : public PassiveItem
    {
    public:
        HealthImmediate(BaseObject* creator);
        virtual ~HealthImmediate();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        virtual bool pickedUp(Pawn* pawn);

        inline float getRecoveredHealth() const
            { return this->recoveredHealth_; }
        inline void setRecoveredHealth(float recovery)
            { this->recoveredHealth_ = recovery; }

    private:
        float recoveredHealth_;
    };
}

#endif /* _HealthImmediate_H__ */
