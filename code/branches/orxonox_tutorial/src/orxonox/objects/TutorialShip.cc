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
 *      Oliver 'greenman' Scheuss, Reto '1337' Grieder
 *   Co-authors:
 *      ...
 *
 */

// for precompiled header files. Has to be first!
#include "OrxonoxStableHeaders.h"
// always include this class's header file first so that it compiles on its own too.
#include "TutorialShip.h"

// Additional includes
#include <OgreSceneNode.h>
#include "util/Convert.h"
#include "util/Debug.h"
#include "util/Math.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/input/InputManager.h"
#include "core/XMLPort.h"

namespace orxonox
{
    SetConsoleCommand(TutorialShip, fire, true);

    TutorialShip::TutorialShip()
    {
        RegisterObject(TutorialShip);

        // reset variables
    }

    bool TutorialShip::create()
    {
        return true;
    }

    TutorialShip::~TutorialShip()
    {
    }
    
    void TutorialShip::setConfigValues()
    {
        //SetConfigValue(bInvertYAxis_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = ship down).");
        //SetConfigValue(reloadTime_, 0.125).description("The reload time of the weapon in seconds");
        //SetConfigValue(testvector_, Vector3()).description("asdfblah");
    }

    void TutorialShip::registerAllVariables()
    {
    }
    
    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void TutorialShip::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TutorialShip, XMLPort, xmlelement, mode);

        //XMLPortParam(SpaceShip, "camera", setCamera, getCamera, xmlelement, mode);
    }

    
    void TutorialShip::tick(float dt)
    {
        SUPER(TutorialShip, tick, dt);
    }

    void TutorialShip::fire()
    {
    }
}
