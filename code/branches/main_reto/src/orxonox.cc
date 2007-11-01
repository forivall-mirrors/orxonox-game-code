/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */


#include "orxonox.h"


void Orxonox::go(void)
{
	if (!setup())
		return;

	mTimer = new Timer();

	unsigned long lastTime = mTimer->getMilliseconds();

	while (true)
	{
		//Pump messages in all registered RenderWindow windows
		WindowEventUtilities::messagePump();

		mOgre->getRoot()->renderOneFrame();

		if (!mRunMgr->tick(mTimer->getMilliseconds(),
            (mTimer->getMilliseconds() - lastTime) / 1000.0))
			break;
		lastTime = mTimer->getMilliseconds();
	}

	// clean up
	destroy();
}


bool Orxonox::setup(void)
{
	// create new 3D ogre render engine
	mOgre = new OgreControl();
	mOgre->initialise();

	mRunMgr = new RunManager(mOgre);

	return true;
}


void Orxonox::destroy()
{
	if (mTimer)
		delete mTimer;
	if (mRunMgr)
		delete mRunMgr;
	if (mOgre)
		delete mOgre;
}
