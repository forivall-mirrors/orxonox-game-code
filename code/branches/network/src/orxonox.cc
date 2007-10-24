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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

#include "tnl.h" 
#include "tnlEventConnection.h" 
#include "tnlNetInterface.h" 
#include "tnlRPC.h" 
#include <stdio.h> 
bool gQuit = false; // a flag used when the client wants to quit. using namespace TNL; // make sure we can simply use the TNL classes. 

class SimpleEventConnection : public EventConnection
{
    typedef EventConnection Parent;

public:
    // Let the network system know this is a valid network connection.
    TNL_DECLARE_NETCONNECTION(SimpleEventConnection);

    // declare the client to server message
    TNL_DECLARE_RPC(rpcMessageClientToServer, (const char *theMessageString));

    // declare the server to client message
    TNL_DECLARE_RPC(rpcMessageServerToClient, (const char *theMessageString));
};

TNL_IMPLEMENT_NETCONNECTION(SimpleEventConnection, NetClassGroupGame, true);

TNL_IMPLEMENT_RPC(SimpleEventConnection, rpcMessageClientToServer, (const char *messageString), NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 0)
{
    // display the message the client sent
    printf("Got message from client: %s\n", messageString);
    // send a hello world back to the client.
    rpcMessageServerToClient("Hello, World!");
}

TNL_IMPLEMENT_RPC(SimpleEventConnection, rpcMessageServerToClient, (const char *messageString), NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirServerToClient, 0)
{
    // display the message the server sent
    printf("Got a message from server: %s\n", messageString);

    // once the client has heard back from the server, it should quit.
    gQuit = true;
}

int main(int argc, const char **argv)
{
    if(argc != 3)
    {
        printf("usage: simpletnltest <-server|-client> <connectAddress>");
        return 1;
    }
    bool isClient = !strcmp(argv[1], "-client");

    // convert the command-line address into TNL address form
    Address cmdAddress(argv[2]);

    RefPtr<NetInterface> theNetInterface;
    if(isClient)
    {
        Address bindAddress(IPProtocol, Address::Any, 0);

        // create a new NetInterface bound to any interface, any port (0)
        theNetInterface = new NetInterface(bindAddress);
        
        // create a new SimpleEventConnection and tell it to connect to the
        // server at cmdAddress.
        SimpleEventConnection *newConnection = new SimpleEventConnection;
        newConnection->connect(theNetInterface, cmdAddress);

        // post an RPC, to be executed when the connection is established
        newConnection->rpcMessageClientToServer("Hello??");
    }
    else
    {
        // create a server net interface, bound to the cmdAddress
        theNetInterface = new NetInterface(cmdAddress);

        // notify the NetInterface that it can allow connections
        theNetInterface->setAllowsConnections(true);
    }

    // now just loop, processing incoming packets and sending outgoing packets
    // until the global quit flag is set.
    while(!gQuit)
    {
         theNetInterface->checkIncomingPackets();
         theNetInterface->processConnections();
         Platform::sleep(1);
    }
    return 0;
}
