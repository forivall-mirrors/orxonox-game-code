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

#ifndef _LaserMunition_H__
#define _LaserMunition_H__

#include "OrxonoxPrereqs.h"
#include "ReplenishingMunition.h"

namespace orxonox
{
    class _OrxonoxExport LaserMunition : public ReplenishingMunition
    {
        public:
            LaserMunition(BaseObject* creator);
            virtual ~LaserMunition() {}
    };
}

#endif /* _LaserMunition_H__ */
