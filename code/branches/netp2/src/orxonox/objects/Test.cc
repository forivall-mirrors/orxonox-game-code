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
#include "network/NetworkFunction.h"
#include "Test.h"

namespace orxonox
{
	CreateFactory ( Test );
  
  SetConsoleCommand(Test, printV1, true).accessLevel(AccessLevel::User);
  SetConsoleCommand(Test, printV2, true).accessLevel(AccessLevel::User);
  SetConsoleCommand(Test, printV3, true).accessLevel(AccessLevel::User);
  SetConsoleCommand(Test, printV4, true).accessLevel(AccessLevel::User);
  SetConsoleCommand(Test, call, true).accessLevel(AccessLevel::User);
  
  
  //void=* aaaaa = copyPtr<sizeof(&Test::printV1)>( &NETWORK_FUNCTION_POINTER, &Test::printV1 );
  //void* NETWORK_FUNCTION_TEST_B = memcpy(&NETWORK_FUNCTION_POINTER, &a, sizeof(a));
//   NetworkFunctionBase* NETWORK_FUNCTION_TEST_C = new NetworkFunctionStatic( createFunctor(&Test::printV1), "bla", NETWORK_FUNCTION_POINTER );
  
  registerStaticNetworkFunction( &Test::printV1 );
  registerMemberNetworkFunction( Test, checkU1 );
  
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
		SetConfigValue ( u1, 1 )/*.callback ( this, &Test::checkV1 )*/;
    SetConfigValue ( u2, 2 )/*.callback ( this, &Test::checkV2 )*/;
    SetConfigValue ( u3, 3 )/*.callback ( this, &Test::checkV3 )*/;
    SetConfigValue ( u4, 4 )/*.callback ( this, &Test::checkV4 )*/;
    
    SetConfigValue ( s1, 1 )/*.callback ( this, &Test::checkV1 )*/;
    SetConfigValue ( s2, 2 )/*.callback ( this, &Test::checkV2 )*/;
    SetConfigValue ( s3, 3 )/*.callback ( this, &Test::checkV3 )*/;
    SetConfigValue ( s4, 4 )/*.callback ( this, &Test::checkV4 )*/;
	}


  void Test::registerVariables()
  {
    registerVariable ( u1, variableDirection::toclient, new NetworkCallback<Test> ( this, &Test::checkU1 ));
    registerVariable ( u2, variableDirection::toserver, new NetworkCallback<Test> ( this, &Test::checkU2 ));
    registerVariable ( u3, variableDirection::serverMaster, new NetworkCallback<Test> ( this, &Test::checkU3 ), true );
    registerVariable ( u4, variableDirection::clientMaster, new NetworkCallback<Test> ( this, &Test::checkU4 ), true );
    
    registerVariable ( s1, variableDirection::toclient, new NetworkCallback<Test> ( this, &Test::checkS1 ));
    registerVariable ( s2, variableDirection::toserver, new NetworkCallback<Test> ( this, &Test::checkS2 ));
    registerVariable ( s3, variableDirection::serverMaster, new NetworkCallback<Test> ( this, &Test::checkS3 ), true );
    registerVariable ( s4, variableDirection::clientMaster, new NetworkCallback<Test> ( this, &Test::checkS4 ), true );
  }
  
  void Test::call(unsigned int clientID)
  {
    callStaticNetworkFunction( &Test::printV1, clientID );
    callStaticNetworkFunction( &Test::printV1, clientID );
  }
  
  void Test::tick(float dt)
  {
    if(!Core::isMaster())
      callMemberNetworkFunction( Test, checkU1, this->getObjectID(), 0 );
//       callMemberNetworkFunction( &Test::printV1, this->getObjectID(), 0);
  }
  
  void Test::checkU1(){ COUT(1) << "U1 changed: " << u1 << std::endl; }
  void Test::checkU2(){ COUT(1) << "U2 changed: " << u2 << std::endl; }
  void Test::checkU3(){ COUT(1) << "U3 changed: " << u3 << std::endl; }
  void Test::checkU4(){ COUT(1) << "U4 changed: " << u4 << std::endl; }

  void Test::checkS1(){ COUT(1) << "S1 changed: " << s1 << std::endl; }
  void Test::checkS2(){ COUT(1) << "S2 changed: " << s2 << std::endl; }
  void Test::checkS3(){ COUT(1) << "S3 changed: " << s3 << std::endl; }
  void Test::checkS4(){ COUT(1) << "S4 changed: " << s4 << std::endl; }

}
