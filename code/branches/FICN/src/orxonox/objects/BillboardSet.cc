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

#include <sstream>

#include <OgreSceneManager.h>

#include "../Orxonox.h"
#include "misc/Vector3.h"
#include "misc/ColourValue.h"

#include "BillboardSet.h"

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
        this->billboardSet_ = Orxonox::getSingleton()->getSceneManager()->createBillboardSet("Billboard" + name.str(), count);
        this->billboardSet_->createBillboard(position, colour);
        this->billboardSet_->setMaterialName(file);
    }

    BillboardSet::~BillboardSet()
    {
        if (this->billboardSet_)
            Orxonox::getSingleton()->getSceneManager()->destroyBillboardSet(this->billboardSet_);
    }
}
