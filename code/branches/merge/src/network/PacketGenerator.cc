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
 *      Dumeni Manatschal, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

/*
* Class generates packets that can be send by enet
* ->don't read this without the class PacketDecoder, since they belong together
*
* Autor: Dumeni Manatschal
*
*/

#include "PacketManager.h"
#include "PacketTypes.h"

#include <iostream>
#include <list>
#include <string>
#include <cstring>


namespace network
{
  PacketGenerator::PacketGenerator() { }

  //following functions create a packet in form of bytestream

  ENetPacket* PacketGenerator::acknowledgement( int state, int reliable )
  {
    COUT(4) << "PacketGenerator: generating new acknowledgement, id: " << state << std::endl;
    ack* ackreq = new ack;
    ackreq->id = ACK;
    ackreq->a = state;

    ENetPacket *packet = enet_packet_create( ackreq , sizeof( *ackreq ), reliable );
    delete ackreq;
    return packet;
  }
  
  ENetPacket* command( int dataLength, void *data, int reliable = ENET_PACKET_FLAG_RELIABLE )
  {
    unsigned char *stream = new unsigned char[dataLength + 2*sizeof(int)];
    if(!stream)
      return NULL;
    packet_id a = COMMAND;
    memcpy(stream, (void*)&a, sizeof(int));
    memcpy((unsigned char *)stream+sizeof(int), (void*)&dataLength, sizeof(int));
    memcpy((unsigned char *)stream+2*sizeof(int), data, dataLength);
    ENetPacket *packet = enet_packet_create(stream, dataLength+2*sizeof(int), reliable);
    delete[] stream; // TODO: we could also tell enet not to copy the data, but to use the exisiting memory
    return packet;
  }

  /*### mouseupdates */
  ENetPacket* PacketGenerator::mousem( double x, double y, int reliable )
  {
    COUT(4) << "PacketGenerator: generating new mouse" << std::endl;
    mouse* mousemove = new mouse;
    mousemove->id = MOUSE;
    mousemove->x = x;
    mousemove->y = y;

    ENetPacket *packet = enet_packet_create( mousemove , sizeof( *mousemove ), reliable );
    delete mousemove;
    return packet;
  }

  /*### keystrikes updates */
  ENetPacket* PacketGenerator::keystrike( char press, int reliable )
  {
    COUT(4) << "PacketGenerator: generating new keyboard" << std::endl;
    keyboard* key = new keyboard;
    key->id = KEYBOARD;
    key->press = press;

    ENetPacket *packet = enet_packet_create( key , sizeof( *key ), reliable );
    delete key;
    return packet;
  }

  /*### chat messages packet */
  ENetPacket* PacketGenerator::chatMessage( const char* message, int reliable )
  {
    int* trans = new int[sizeof(int) + strlen(message) + 1];
    *trans = CHAT;
    //be carefull here, don't forget to allocate the space before using it ;-)
    memcpy( &trans[1], (const void*)message, strlen( message ) + 1);
    ENetPacket *packet = enet_packet_create( trans , sizeof( int ) + strlen( message ) + 1, reliable );
    delete[] trans;
    return packet;
  }

  /*### gamestate packet */
  ENetPacket* PacketGenerator::gstate( GameStateCompressed* states, int reliable )
  {
    //std::cout << "packetgenerator" << std::endl;
    //std::cout << "states->normsize " << states->normsize << std::endl;
    //std::cout << "states->compsize " << states->compsize << std::endl;
    int gid = GAMESTATE; //first assign the correct enet id
    int totalLen = 5*sizeof( int ) + 2*sizeof(bool) + states->compsize; //calculate the total size of the datastream memory
    //std::cout << "totalLen " << totalLen << std::endl;
    //unsigned char *data = (unsigned char*)malloc( totalLen ); //allocate the memory for datastream
    unsigned char *data = new unsigned char[totalLen];
    memcpy( (void*)(data), (const void*)&gid, sizeof( int ) ); //this is the enet id
    memcpy( (void*)(data+sizeof(int)), (const void*)&(states->id), sizeof(int) ); //the GameStateCompressed id
    memcpy( (void*)(data+2*sizeof(int)), (const void*)&(states->compsize), sizeof(int));
    memcpy( (void*)(data+3*sizeof(int)), (const void*)&(states->normsize), sizeof(int));
    memcpy( (void*)(data+4*sizeof(int)), (const void*)&(states->base_id), sizeof(int));
    memcpy( (void*)(data+5*sizeof(int)), (const void*)&(states->diffed), sizeof(bool));
    //place the GameStateCompressed data at the end of the enet datastream
    memcpy( (void*)(data+5*sizeof( int ) + sizeof(bool)), (const void*)&(states->complete), sizeof(bool) );
    memcpy( (void*)(data+5*sizeof( int ) + 2*sizeof(bool)), (const void*)states->data, states->compsize );
    //create an enet packet with the generated bytestream
    COUT(4) << "PacketGenerator generating totalLen " << totalLen << std::endl;
    ENetPacket *packet = enet_packet_create( data , totalLen, reliable );
    delete[] data;
    return packet;
  }

  ENetPacket* PacketGenerator::clid( int classid, std::string classname, int reliable )
  {
    //unsigned char* data = (unsigned char *)malloc(3*sizeof(int)+classname.length()+1);
    unsigned char *data = new unsigned char[3*sizeof(int)+classname.length()+1];
    std::cout << "PacketGenerator: classid: " << classid << ", name: " << classname << std::endl;
    *(int *)data = CLASSID;
    *((int *)data+1) = classname.length()+1;
    *((int *)data+2) = classid;
    memcpy( (void *)(data+3*sizeof(int)), classname.c_str(), classname.length()+1);
    ENetPacket *packet = enet_packet_create( data , 3*sizeof(int)+classname.length()+1, reliable );
    delete[] data;
    return packet;
  }
  
  ENetPacket* PacketGenerator::generateWelcome( int clientID,int shipID, bool allowed , int reliable ){
    welcome *wc = new welcome;
    wc->id = WELCOME;
    wc->clientID = clientID;
    wc->shipID = shipID;
    wc->allowed = true;
    ENetPacket *packet = enet_packet_create( wc, sizeof(welcome), reliable);
    delete wc;
    return packet;
  }
  
  ENetPacket* PacketGenerator::generateConnectRequest( int reliable ){
    connectRequest *con = new connectRequest;
    con->id=CONNECT;
    ENetPacket *packet = enet_packet_create( con, sizeof(connectRequest), reliable);
    delete con;
    return packet;
  }

}
