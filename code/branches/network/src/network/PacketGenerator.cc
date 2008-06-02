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
  void calcCRCBit(uint32_t &crc32, int bit){
    int hbit;
 
    hbit=(crc32 & 0x80000000) ? 1 : 0;
    if (hbit != bit)
      crc32=(crc32<<1) ^ NETWORK_CRC32POLY;
    else
      crc32=crc32<<1;
  }
  
  uint32_t calcCRC(unsigned char *data, unsigned int dataLength){
    uint32_t crc32=0;
    for(unsigned int i=0; i<dataLength; i++){
      calcCRCBit(crc32, (data[i]&0x1)>>0); // 1st bit
      calcCRCBit(crc32, (data[i]&0x2)>>1); // 2nd bit
      calcCRCBit(crc32, (data[i]&0x3)>>2); // 3rd bit
      calcCRCBit(crc32, (data[i]&0x4)>>3); // 4th bit
      calcCRCBit(crc32, (data[i]&0x5)>>4); // 5th bit
      calcCRCBit(crc32, (data[i]&0x6)>>5); // 6th bit
      calcCRCBit(crc32, (data[i]&0x7)>>6); // 7th bit
      calcCRCBit(crc32, (data[i]&0x8)>>7); // 8th bit
    }
    return crc32;
  }
  
  PacketGenerator::PacketGenerator() { }

  //following functions create a packet in form of bytestream

  ENetPacket* PacketGenerator::acknowledgement( int state, int reliable )
  {
    COUT(4) << "PacketGenerator: generating new acknowledgement, id: " << state << std::endl;
    
    ENetPacket *packet = enet_packet_create( NULL , sizeof( ack ), reliable );
    ack* ackreq = (ack *)packet->data;
    ackreq->id = ACK;
    ackreq->a = state;
    //delete ackreq;
    return packet;
  }
  
  ENetPacket* command( int dataLength, void *data, int reliable = ENET_PACKET_FLAG_RELIABLE )
  {
    if(dataLength==0)
      return NULL;
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
    if(totalLen==0)
      return NULL;
    ENetPacket *packet = enet_packet_create( NULL , totalLen+sizeof(uint32_t), reliable );
    //unsigned char *data = new unsigned char[totalLen];
    unsigned char *data = packet->data;
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
    //delete[] data;
//    if(!addCRC(packet))
//      COUT(3) << "could not add crc to gamestate packet" << std::endl;
    return packet;
  }

  ENetPacket* PacketGenerator::clid( int classid, std::string classname, int reliable )
  {
    //unsigned char* data = (unsigned char *)malloc(3*sizeof(int)+classname.length()+1);
    if(classname.length()==0)
      return NULL;
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
  
  
  bool PacketGenerator::addCRC( ENetPacket *packet){
    unsigned char *data = packet->data;
    uint32_t crc32=calcCRC(data, packet->dataLength-sizeof(uint32_t));
    // now append the crc to the packet data
    int oldlength = packet->dataLength-sizeof(uint32_t);
    //if(enet_packet_resize(packet, packet->dataLength+sizeof(uint32_t))==0){
      memcpy(&packet->data[oldlength], &crc32, sizeof(uint32_t));
      return true;
    //}else{
     // COUT(3) << "could not add crc to gamestate" << std::endl;
     // return false;
    //}
  }

}
