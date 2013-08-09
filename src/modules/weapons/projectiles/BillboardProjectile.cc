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
    @file BillboardProjectile.h
    @brief Implementation of the BillboardProjectile class.
*/

#include "BillboardProjectile.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "Scene.h"

namespace orxonox
{
    CreateFactory(BillboardProjectile);

    BillboardProjectile::BillboardProjectile(Context* context) : Projectile(context)
    {
        RegisterObject(BillboardProjectile);

        if (GameMode::showsGraphics())
        {
            assert(this->getScene()->getSceneManager()); // getScene() was already checked by WorldEntity

            // Create the billboard.
            this->billboard_.setBillboardSet(this->getScene()->getSceneManager(), "Examples/Flare", ColourValue(0.5f, 0.5f, 0.7f, 0.8f), 1);
            this->attachOgreObject(this->billboard_.getBillboardSet());
        }

        this->setScale(0.2f);
    }

    BillboardProjectile::~BillboardProjectile()
    {
        if (this->isInitialized() && GameMode::showsGraphics() && this->billboard_.getBillboardSet())
            this->detachOgreObject(this->billboard_.getBillboardSet());
    }

    /**
    @brief
        Set the colour of the BillboardProjectile.
    @param colour
        The colour to be set.
    */
    void BillboardProjectile::setColour(const ColourValue& colour)
    {
        this->billboard_.setColour(colour);
    }

    /**
    @brief
        Set the material of the BillboardProjectile.
    @param material
        The material name.
    */
    void BillboardProjectile::setMaterial(const std::string& material)
    {
        this->billboard_.setMaterial(material);
    }

    /**
    @brief
        Is called when the visibility of the BillboardProjectile has changed.
    */
    void BillboardProjectile::changedVisibility()
    {
        SUPER(BillboardProjectile, changedVisibility);

        // Also change the visibility of the billboard.
        this->billboard_.setVisible(this->isVisible());
    }
}
