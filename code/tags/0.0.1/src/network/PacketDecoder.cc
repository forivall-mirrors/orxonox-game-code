/*
*   ORXONOX - the hottest 3D action shooter ever to exist
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
* Class contains functions to determine and decode incomming packages
* ->don't read this without the class PacketGenerator, since they belong together
*
* Autor: Dumeni Manatschal
*
*/

#include <iostream>

#include "PacketTypes.h"
#include "PacketManager.h"

namespace network
{
  using namespace std;

  PacketDecoder::PacketDecoder(){}

  PacketDecoder::~PacketDecoder(){}

  //call this function out of an instance of PacketDecoder
  //it will determine the type id and call the right decode function
  bool PacketDecoder::elaborate( ENetPacket* packet, int clientId )
  {
    int client = clientId;
    cout << "clientId: " << client << endl; //control cout, not important, just debugging info
    int id = (int)*packet->data; //the first 4 bytes are always the enet packet id
    std::cout << "packet id: " << id << std::endl;
    std::cout << "packet size inside packetdecoder: " << packet->dataLength << std::endl;
    switch( id ) {
  case ACK:
    acknowledgement( packet, clientId );
    return true;
    break;
  case MOUSE:
    mousem( packet, clientId );
    return true;
    break;
  case KEYBOARD:
    keystrike( packet, clientId );
    return true;
    break;
  case CHAT:
    chatMessage( packet, clientId );
    return true;
    break;
  case GAMESTATE:
    gstate( packet );
    return true;
    break;
  case CLASSID:
    clid(packet);
    return true;
    break;
    }
    return false;
  }

  //following are the decode functions for the data of the packets

  void PacketDecoder::acknowledgement( ENetPacket* packet, int clientId )
  {
    ack* a = new ack;
    *a = *(ack*)packet->data; //press pattern of ack on new data


    std::cout << "got ack id: " << a->id << std::endl;
    processAck( a, clientId ); //debug info

    //clean memory
    enet_packet_destroy( packet );
  }

  void PacketDecoder::mousem( ENetPacket* packet, int clientId )
  {
    mouse* mouseMove = new mouse;
    //copy data of packet->data to new struct
    *mouseMove = *(mouse*)packet->data;

    //clean memory
    enet_packet_destroy( packet );

    printMouse( mouseMove ); //debug info
  }

  void PacketDecoder::keystrike( ENetPacket* packet, int clientId )
  {
    keyboard* key = new keyboard;
    *key = *(keyboard*)packet->data; //see above

    //clean memory
    enet_packet_destroy( packet );

    printKey( key ); //debug info

  }

  void PacketDecoder::chatMessage( ENetPacket* packet, int clientId )
  {
    chat* chatting = new chat;
    chatting->id = (int)*packet->data; //first copy id into new struct
    //since the chat message is a char*, allocate the memory needed
    char* reserve = new char[packet->dataLength-4];
    //copy the transmitted bytestream into the new generated char*,
    //note the lenght of the message is represented as "packet->dataLength-sizeof( int )"
    memcpy( &reserve[0], packet->data+sizeof(int), packet->dataLength-sizeof(int) );
    //put pointer of chatting struct to the begining of the new generated char*
    chatting->message = reserve;

    //clean memory
    enet_packet_destroy( packet );

    processChat( chatting, clientId ); //debug info
  }

  void PacketDecoder::gstate( ENetPacket* packet )
  {
    GameStateCompressed* currentState = new GameStateCompressed;
    //since it's not alowed to use void* for pointer arithmetic
    unsigned char* data = (unsigned char*)packet->data;
    //copy the GameStateCompressed id into the struct, which is located at second place data+sizeof( int )
    //memcpy( (void*)&(currentState->id), (const void*)(data+sizeof( int )), sizeof( int ) );
    currentState->id = (int)*(data+sizeof(int));
    std::cout << "id: " << currentState->id << std::endl;
    //copy the size of the GameStateCompressed compressed data into the new GameStateCompressed struct, located at 3th
    //position of the data stream, data+2*sizeof( int )
    memcpy( (void*)&(currentState->compsize), (const void*)(data+2*sizeof( int )), sizeof( int) );
    //currentState->compsize = (int)*(data+2*sizeof(int));
    std::cout << "compsize: " << currentState->compsize << std::endl;
    //size of uncompressed data
    memcpy( (void*)&(currentState->normsize), (const void*)(data+3*sizeof( int )), sizeof( int ) );
    //currentState->normsize = (int)*(data+3*sizeof(int));
    std::cout << "normsize. " << currentState->normsize << std::endl;
    //since the packetgenerator was changed, due to a new parameter, change this function too
    memcpy( (void*)&(currentState->diffed), (const void*)(data+4*sizeof(int)), sizeof(bool));
    //currentState->diffed = (bool)*(data+4*sizeof(int));
    std::cout << "diffed: " << currentState->diffed << std::endl;
    //since data is not allocated, because it's just a pointer, allocate it with size of gamestatedatastream
    currentState->data = (unsigned char*)(malloc( currentState->compsize ));
    if(currentState->data==NULL)
      std::cout << "memory leak" << std::endl;
    //copy the GameStateCompressed data
    //std::cout << "packet size (enet): " << packet->dataLength << std::endl;
    //std::cout << "totallen: " << 4*sizeof(int)+sizeof(bool)+currentState->compsize << std::endl;
    memcpy( (void*)(currentState->data), (const void*)(data+4*sizeof( int ) + sizeof(bool)), currentState->compsize );

    //clean memory
    enet_packet_destroy( packet );
    //run processGameStateCompressed
    //TODO: not yet implemented!
    processGamestate(currentState);
  }

  void PacketDecoder::clid( ENetPacket *packet)
  {
    classid* cid = new classid;
    cid->length = ((classid*)(packet->data))->length;
    cid->id = ((classid *)(packet->data))->id;
    cid->clid = ((classid *)(packet->data))->clid;
    cid->message = (const char *)malloc(cid->length);
    void *data  = (void *)cid->message;
    memcpy(data, (const void*)(packet->data+3*sizeof(int)), cid->length);
    std::cout << "classid: " << cid->clid << ", name: " << cid->message << std::endl;
    enet_packet_destroy( packet );
    processClassid(cid);
  }


  // now the data processing functions:

  void PacketDecoder::processChat( chat *data, int clientId)
  {
    printChat(data, clientId);
  }

  void PacketDecoder::processGamestate( GameStateCompressed *state )
  {
  }

  void PacketDecoder::processClassid( classid *cid)
  {
    printClassid(cid);
    return;
  }

  void PacketDecoder::processAck( ack *data, int clientID)
  {
    printAck(data);
    return;
  }


  //these are some print functions for test stuff

  void PacketDecoder::printAck( ack* data )
  {
    cout << "data id: " << data->id << endl;
    cout << "data:    " << data->a << endl;
  }

  void PacketDecoder::printMouse( mouse* data )
  {
    cout << "data id: " << data->id << endl;
    cout << "data:    " << data->x << " " << data->y << endl;
  }

  void PacketDecoder::printKey( keyboard* data )
  {
    cout << "data id: " << data->id << endl;
    cout << "data:    " << (char)data->press << endl;
  }

  void PacketDecoder::printChat( chat* data, int clientId )
  {
    if(clientId!=CLIENTID_CLIENT)
      cout << "client: " << clientId << endl;
    cout << "data id: " << data->id << endl;
    cout << "data:    " << data->message << endl;
  }

  void PacketDecoder::printGamestate( GameStateCompressed* data )
  {
    cout << "id of GameStateCompressed:   " << data->id << endl;
    cout << "size of GameStateCompressed: " << data->compsize << endl;
  }

  void PacketDecoder::printClassid( classid *cid)
  {
    cout << "id of classid:    " << cid->id << endl;
    cout << "size of classid:  " << cid->length << endl;
    cout << "ID of classid:    " << cid->clid <<endl;
    cout << "data of classid:  " << cid->message <<endl;
  }

}