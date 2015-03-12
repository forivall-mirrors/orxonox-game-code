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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpPlatformDisappear.cc
    @brief This platform disappears after contact with the figure.
*/

#include "JumpPlatformDisappear.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"

#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpPlatformDisappear);

    JumpPlatformDisappear::JumpPlatformDisappear(Context* context) : JumpPlatform(context)
    {
        RegisterObject(JumpPlatformDisappear);

        setProperties(true);
    }

    JumpPlatformDisappear::~JumpPlatformDisappear()
    {

    }

    void JumpPlatformDisappear::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPlatformDisappear, XMLPort, xmlelement, mode);
    }

    void JumpPlatformDisappear::tick(float dt)
    {
        SUPER(JumpPlatformDisappear, tick, dt);
    }

    void JumpPlatformDisappear::setProperties(bool active)
    {
        active_ = active;
    }

    bool JumpPlatformDisappear::isActive()
    {
        return active_;
    }

    void JumpPlatformDisappear::touchFigure()
    {
        if (isActive())
        {
            figure_->JumpFromPlatform(this);
            active_ = false;
        }
    }
}
