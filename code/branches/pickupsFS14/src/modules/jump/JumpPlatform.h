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
 *      Florian Zinggeler
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpPlatform.h
    @brief Declaration of the JumpPlatform class.
*/

#ifndef _JumpPlatform_H__
#define _JumpPlatform_H__

#include "jump/JumpPrereqs.h"

#include "worldentities/MobileEntity.h"

namespace orxonox
{
    class _JumpExport JumpPlatform : public MobileEntity
    {
        public:
            JumpPlatform(Context* context);


        private:
            Model* model;
            MovableEntity* movableEntity;

    };
}

#endif /* _JumpPlatform_H__ */
