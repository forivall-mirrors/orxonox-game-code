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

#include "OrxonoxStableHeaders.h"
#include "BillboardProjectile.h"

#include <OgreBillboard.h>

#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(BillboardProjectile);

    BillboardProjectile::BillboardProjectile(Weapon* owner) : Projectile(owner)
    {
        RegisterObject(BillboardProjectile);

        this->billboard_.setBillboardSet("Examples/Flare", ColourValue(1.0, 1.0, 0.5), 1);
        this->attachObject(this->billboard_.getBillboardSet());
        this->scale(0.5);
    }

    BillboardProjectile::~BillboardProjectile()
    {
        if (this->isInitialized() && this->owner_)
            this->detachObject(this->billboard_.getBillboardSet());
    }

    void BillboardProjectile::setColour(const ColourValue& colour)
    {
        this->billboard_.getBillboardSet()->getBillboard(0)->setColour(colour);
    }

    void BillboardProjectile::changedVisibility()
    {
        SUPER(BillboardProjectile, changedVisibility);
        this->billboard_.setVisible(this->isVisible());
    }
}
