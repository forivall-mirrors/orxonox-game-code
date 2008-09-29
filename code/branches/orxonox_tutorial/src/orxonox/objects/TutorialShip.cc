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
// always include this class's header file first so that
// it compiles on its own too.
#include "TutorialShip.h"

// Additional includes
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
    // Specify a console command that can be used in
    // the shell or as key binding.
    SetConsoleCommand(TutorialShip, fire, true)
        .keybindMode(KeybindMode::OnHold);

    // Make sure we can create an object of this class by XML
    CreateFactory(TutorialShip);

    // Constructor
    TutorialShip::TutorialShip()
    {
        RegisterObject(TutorialShip);

        // reset variables
        this->hasSpecialEffects_ = false;

        // set config values
        this->setConfigValues();
    }

    // Destructor
    TutorialShip::~TutorialShip()
    {
    }

    // Sets the configurable member variables.
    // They can be found later in orxonox.ini directly.
    void TutorialShip::setConfigValues()
    {
        SetConfigValue(reloadTime_, 0.125)
            .description("The reload time of the weapon in seconds");
    }
    
    // Called when loading an object of this class with XML
    // You don't have to know what exactly xmlelement is.
    // And mode is not important yet (load/save).
    void TutorialShip::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        // Load our parameter "specialEffects". Case sensitive!
        XMLPortParam(TutorialShip, "specialEffects", setSpecialEffects,
            hasSpecialEffects, xmlelement, mode);

        // Calls SpaceShip::XMLPort so that the SpaceShip XML parameters
        // are loaded too.
        SUPER(TutorialShip, XMLPort, xmlelement, mode);
    }

    // XML save function. Also used by back end class SpaceShip
    // to show or hide the special effects.
    bool TutorialShip::hasSpecialEffects()
    {
        return this->hasSpecialEffects_;
    }

    // XML load function. Called by the XML macro above.
    void TutorialShip::setSpecialEffects(bool value)
    {
        this->hasSpecialEffects_ = value;
    }

    // virtual function used by back end class SpaceShip.
    float TutorialShip::getReloadTime()
    {
        return this->reloadTime_;
    }

    // run time update method. Gets called every frame with the delta time that
    // has passed since the last frame.
    void TutorialShip::tick(float dt)
    {
        // Also call the tick() method of the base clas.
        SUPER(TutorialShip, tick, dt);
    }

    // Fire a projectile. Delegated to the back end class SpaceShip.
    // Function content is insignificant for the tutorial.
    void TutorialShip::fire()
    {
        SpaceShip::getLocalShip()->doFire();
    }
}
