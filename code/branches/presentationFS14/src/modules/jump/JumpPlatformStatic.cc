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
    @file JumpPlatformStatic.cc
    @brief This is the most simple platform in this minigame.
*/

#include "JumpPlatformStatic.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "gametypes/Gametype.h"
#include "JumpFigure.h"
#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpPlatformStatic);

    JumpPlatformStatic::JumpPlatformStatic(Context* context) : JumpPlatform(context)
    {
        RegisterObject(JumpPlatformStatic);

    }

    JumpPlatformStatic::~JumpPlatformStatic()
    {

    }

    //xml port for loading sounds
    void JumpPlatformStatic::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPlatformStatic, XMLPort, xmlelement, mode);
    }

    void JumpPlatformStatic::tick(float dt)
    {
        SUPER(JumpPlatformStatic, tick, dt);
    }

    void JumpPlatformStatic::touchFigure()
    {
        figure_->JumpFromPlatform(this);
    }
}
