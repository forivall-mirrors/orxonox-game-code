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

#include "BillboardSet.h"

#include <cassert>
#include <string>
#include <sstream>
#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

#include "util/Convert.h"
#include "util/Math.h"
#include "core/GameMode.h"

namespace orxonox
{
    unsigned int BillboardSet::billboardSetCounter_s = 0;

    BillboardSet::BillboardSet()
    {
        this->billboardSet_ = 0;
    }

    BillboardSet::~BillboardSet()
    {
        this->destroyBillboardSet();
    }

    void BillboardSet::setBillboardSet(Ogre::SceneManager* scenemanager, const std::string& file, int count)
    {
        this->setBillboardSet(scenemanager, file, Vector3::ZERO, count);
    }

    void BillboardSet::setBillboardSet(Ogre::SceneManager* scenemanager, const std::string& file, const ColourValue& colour, int count)
    {
        this->setBillboardSet(scenemanager, file, colour, Vector3::ZERO, count);
    }

    void BillboardSet::setBillboardSet(Ogre::SceneManager* scenemanager, const std::string& file, const Vector3& position, int count)
    {
        assert(scenemanager);
        this->destroyBillboardSet();

        try
        {
            if (GameMode::showsGraphics())
            {
                this->billboardSet_ = scenemanager->createBillboardSet("Billboard" + multi_cast<std::string>(BillboardSet::billboardSetCounter_s++), count);
                this->billboardSet_->createBillboard(position);
                this->billboardSet_->setMaterialName(file);
            }
        }
        catch (...)
        {
            COUT(1) << "Error: Couln't load billboard \"" << file << '"' << std::endl;
            this->billboardSet_ = 0;
        }

        this->scenemanager_ = scenemanager;
    }

    void BillboardSet::setBillboardSet(Ogre::SceneManager* scenemanager, const std::string& file, const ColourValue& colour, const Vector3& position, int count)
    {
        assert(scenemanager);
        this->destroyBillboardSet();

        try
        {
            if (GameMode::showsGraphics())
            {
                this->billboardSet_ = scenemanager->createBillboardSet("Billboard" + multi_cast<std::string>(BillboardSet::billboardSetCounter_s++), count);
                this->billboardSet_->createBillboard(position, colour);
                this->billboardSet_->setMaterialName(file);
            }
        }
        catch (...)
        {
            COUT(1) << "Error: Couln't load billboard \"" << file << '"' << std::endl;
            this->billboardSet_ = 0;
        }

        this->scenemanager_ = scenemanager;
    }

    void BillboardSet::destroyBillboardSet()
    {
        if (this->billboardSet_ && this->scenemanager_)
            this->scenemanager_->destroyBillboardSet(this->billboardSet_);
        this->billboardSet_ = 0;
    }

    const std::string& BillboardSet::getName() const
    {
        if (this->billboardSet_)
            return this->billboardSet_->getName();
        else
            return BLANKSTRING;
    }

    void BillboardSet::setVisible(bool visible)
    {
        if (this->billboardSet_)
            this->billboardSet_->setVisible(visible);
    }

    bool BillboardSet::getVisible() const
    {
        if (this->billboardSet_)
            return this->billboardSet_->getVisible();
        else
            return false;
    }

    void BillboardSet::setColour(const ColourValue& colour)
    {
        if (this->billboardSet_)
        {
            for (int i = 0; i < this->billboardSet_->getNumBillboards(); ++i)
                this->billboardSet_->getBillboard(i)->setColour(colour);
        }
    }

    const ColourValue& BillboardSet::getColour() const
    {
        if (this->billboardSet_ && this->billboardSet_->getNumBillboards() > 0)
        {
            return this->billboardSet_->getBillboard(0)->getColour();
        }
        else
            return ColourValue::White;
    }

    void BillboardSet::setMaterial(const std::string& material)
    {
        if (this->billboardSet_)
            this->billboardSet_->setMaterialName(material);
    }

    const std::string& BillboardSet::getMaterial() const
    {
        if (this->billboardSet_)
            return this->billboardSet_->getMaterialName();
        else
            return BLANKSTRING;
    }
}
