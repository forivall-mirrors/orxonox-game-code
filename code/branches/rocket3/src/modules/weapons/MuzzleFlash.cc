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
 *      Ich
 *      Dini Mueter
 *
 */

#include "MuzzleFlash.h"

#include "core/CoreIncludes.h"
#include "core/Executor.h"

namespace orxonox
{
    CreateFactory(MuzzleFlash);

    MuzzleFlash::MuzzleFlash(BaseObject* creator) : Billboard(creator)
    {
        RegisterObject(MuzzleFlash);
        this->setScale(0.1f);

        this->delayTimer_.setTimer(0.1f, false, createExecutor(createFunctor(&MuzzleFlash::destroy, this)));
    }
}
