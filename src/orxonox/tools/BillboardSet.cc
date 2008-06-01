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
#include "BillboardSet.h"

#include <sstream>

#include <OgreSceneManager.h>

#include "GraphicsEngine.h"
#include "util/Math.h"

namespace orxonox
{
    unsigned int BillboardSet::billboardSetCounter_s = 0;

    BillboardSet::BillboardSet()
    {
        this->billboardSet_ = 0;
    }

    void BillboardSet::setBillboardSet(const std::string& file, const ColourValue& colour, int count, const Vector3& position)
    {
        std::ostringstream name;
        name << (BillboardSet::billboardSetCounter_s++);
        this->billboardSet_ = GraphicsEngine::getSingleton().getSceneManager()->createBillboardSet("Billboard" + name.str(), count);
        this->billboardSet_->createBillboard(position, colour);
        this->billboardSet_->setMaterialName(file);
    }

    BillboardSet::~BillboardSet()
    {
        if (this->billboardSet_)
            GraphicsEngine::getSingleton().getSceneManager()->destroyBillboardSet(this->billboardSet_);
    }
}
