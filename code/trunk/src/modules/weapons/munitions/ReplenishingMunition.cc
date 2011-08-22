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
    @brief Implementation of the ReplenishingMunition class.
*/

#include "ReplenishingMunition.h"

#include "core/CoreIncludes.h"
#include "core/command/Executor.h"

namespace orxonox
{
    CreateFactory(ReplenishingMunition);

    ReplenishingMunition::ReplenishingMunition(BaseObject* creator) : Munition(creator)
    {
        RegisterObject(ReplenishingMunition);

        this->replenishInterval_ = 1.0f;
        this->replenishMunitionAmount_ = 1;

        // Use the timer to initialize itself after the first tick (because the real values for
        // replenishIntervall_ and replenishMunitionAmount_ will be set in the constructor of the
        // inheriting class, which comes after this constructor)
        this->replenishingTimer_.setTimer(0.0f, false, createExecutor(createFunctor(&ReplenishingMunition::initializeTimer, this)));
    }

    void ReplenishingMunition::initializeTimer()
    {
        // Initialize the timer
        this->replenishingTimer_.setTimer(this->replenishInterval_, true, createExecutor(createFunctor(&ReplenishingMunition::replenish, this)));
    }

    void ReplenishingMunition::replenish()
    {
        // Make a temporary copy of bAllowMunitionRefilling_, because this might be disallowed in every
        // case except the internal munition replenishing
        bool temp = this->bAllowMunitionRefilling_;
        this->bAllowMunitionRefilling_ = true;

        // Replenish munition
        this->addMunition(this->replenishMunitionAmount_);

        // Write back the temporary value
        this->bAllowMunitionRefilling_ = temp;
    }
}
