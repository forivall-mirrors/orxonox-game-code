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

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "network/NetworkFunction.h"
#include "Test.h"
#include "util/MultiType.h"

namespace orxonox
{
    CreateFactory ( Test );
  
    SetConsoleCommand(Test, printV1, true).accessLevel(AccessLevel::User);
    SetConsoleCommand(Test, printV2, true).accessLevel(AccessLevel::User);
    SetConsoleCommand(Test, printV3, true).accessLevel(AccessLevel::User);
    SetConsoleCommand(Test, printV4, true).accessLevel(AccessLevel::User);
    SetConsoleCommand(Test, call, true).accessLevel(AccessLevel::User);
    SetConsoleCommand(Test, call2, true).accessLevel(AccessLevel::User);
  
  
  //void=* aaaaa = copyPtr<sizeof(&Test::printV1)>( &NETWORK_FUNCTION_POINTER, &Test::printV1 );
  //void* NETWORK_FUNCTION_TEST_B = memcpy(&NETWORK_FUNCTION_POINTER, &a, sizeof(a));
//   NetworkFunctionBase* NETWORK_FUNCTION_TEST_C = new NetworkFunctionStatic( createFunctor(&Test::printV1), "bla", NETWORK_FUNCTION_POINTER );
  
    registerStaticNetworkFunction( &Test::printV1 );
    registerMemberNetworkFunction( Test, checkU1 );
    registerMemberNetworkFunction( Test, printBlaBla );
  
    Test* Test::instance_ = 0;

    Test::Test(BaseObject* creator) : BaseObject(creator), Synchronisable(creator)
    {
        assert(instance_==0);
        instance_=this;
        RegisterObject ( Test );
        setConfigValues();
        registerVariables();
        setSyncMode(0x3);
        this->pointer_ = 0;
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
        registerVariable ( u1, VariableDirection::ToClient, new NetworkCallback<Test> ( this, &Test::checkU1 ));
        registerVariable ( u2, VariableDirection::ToServer, new NetworkCallback<Test> ( this, &Test::checkU2 ));
        registerVariable ( u3, Bidirectionality::ServerMaster, new NetworkCallback<Test> ( this, &Test::checkU3 ), true );
        registerVariable ( u4, Bidirectionality::ClientMaster, new NetworkCallback<Test> ( this, &Test::checkU4 ), true );
    
        registerVariable ( s1, VariableDirection::ToClient, new NetworkCallback<Test> ( this, &Test::checkS1 ));
        registerVariable ( s2, VariableDirection::ToServer, new NetworkCallback<Test> ( this, &Test::checkS2 ));
        registerVariable ( s3, Bidirectionality::ServerMaster, new NetworkCallback<Test> ( this, &Test::checkS3 ), true );
        registerVariable ( s4, Bidirectionality::ClientMaster, new NetworkCallback<Test> ( this, &Test::checkS4 ), true );
    
        registerVariable ( pointer_, VariableDirection::ToClient, new NetworkCallback<Test> ( this, &Test::printPointer ) );
    }
  
  void Test::call(unsigned int clientID)
    {
        callStaticNetworkFunction( &Test::printV1, clientID );
        callStaticNetworkFunction( &Test::printV1, clientID );
    }
  
    void Test::call2(unsigned int clientID, std::string s1, std::string s2, std::string s3, std::string s4)
    {
        callMemberNetworkFunction( Test, printBlaBla, this->getObjectID(), clientID, s1, s2, s3, s4, s4 );
    }
  
    void Test::tick(float dt)
    {
    //     std::string str1 = "blub";
    //     //MultiType mt1(std::string("blub"));
    //     MultiType mt1(str1);
    //     uint8_t* mem = new uint8_t[mt1.getNetworkSize()];
    //     uint8_t* temp = mem;
    //     mt1.exportData( temp );
    //     assert( temp-mem == mt1.getNetworkSize() );
    //     MultiType mt2;
    //     temp = mem;
    //     mt2.importData( temp );
    //     assert( temp-mem == mt1.getNetworkSize() );
    //     COUT(0) << mt2 << endl;
    //     if(!Core::isMaster())
    //       call2(0, "bal", "a", "n", "ce");
    //       callMemberNetworkFunction( Test, checkU1, this->getObjectID(), 0 );
    }
    
    void Test::printBlaBla(std::string s1, std::string s2, std::string s3, std::string s4, std::string s5)
    {
        COUT(0) << s1 << s2 << s3 << s4 << s5 << endl;
    }
    
    void Test::checkU1(){ COUT(1) << "U1 changed: " << u1 << std::endl; }
    void Test::checkU2(){ COUT(1) << "U2 changed: " << u2 << std::endl; }
    void Test::checkU3(){ COUT(1) << "U3 changed: " << u3 << std::endl; }
    void Test::checkU4(){ COUT(1) << "U4 changed: " << u4 << std::endl; }

    void Test::checkS1(){ COUT(1) << "S1 changed: " << s1 << std::endl; }
    void Test::checkS2(){ COUT(1) << "S2 changed: " << s2 << std::endl; }
    void Test::checkS3(){ COUT(1) << "S3 changed: " << s3 << std::endl; }
    void Test::checkS4(){ COUT(1) << "S4 changed: " << s4 << std::endl; }
    
    void Test::printPointer(){ CCOUT(1) << "pointer: " << this->pointer_ << endl; }

}
