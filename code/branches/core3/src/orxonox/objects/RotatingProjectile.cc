/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
#include "RotatingProjectile.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include <OgreBillboard.h>

namespace orxonox
{
    CreateFactory(RotatingProjectile);

    RotatingProjectile::RotatingProjectile(SpaceShip* owner) : BillboardProjectile(owner)
    {
        RegisterObject(RotatingProjectile);

        this->time_ = 0;

        if (/*this->owner_*/true)
        {
            this->rotatingBillboard1_.setBillboardSet("Examples/Flare", ColourValue(1.0, 0.0, 0.0), 1);
            this->rotatingBillboard2_.setBillboardSet("Examples/Flare", ColourValue(1.0, 0.0, 0.0), 1);

            this->rotatingNode1_ = this->getNode()->createChildSceneNode(this->getName() + "rotating1", Vector3(0, 50, 0));
            this->rotatingNode2_ = this->getNode()->createChildSceneNode(this->getName() + "rotating2", Vector3(0, -50, 0));
            this->rotatingNode1_->attachObject(this->rotatingBillboard1_.getBillboardSet());
            this->rotatingNode2_->attachObject(this->rotatingBillboard2_.getBillboardSet());
            this->rotatingNode1_->scale(0.3, 0.3, 0.3);
            this->rotatingNode2_->scale(0.3, 0.3, 0.3);
        }

        this->setConfigValues();
    }

    RotatingProjectile::~RotatingProjectile()
    {
    }

    void RotatingProjectile::setConfigValues()
    {
        SetConfigValue(colour_, ColourValue(1.0, 0.0, 0.0)).callback(this, &RotatingProjectile::colourChanged);
    }

    void RotatingProjectile::colourChanged()
    {
        if (this->isInitialized())
        {
            this->rotatingBillboard1_.getBillboardSet()->getBillboard(0)->setColour(this->colour_);
            this->rotatingBillboard2_.getBillboardSet()->getBillboard(0)->setColour(this->colour_);
        }
    }

    void RotatingProjectile::tick(float dt)
    {
        if (this->isActive())
        {
            this->time_ += dt;

            this->rotatingNode1_->setPosition(0, 50 * sin(this->time_ * 20), 50 * cos(this->time_ * 20));
            this->rotatingNode2_->setPosition(0, -50 * sin(this->time_ * 20), -50 * cos(this->time_ * 20));
        }

        Projectile::tick(dt);
//        SUPER(RotatingProjectile, tick, dt);
    }

    void RotatingProjectile::changedVisibility()
    {
//        BillboardProjectile::changedVisibility();
        SUPER(RotatingProjectile, changedVisibility);
        this->rotatingBillboard1_.setVisible(this->isVisible());
        this->rotatingBillboard2_.setVisible(this->isVisible());
    }
}
