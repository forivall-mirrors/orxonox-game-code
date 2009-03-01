#ifndef _PacketManager_H__
#define _PacketManager_H__

#include <string>
#include <enet/enet.h>

#include "NetworkPrereqs.h"

#define CLIENTID_CLIENT -1

//enum netowk generally used to set the type ID of a packet
namespace network
{
  /*
  * class to generate packets
  * 
  * @autor: Dumeni Manatschal
  * 
  */ 
  class PacketGenerator
  {
  public:
    PacketGenerator();
    //call one of this functions out of an instance of PacketGenerator to create a packet
    ENetPacket* acknowledgement( int state, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* mousem( double x, double y, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* keystrike( char press, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* chatMessage( const char* message, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* gstate( GameStateCompressed *states, int reliable = ENET_PACKET_FLAG_RELIABLE );
    ENetPacket* clid( int classid, std::string classname, int reliable = ENET_PACKET_FLAG_RELIABLE );
  private:
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



    void acknowledgement( ENetPacket* packet, int clientId = CLIENTID_CLIENT );
    void mousem( ENetPacket* packet, int clientId = CLIENTID_CLIENT );
    void keystrike( ENetPacket* packet, int clientId = CLIENTID_CLIENT );
    void chatMessage( ENetPacket* packet, int clientId = CLIENTID_CLIENT);
    void gstate( ENetPacket* packet );
    void clid( ENetPacket *packet);

    //process data
    //two functions are note yet implemented!
    virtual void processGamestate(GameStateCompressed *state);
    virtual void processAck( ack *data, int clientID);
    void processClassid( classid *cid);
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
