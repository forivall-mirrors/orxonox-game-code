//
// C++ Interface: ClientConnection
//
// Description: The Class ClientConnection manages the servers conenctions to the clients.
// each connection is provided by a new process. communication between master process and
// connection processes is provided by ...
//
//
// Author:  Oliver Scheuss
//

#include "ClientConnection.h"

#ifdef WIN32
#include <windows.h>
#define usleep(x) Sleep((x)/1000)
#else
#include <unistd.h>
#endif

namespace network{

  static boost::thread_group network_threads;

  ClientConnection::ClientConnection(int port, std::string address){
    quit=false;
    server=NULL;
    enet_address_set_host(&serverAddress, address.c_str());
    serverAddress.port = NETWORK_PORT;
    established=false;
  }

  ClientConnection::ClientConnection(int port, const char *address){
    quit=false;
    server=NULL;
    enet_address_set_host(&serverAddress, address);
    serverAddress.port = NETWORK_PORT;
    established=false;
  }

  bool ClientConnection::waitEstablished(int milisec){
    for(int i=0; i<=milisec && !established; i++)
      usleep(1000);

    return established;
  }


  ENetPacket *ClientConnection::getPacket(ENetAddress &address){
    if(!buffer.isEmpty())
      return buffer.pop(address);
    else
        return NULL;
  }

  bool ClientConnection::queueEmpty(){
    return buffer.isEmpty();
  }

  bool ClientConnection::createConnection(){
    network_threads.create_thread(boost::bind(boost::mem_fn(&ClientConnection::receiverThread), this));
    // wait 10 seconds for the connection to be established
    return waitEstablished(10000);
  }

  bool ClientConnection::closeConnection(){
    quit=true;
    network_threads.join_all();
    established=false;
    return true;
  }


  bool ClientConnection::addPacket(ENetPacket *packet){
    if(server==NULL)
      return false;
    if(enet_peer_send(server, 1, packet)!=0)
      return false;
    else
      return true;
  }

  bool ClientConnection::sendPackets(ENetEvent *event){
    if(server==NULL)
      return false;
    if(enet_host_service(client, event, NETWORK_SEND_WAIT)>=0)
      return true;
    else
      return false;
  }

  bool ClientConnection::sendPackets(){
    ENetEvent event;
    if(server==NULL)
      return false;
    if(enet_host_service(client, &event, NETWORK_SEND_WAIT)>=0)
      return true;
    else
      return false;
  }

  void ClientConnection::receiverThread(){
    // what about some error-handling here ?
    enet_initialize();
    atexit(enet_deinitialize);
    ENetEvent event;
    client = enet_host_create(NULL, NETWORK_CLIENT_MAX_CONNECTIONS, 0, 0);
    if(client==NULL)
      // add some error handling here ==========================
      quit=true;
    //connect to the server
    if(!establishConnection())
      quit=true;
    //main loop
    while(!quit){
      if(enet_host_service(client, &event, NETWORK_WAIT_TIMEOUT)<0){
        // we should never reach this point
        quit=true;
        // add some error handling here ========================
      }
      switch(event.type){
        // log handling ================
      case ENET_EVENT_TYPE_CONNECT:
      case ENET_EVENT_TYPE_RECEIVE:
        processData(&event);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        // add some error/log handling here
        // extend =====================
        break;
      case ENET_EVENT_TYPE_NONE:
        continue;
      }
    }
    // now disconnect

    if(!disconnectConnection())
    // if disconnecting failed destroy conn.
      enet_peer_reset(server);
    return;
  }

  bool ClientConnection::disconnectConnection(){
    ENetEvent event;
//     enet_peer_disconnect(server);
    enet_peer_disconnect(server, 0);
    while(enet_host_service(client, &event, NETWORK_WAIT_TIMEOUT) > 0){
      switch (event.type)
      {
	case ENET_EVENT_TYPE_NONE:
	case ENET_EVENT_TYPE_CONNECT:
        case ENET_EVENT_TYPE_RECEIVE:
          enet_packet_destroy(event.packet);
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          return true;
      }
    }
    enet_peer_reset(server);
    return false;
  }

  bool ClientConnection::establishConnection(){
    ENetEvent event;
    // connect to peer
    server = enet_host_connect(client, &serverAddress, NETWORK_CLIENT_CHANNELS);
    if(server==NULL)
      // error handling
      return false;
    // handshake
    if(enet_host_service(client, &event, NETWORK_WAIT_TIMEOUT)>0 && event.type == ENET_EVENT_TYPE_CONNECT){
      established=true;
      return true;
    }
    else
      return false;
  }

  bool ClientConnection::processData(ENetEvent *event){
    // just add packet to the buffer
    // this can be extended with some preprocessing
    return buffer.push(event);
  }


}
