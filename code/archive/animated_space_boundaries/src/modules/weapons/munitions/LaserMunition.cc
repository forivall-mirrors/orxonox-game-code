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
 *      ...
 *
 */

#include "LaserMunition.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(LaserMunition);

    LaserMunition::LaserMunition(BaseObject* creator) : ReplenishingMunition(creator)
    {
        RegisterObject(LaserMunition);

        this->maxMunitionPerMagazine_ = 20;
        this->maxMagazines_ = 1;
        this->magazines_ = 1;

        this->bUseSeparateMagazines_ = false;
        this->bStackMunition_ = true;

        this->bAllowMunitionRefilling_ = true;
        this->bAllowMultiMunitionRemovementUnderflow_ = true;

        this->replenishIntervall_ = 0.5f;
        this->replenishMunitionAmount_ = 1;
    }
}
