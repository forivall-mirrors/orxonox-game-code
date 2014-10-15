/*
 * DodgeRace.cc
 *
 *  Created on: Oct 15, 2014
 *      Author: sriedel
 */

#include "DodgeRace.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/command/Executor.h"
#include "core/config/ConfigValueIncludes.h"

#include "gamestates/GSLevel.h"
#include "chat/ChatManager.h"

namespace orxonox
{
	RegisterUnloadableClass(DodgeRace);

	DodgeRace::DodgeRace()
	{
		init();

	}

	void DodgeRace::init()
	{
		level = 0;
		point = 0;
		lives = 1;

	}

	DodgeRace::~DodgeRace() {
		// TODO Auto-generated destructor stub
	}

} /* namespace orxonox */
