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

/**
    @file JumpPlatformFake.cc
    @brief Implementation of the JumpPlatform class.
*/

#include "JumpPlatformFake.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"

#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpPlatformFake);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpPlatformFake::JumpPlatformFake(Context* context) : JumpPlatform(context)
    {
        RegisterObject(JumpPlatformFake);
    }

    /**
    @brief
        Destructor.
    */
    JumpPlatformFake::~JumpPlatformFake()
    {

    }

    //xml port for loading sounds
    void JumpPlatformFake::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPlatformFake, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
    void JumpPlatformFake::tick(float dt)
    {
        SUPER(JumpPlatformFake, tick, dt);
    }

}
