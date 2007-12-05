//
// C++ Interface: ConnectionManager
//
// Description: The Class ConnectionManager manages the servers conenctions to the clients.
// each connection is provided by a new process. communication between master process and
// connection processes is provided by ...
//
//
// Author:  Oliver Scheuss
//

#include "ConnectionManager.h"

namespace std{
  bool operator< (ENetAddress a, ENetAddress b){
    if(a.host <= b.host)
      return true;
    else
      return false;
  }
}

namespace network{

  boost::thread_group network_threads;
  
  ConnectionManager::ConnectionManager(){
    quit=false;
    bindAddress.host = ENET_HOST_ANY;
    bindAddress.port = NETWORK_PORT;
  }

  ConnectionManager::ConnectionManager(int port, std::string address){
    quit=false;
    enet_address_set_host (& bindAddress, address.c_str());
    bindAddress.port = NETWORK_PORT;
  }

  ConnectionManager::ConnectionManager(int port, const char *address){
    quit=false;
    enet_address_set_host (& bindAddress, address);
    bindAddress.port = NETWORK_PORT;
  }

  ENetPacket *ConnectionManager::getPacket(ENetAddress &address){
    if(!buffer.isEmpty())
      return buffer.pop(address);
    else
        return NULL;
  }
  
  ENetPacket *ConnectionManager::getPacket(int &clientID){
    ENetAddress address;
    ENetPacket *packet=getPacket(address);
    clientID=clientMap.find(address)->second;
    return packet;
  }

  bool ConnectionManager::queueEmpty(){
    return buffer.isEmpty();
  }

  void ConnectionManager::createListener(){
    network_threads.create_thread(boost::bind(boost::mem_fn(&ConnectionManager::receiverThread), this));
//     boost::thread thr(boost::bind(boost::mem_fn(&ConnectionManager::receiverThread), this));
    return;
  }

  bool ConnectionManager::quitListener(){
    quit=true;
    network_threads.join_all();
    return true;
  }

  bool ConnectionManager::addPacket(ENetPacket *packet, ENetPeer *peer){
    if(clientVector.size()==0)
      return false;
    if(enet_peer_send(peer, clientMap.find(peer->address)->second , packet)!=0)
      return false;
    return true;
  }
  
  bool ConnectionManager::addPacket(ENetPacket *packet, int clientID){
    if(clientVector.size()==0)
      return false;
    if(enet_peer_send(&(peerMap.find(clientVector[clientID])->second), clientID, packet)!=0)
      return false;
    return true;
  }
  
  bool ConnectionManager::addPacketAll(ENetPacket *packet){
    for(int i=0; i<clientVector.size(); i++){
      if(enet_peer_send(&(peerMap.find(clientVector[i])->second), i, packet)!=0)
         return false;
    }
    return true;
  }

  bool ConnectionManager::sendPackets(ENetEvent *event){
    if(server==NULL)
      return false;
    if(enet_host_service(server, event, NETWORK_SEND_WAIT)>=0)
      return true;
    else
      return false;
  }
  
  bool ConnectionManager::sendPackets(){
    ENetEvent event;
    if(server==NULL)
      return false;
    if(enet_host_service(server, &event, NETWORK_SEND_WAIT)>=0)
      return true;
    else
      return false;
  }

  void ConnectionManager::receiverThread(){
    // what about some error-handling here ?
    enet_initialize();
    atexit(enet_deinitialize);
    ENetEvent event;
    server = enet_host_create(&bindAddress, NETWORK_MAX_CONNECTIONS, 0, 0);
    if(server==NULL){
      // add some error handling here ==========================
      quit=true;
      return;
    }

    while(!quit){
      if(enet_host_service(server, &event, NETWORK_WAIT_TIMEOUT)<0){
        // we should never reach this point
        quit=true;
        // add some error handling here ========================
      }
      switch(event.type){
        // log handling ================
        case ENET_EVENT_TYPE_CONNECT:
        addClient(&event);
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        processData(&event);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        // add some error/log handling here
        clientDisconnect(event.peer);
        break;
      case ENET_EVENT_TYPE_NONE:
        break;
      }
    }
    disconnectClients();
    // if we're finishied, destroy server
    enet_host_destroy(server);
  }
  
  void ConnectionManager::disconnectClients(){
    bool disconnected=false;
    ENetEvent event;
    for(int i=0; i<clientVector.size(); i++){
      enet_peer_disconnect(&(peerMap.find(clientVector[i])->second), 0);
      while( !disconnected && enet_host_service(server, &event, NETWORK_WAIT_TIMEOUT) > 0){
        switch (event.type)
        {
          case ENET_EVENT_TYPE_NONE:
          case ENET_EVENT_TYPE_CONNECT:
          case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy(event.packet);
            break;
          case ENET_EVENT_TYPE_DISCONNECT:
            disconnected=true;
            break;
        }
      }
      disconnected=false;
    }
    return;
  }

  bool ConnectionManager::processData(ENetEvent *event){
    // just add packet to the buffer
    // this can be extended with some preprocessing
    return buffer.push(event);
  }

  bool ConnectionManager::clientDisconnect(ENetPeer *peer){
    return clientDisconnect(*peer);
  }
  
  bool ConnectionManager::clientDisconnect(ENetPeer peer){
    std::map<ENetAddress, int>::iterator it;
    it=clientMap.find(peer.address);
    clientVector.erase(clientVector.begin()+it->second);
    clientMap.erase(it);
    peerMap.erase(peerMap.find(peer.address));
    return true;
  }

  bool ConnectionManager::addClient(ENetEvent *event){
    int id=clientVector.size();
    clientVector.push_back((event->peer->address));
    clientMap[event->peer->address]=id;
    peerMap[event->peer->address]=*(event->peer);
    syncClassid(id);
    return true;
  }
  
  int ConnectionManager::getClientID(ENetPeer peer){
    return clientMap.find(peer.address)->second;
  }
  
  int ConnectionManager::getClientID(ENetAddress address){
    return clientMap.find(address)->second;
  }
  
  ENetPeer ConnectionManager::getClientPeer(int clientID){
    return peerMap.find(clientVector[clientID])->second;
  }
  
  void ConnectionManager::syncClassid(int clientID){
    int i=0;
    std::string classname;
    bool abort=false;
    orxonox::Identifier *id;
    while(!abort){
      id = orxonox::ID(i);
      if(id == NULL)
        abort=true;
      else{
        classname = id->getName();
        addPacket(packet_gen.clid( i, classname ),clientID);
      }
    }
    sendPackets();
  }
  
}
