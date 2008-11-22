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
#include "core/ConsoleCommand.h"
#include "Test.h"

namespace orxonox
{
	CreateFactory ( Test );
  
  SetConsoleCommand(Test, printV1, true).accessLevel(AccessLevel::User);
  SetConsoleCommand(Test, printV2, true).accessLevel(AccessLevel::User);
  SetConsoleCommand(Test, printV3, true).accessLevel(AccessLevel::User);
  SetConsoleCommand(Test, printV4, true).accessLevel(AccessLevel::User);
  
  Test* Test::instance_ = 0;

	Test::Test(BaseObject* creator) : BaseObject(creator), Synchronisable(creator)
	{
    assert(instance_==0);
    instance_=this;
		RegisterObject ( Test );
    setConfigValues();
    registerVariables();
		setObjectMode(0x3);
	}

	Test::~Test()
	{
    instance_=0;
	}

	void Test::setConfigValues()
	{
		SetConfigValue ( v1, 1 )/*.callback ( this, &Test::checkV1 )*/;
    SetConfigValue ( v2, 2 )/*.callback ( this, &Test::checkV2 )*/;
    SetConfigValue ( v3, 3 )/*.callback ( this, &Test::checkV3 )*/;
    SetConfigValue ( v4, 4 )/*.callback ( this, &Test::checkV4 )*/;
	}


	void Test::registerVariables()
	{
		registerVariable ( v1, variableDirection::toclient, new NetworkCallback<Test> ( this, &Test::checkV1 ));
    registerVariable ( v2, variableDirection::toserver, new NetworkCallback<Test> ( this, &Test::checkV2 ));
		registerVariable ( v3, variableDirection::serverMaster, new NetworkCallback<Test> ( this, &Test::checkV3 ), true );
    registerVariable ( v4, variableDirection::clientMaster, new NetworkCallback<Test> ( this, &Test::checkV4 ), true );
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
  
  void Test::checkV4(){
    COUT(1) << "V4 changed: " << v4 << std::endl;
  }


}
