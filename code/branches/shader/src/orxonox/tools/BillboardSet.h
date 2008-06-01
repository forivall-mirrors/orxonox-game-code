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

#ifndef _BillboardSet_H__
#define _BillboardSet_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include <OgreBillboardSet.h>
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport BillboardSet
    {
        public:
            BillboardSet();
            ~BillboardSet();
            void setBillboardSet(const std::string& file, const ColourValue& colour = ColourValue(1.0, 1.0, 1.0), int count = 1, const Vector3& position = Vector3::ZERO);

            inline Ogre::BillboardSet* getBillboardSet()
                { return this->billboardSet_; }

            inline const std::string& getName() const
                { return this->billboardSet_->getName(); }

        private:
            static unsigned int billboardSetCounter_s;
            Ogre::BillboardSet* billboardSet_;
    };
}

#endif /* _BillboardSet_H__ */
