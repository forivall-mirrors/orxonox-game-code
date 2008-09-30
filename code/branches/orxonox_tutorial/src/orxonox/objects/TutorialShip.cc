/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *                         TUTORIAL
 */

// for precompiled header files. Has to be first!
#include "OrxonoxStableHeaders.h"
// always include this class's header file first so that
// it compiles on its own too.
#include "TutorialShip.h"

// Additional includes
#include "util/Convert.h"
#include "util/Debug.h"
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

        // Display a message in shell/logfile/console
        COUT(3) << "TutorialShip was loaded." << std::endl;
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

    /*** NOT SO IMPORTATANT... ***/

    // run time update method. Gets called every frame with the delta time that
    // has passed since the last frame.
    void TutorialShip::tick(float dt)
    {
        // Also call the tick() method of the base clas.
        SUPER(TutorialShip, tick, dt);
    }

    // virtual function used by back end class SpaceShip.
    float TutorialShip::getReloadTime()
    {
        return this->reloadTime_;
    }

    // Fire a projectile. Delegated to the back end class SpaceShip.
    // Function content is insignificant for the tutorial.
    void TutorialShip::fire()
    {
        SpaceShip::getLocalShip()->doFire();
    }
}
