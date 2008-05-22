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

#ifndef _PacketManager_H__
#define _PacketManager_H__

#include "NetworkPrereqs.h"

#include <string>
#include <inttypes.h>
#include <enet/enet.h>

#include "core/CoreIncludes.h"

#define CLIENTID_CLIENT -1
#define NETWORK_CRC32POLY 0x04C11DB7 /* CRC-32 Polynom */

//enum netowk generally used to set the type ID of a packet
namespace network
{
  /*
  * class to generate packets
  *
  * @autor: Dumeni Manatschal
  *
  */
  //void calcCRC(uint32_t &crc32, int bit);
  uint32_t calcCRC(unsigned char *data, unsigned int dataLength);
  
  class PacketGenerator
  {
  public:
    PacketGenerator();
    //call one of this functions out of an instance of PacketGenerator to create a packet
    ENetPacket* acknowledgement( int state, int reliable = 0 ); // we do not want reliability
    ENetPacket* command( int dataLength, void *data, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* mousem( double x, double y, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* keystrike( char press, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* chatMessage( const char* message, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* gstate( GameStateCompressed *states, int reliable = 0 ); // we do not want reliability of gamestates
    ENetPacket* clid( int classid, std::string classname, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* generateWelcome( int clientID,int shipID, bool allowed, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* generateConnectRequest( int reliable = ENET_PACKET_FLAG_RELIABLE );
  private:
    bool addCRC( ENetPacket *packet);
  };

  /*
  * class used to decode incoming packets
  *
  * @autor: Dumeni Manatschal
  *
  */
  class _NetworkExport PacketDecoder
  {
  public:
    PacketDecoder();
    virtual ~PacketDecoder();
    //call this function to decode, it calls the right decoding function below
    bool elaborate( ENetPacket* packet, int clientId );
  protected:

    virtual void processChat( chat *data, int clientId);


  private:
    bool testAndRemoveCRC(ENetPacket *packet);


    void acknowledgement( ENetPacket* packet, int clientId = CLIENTID_CLIENT );
    bool command( ENetPacket* packet, int clientId );
    void mousem( ENetPacket* packet, int clientId = CLIENTID_CLIENT );
    void keystrike( ENetPacket* packet, int clientId = CLIENTID_CLIENT );
    void chatMessage( ENetPacket* packet, int clientId = CLIENTID_CLIENT );
    void gstate( ENetPacket* packet, int clientID = CLIENTID_CLIENT );
    void clid( ENetPacket *packet);
    bool decodeWelcome( ENetPacket* packet, int clientID = CLIENTID_CLIENT );
    bool decodeConnectRequest( ENetPacket *packet, int clientID = CLIENTID_CLIENT );

    //process data
    //two functions are note yet implemented!
    virtual void processGamestate(GameStateCompressed *state, int clientID);
    virtual void processAck( ack *data, int clientID);
    virtual void processClassid( classid *cid);
    virtual bool processWelcome( welcome *w );
    virtual bool processConnectRequest( connectRequest *con, int clientID );
    //virtual void processAck( ack *data);

    //print functions
    void printAck( ack* data );
    void printMouse( mouse* data );
    void printKey( keyboard* data );
    void printChat( chat* data, int clientId );
    void printGamestate( GameStateCompressed *data );
    void printClassid( classid *cid);
  };
}

#endif /* _PacketManager_H__ */
