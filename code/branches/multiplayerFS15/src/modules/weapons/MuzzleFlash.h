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
 *      Hagen Seifert
 *   Co-authors:
 *      ...
 *
 */

/**
    @file MuzzleFlash.h
    @brief Definition of the MuzzleFlash class.
*/

#ifndef _MuzzleFlash_H__
#define _MuzzleFlash_H__

#include "weapons/WeaponsPrereqs.h"

#include "tools/Timer.h"
#include "graphics/Billboard.h"

namespace orxonox
{

    /**
    @brief
        A muzzle flash that destroys itself after some time.
    @author
        Hagen Seifert
    @ingroup Weapons
    @todo
        Remove because it's unused?
    */
    class _WeaponsExport MuzzleFlash : public Billboard
    {
        public:
            MuzzleFlash(Context* context);
            virtual ~MuzzleFlash() {}

        private:
            Timer delayTimer_; //!< Timer to destroy the object.
    };
}

#endif /* _MuzzleFlash_H__ */
