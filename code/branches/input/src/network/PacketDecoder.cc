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
* Class contains functions to determine and decode incomming packages
* ->don't read this without the class PacketGenerator, since they belong together
*
* Autor: Dumeni Manatschal
*
*/

#include "PacketTypes.h"
#include "PacketManager.h"

#include <iostream>

#include "core/Debug.h"

namespace network
{

  PacketDecoder::PacketDecoder(){}

  PacketDecoder::~PacketDecoder(){}

  //call this function out of an instance of PacketDecoder
  //it will determine the type id and call the right decode function
  bool PacketDecoder::elaborate( ENetPacket* packet, int clientId )
  {
    COUT(5) << "PacketDecoder: clientId: " << clientId << std::endl; //control cout, not important, just debugging info
    int id = (int)*packet->data; //the first 4 bytes are always the enet packet id
    COUT(5) << "PacketDecoder: packet id: " << id << std::endl;
    //COUT(5) << "packet size inside packetdecoder: " << packet->dataLength << std::endl;

    if ( packet == NULL ) {
      COUT(4) << "PacketDecoder: no packets->packetbuffer queue is empty" << std::endl;
      return false;
    }
    switch( id ) {
    case ACK:
      acknowledgement( packet, clientId );
      return true;
    case COMMAND:
      return command( packet, clientId );
    case MOUSE:
      mousem( packet, clientId );
      return true;
    case KEYBOARD:
      keystrike( packet, clientId );
      return true;
    case CHAT:
      chatMessage( packet, clientId );
      return true;
    case GAMESTATE:
      gstate( packet, clientId );
      return true;
    case CLASSID:
      clid(packet);
      return true;
    case WELCOME:
      return decodeWelcome( packet, clientId );
    case CONNECT:
      return decodeConnectRequest( packet, clientId );
    }
    return false;
  }
  
  // ATTENTION: TODO watch, that arguments we pass over to the processFunction gets deleted in THE PROCESSXXX function

  //following are the decode functions for the data of the packets

  void PacketDecoder::acknowledgement( ENetPacket* packet, int clientId )
  {
    ack* a = new ack;
    *a = *(ack*)(packet->data); //press pattern of ack on new data


    COUT(4) << "PacketDecoder: got ack id: " << a->a << std::endl;
    processAck( a, clientId ); //debug info
    //clean memory
    enet_packet_destroy( packet );
  }
  
  bool PacketDecoder::command( ENetPacket* packet, int clientId ){
    int length = *(int*)((unsigned char *)packet->data+sizeof(int));
    void *data = (void *)new unsigned char[length];
    memcpy(data, (void *)(packet->data+2*sizeof(int)), length);
    return true;
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

  void PacketDecoder::gstate( ENetPacket* packet, int clientID )
  {
    GameStateCompressed* currentState = NULL;
    currentState = new GameStateCompressed;
    if(currentState == NULL){
      COUT(3) << "PacketDecoder: could not generate new GameStateCompressed" << std::endl;
      return;
    }
    //since it's not alowed to use void* for pointer arithmetic
    //FIXME: variable never used
    unsigned char* data = (unsigned char *)(packet->data);
    //copy the GameStateCompressed id into the struct, which is located at second place data+sizeof( int )
    memcpy( (void*)&(currentState->id), (const void*)(packet->data+1*sizeof( int )), sizeof( int) );
    COUT(5) << "PacketDecoder: received gs id: " << currentState->id << std::endl;
    //copy the size of the GameStateCompressed compressed data into the new GameStateCompressed struct, located at 3th
    //position of the data stream, data+2*sizeof( int )
    memcpy( (void*)&(currentState->compsize), (const void*)(packet->data+2*sizeof( int )), sizeof( int) );
    //size of uncompressed data
    memcpy( (void*)&(currentState->normsize), (const void*)(packet->data+3*sizeof( int )), sizeof( int ) );
    memcpy( (void*)&(currentState->base_id), (const void*)(packet->data+4*sizeof( int )), sizeof( int ) );
    //since the packetgenerator was changed, due to a new parameter, change this function too
    memcpy( (void*)&(currentState->diffed), (const void*)(packet->data+5*sizeof(int)), sizeof(bool));
    memcpy( (void*)&(currentState->complete), (const void*)(packet->data+5*sizeof(int)+sizeof(bool)), sizeof(bool));
    //since data is not allocated, because it's just a pointer, allocate it with size of gamestatedatastream
    if(currentState->compsize==0)
      COUT(2) << "PacketDecoder: compsize is 0" << std::endl;
//     currentState->data = (unsigned char*)(malloc( currentState->compsize ));
    currentState->data = new unsigned char[currentState->compsize];
    if(currentState->data==NULL)
      COUT(2) << "PacketDecoder: Gamestatepacket-decoder: memory leak" << std::endl;
    //copy the GameStateCompressed data
    memcpy( (void*)(currentState->data), (const void*)(packet->data+5*sizeof( int ) + 2*sizeof(bool)), currentState->compsize );

    //clean memory
    enet_packet_destroy( packet );
    processGamestate(currentState, clientID);
  }

  void PacketDecoder::clid( ENetPacket *packet)
  {
    classid* cid = new classid;
    cid->length = ((classid*)(packet->data))->length;
    cid->id = ((classid *)(packet->data))->id;
    cid->clid = ((classid *)(packet->data))->clid;
//     cid->message = (const char *)malloc(cid->length);
    cid->message = new char[cid->length];
    void *data  = (void *)cid->message;
    memcpy(data, (const void*)(packet->data+3*sizeof(int)), cid->length);
    COUT(4) << "PacketDecoder: classid: " << cid->clid << ", name: " << cid->message << std::endl;
    enet_packet_destroy( packet );
    processClassid(cid);
  }
  
  
  bool PacketDecoder::decodeWelcome( ENetPacket* packet, int clientID ){
    welcome *w = new welcome;
    w->allowed = ((welcome *)(packet->data))->allowed;
    w->shipID = ((welcome *)(packet->data))->shipID;
    w->clientID = ((welcome *)(packet->data))->clientID;
    w->id = ((welcome *)(packet->data))->id;
    enet_packet_destroy( packet );
    return processWelcome(w);
  }
  
  bool PacketDecoder::decodeConnectRequest( ENetPacket *packet, int clientID ){
    connectRequest *con = new connectRequest;
    con->id = ((connectRequest *)(packet->data))->id;
    enet_packet_destroy( packet );
    return processConnectRequest(con, clientID );
  }


  // now the data processing functions:

  void PacketDecoder::processChat( chat *data, int clientId)
  {
    printChat(data, clientId);
    delete[] data->message;
    delete data;
  }

  void PacketDecoder::processGamestate( GameStateCompressed *state, int clientID )
  {
    COUT(3) << "PacketDecoder-process: processing Gamestate" << std::endl;
    //printGamestate( state );
//     delete[] state->data;
//     delete state;
  }

  void PacketDecoder::processClassid( classid *cid)
  {
    printClassid(cid);
    delete cid;
    return;
  }

  void PacketDecoder::processAck( ack *data, int clientID)
  {
    printAck(data);
    delete data;
    return;
  }
  
  bool PacketDecoder::processWelcome( welcome *w ){
    delete w;
    return true;
  }
  
  bool PacketDecoder::processConnectRequest( connectRequest *con, int clientID ){
    COUT(3) << "packetdecoder: processing connectRequest" << std::endl;
    delete con;
    return true;
  }

  //these are some print functions for test stuff

  void PacketDecoder::printAck( ack* data )
  {
    COUT(5) << "data id: " << data->id << std::endl;
    COUT(5) << "data:    " << data->a << std::endl;
  }

  void PacketDecoder::printMouse( mouse* data )
  {
    COUT(5) << "data id: " << data->id << std::endl;
    COUT(5) << "data:    " << data->x << " " << data->y << std::endl;
  }

  void PacketDecoder::printKey( keyboard* data )
  {
    COUT(5) << "data id: " << data->id << std::endl;
    COUT(5) << "data:    " << (char)data->press << std::endl;
  }

  void PacketDecoder::printChat( chat* data, int clientId )
  {
    if(clientId!=CLIENTID_CLIENT)
      COUT(5) << "client: " << clientId << std::endl;
    COUT(5) << "data id: " << data->id << std::endl;
    COUT(5) << "data:    " << data->message << std::endl;
  }

  void PacketDecoder::printGamestate( GameStateCompressed* data )
  {
    COUT(5) << "id of GameStateCompressed:   " << data->id << std::endl;
    COUT(5) << "size of GameStateCompressed: " << data->compsize << std::endl;
  }

  void PacketDecoder::printClassid( classid *cid)
  {
    COUT(5) << "id of classid:    " << cid->id << std::endl;
    COUT(5) << "size of classid:  " << cid->length << std::endl;
    COUT(5) << "ID of classid:    " << cid->clid << std::endl;
    COUT(5) << "data of classid:  " << cid->message << std::endl;
  }

}
