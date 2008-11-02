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
 *      Oli Scheuss
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "Test.h"

namespace orxonox
{
	CreateFactory ( Test );

	Test::Test(BaseObject* creator) : BaseObject(creator), Synchronisable(creator)
	{
		RegisterObject ( Test );
                setConfigValues();
                registerVariables();
		setObjectMode(0x3);
	}

	Test::~Test()
	{

	}

	void Test::setConfigValues()
	{
		SetConfigValue ( v1, 1 ).callback ( this, &Test::checkV1 );
                SetConfigValue ( v2, 2 ).callback ( this, &Test::checkV2 );
                SetConfigValue ( v3, 3 ).callback ( this, &Test::checkV3 );
	}


	void Test::registerVariables()
	{
		REGISTERDATA ( v1,direction::toclient, new NetworkCallback<Test> ( this, &Test::checkV1 ) );
                REGISTERDATA ( v2,direction::toserver, new NetworkCallback<Test> ( this, &Test::checkV2 ) );
		REGISTERDATA ( v3,direction::bidirectional, new NetworkCallback<Test> ( this, &Test::checkV3 ) );
	}

        void Test::checkV1(){
                COUT(1) << "V1 changed: " << v1 << std::endl;
        }

        void Test::checkV2(){
                COUT(1) << "V2 changed: " << v2 << std::endl;
        }

        void Test::checkV3(){
                COUT(1) << "V3 changed: " << v3 << std::endl;
        }


}
