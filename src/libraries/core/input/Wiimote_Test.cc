/*
 * WiimoteTest.cpp
 *
 *  Created on: Oct 21, 2013
 *      Author: georgr
 */

#include "/usr/tardis/home-itet-ab/georgr-extra-0/orxonox/wiimote/src/libraries/core/input/Wiimote_Test.h"
#include "core/CoreIncludes.h"
#include "core/command/ConsoleCommand.h"
#include "InputState.h"
#include </usr/tardis/home-itet-ab/georgr-extra-0/orxonox/wiimote/src/orxonox/OrxonoxPrereqs.h>
#include "InputManager.h"
#include </usr/tardis/home-itet-ab/georgr-extra-0/orxonox/wiimote/src/libraries/util/UtilPrereqs.h>
#include </usr/tardis/home-itet-ab/georgr-extra-0/orxonox/wiimote/src/libraries/util/ScopedSingletonManager.h>


namespace orxonox
{
	ManageScopedSingleton(Wiimote_Test, ScopeID::Root, false);

	Wiimote_Test::Wiimote_Test()
	{
		RegisterObject(Wiimote_Test);

	}

	Wiimote_Test::~Wiimote_Test() {
		// TODO Auto-generated destructor stub
	}

	void Wiimote_Test::tick(float dt)
	{
		IntVector2 abs(10,50);
		IntVector2 rel(50, 20);
		IntVector2 clippingSize(1680, 1050);
		InputManager * myIM = &(InputManager::getInstance());
		InputState * gameState = myIM->getState("game");
		gameState->mouseMoved(abs, rel, clippingSize);
		orxout()<<"I'm a huge faggot";
	}
}
