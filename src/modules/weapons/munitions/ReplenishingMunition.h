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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file ReplenishingMunition.h
    @brief Definition of the ReplenishingMunition class.
*/

#ifndef _ReplenishingMunition_H__
#define _ReplenishingMunition_H__

#include "weapons/WeaponsPrereqs.h"

#include "tools/Timer.h"
#include "weaponsystem/Munition.h"

namespace orxonox
{

    /**
    @brief
        Munition that is replenished by a certain amount each time interval.
    @author
        Fabian 'x3n' Landau
    @ingroup WeaponsMunitions
    */
    class _WeaponsExport ReplenishingMunition : public Munition
    {
        public:
            ReplenishingMunition(Context* context);
            virtual ~ReplenishingMunition() {}

        protected:
            float replenishInterval_; //!< The interval in which the munition is replenished.
            unsigned int replenishMunitionAmount_; //!< The amount by which it is replenished.

        private:
            void replenish();
            void initializeTimer();

            Timer replenishingTimer_; //!< Timer to do the replenishing.
    };
}

#endif /* _ReplenishingMunition_H__ */
