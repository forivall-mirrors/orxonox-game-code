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

#include "network/ConnectionManager.h"

namespace network{
  
  boost::thread_group network_threads;
  
  void test(){
    return;
  }
  
  ConnectionManager::ConnectionManager(){
    quit=false;
    client=NULL;
    bindAddress.host = ENET_HOST_ANY;
    bindAddress.port = NETWORK_PORT;
  }
  
  ConnectionManager::ConnectionManager(int port, std::string address){
    quit=false;
    client=NULL;
    enet_address_set_host (& bindAddress, address.c_str());
    bindAddress.port = NETWORK_PORT;
  }
  
  ConnectionManager::ConnectionManager(int port, const char *address){
    quit=false;
    client=NULL;
    enet_address_set_host (& bindAddress, address);
    bindAddress.port = NETWORK_PORT;
  }
  
  ENetPacket *ConnectionManager::getPacket(ENetAddress &address){
    if(!buffer.isEmpty())
      return buffer.pop(address);
    else
        return NULL;
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
    if(client=NULL)
      return false;
    ClientList *temp=client;
    while(peer->host != temp->event->peer->host){
      temp=temp->next;
      if(temp==NULL)
        return false;
    }
    if(enet_peer_send(temp->event->peer, temp->ID, packet)!=0)
      return false;
    return true;
  }
  
  bool ConnectionManager::addPacket(ENetPacket *packet, int ID){
    if(client==NULL)
      return false;
    ClientList *temp=client;
    while(ID != temp->ID){
      temp=temp->next;
      if(temp==NULL)
        return false;
    }
    if(enet_peer_send(temp->event->peer, temp->ID, packet)!=0)
      return false;
    else
      return true;
  }
  
  bool ConnectionManager::addPacketAll(ENetPacket *packet){
    ClientList *temp=client;
    while(temp!=NULL){
      if(enet_peer_send(temp->event->peer, temp->ID, packet)!=0)
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
  
  void ConnectionManager::receiverThread(){
    // what about some error-handling here ?
    enet_initialize();
    atexit(enet_deinitialize);
    ENetEvent event;
    server = enet_host_create(&bindAddress, NETWORK_MAX_CONNECTIONS, 0, 0);
    if(server==NULL)
      // add some error handling here ==========================
      quit=true;
    
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
      }
    }
    // if we're finishied, destroy server
    enet_host_destroy(server);
  }
  
  bool ConnectionManager::processData(ENetEvent *event){
    // just add packet to the buffer
    // this can be extended with some preprocessing
    return buffer.push(event);
  }
  
  bool ConnectionManager::clientDisconnect(ENetPeer *peer){
    ClientList *temp=client;
    // do we have to remove the first item ?
    if(temp->event->peer->host==peer->host){
      if(temp->next==NULL){
        client=NULL;
      } else{
        client=temp->next;
      }
      delete temp;
      return true;
    } else {
      while(temp->next!=NULL){
        if(temp->next->event->peer->host==peer->host){
          // do a correct relink and delete the disconnected client
          ClientList *temp2=temp->next;
          temp->next=temp2->next;
          delete temp2;
          return true;
        } else
          //otherwise keep on searching ;)
          temp=temp->next;
      }
    }
    return false;
  }
  
  bool ConnectionManager::addClient(ENetEvent *event){
    
    // first client?
    if(client==NULL){
      client =new ClientList;
      client->ID=1;
      client->next=NULL;
      client->event = event;
    } else {
      // otherwise add a new element to clientlist
      ClientList *temp = client;
      int i=1;
      while(temp->next != NULL){
        temp=temp->next;
        i++;
      }
      temp->next=new ClientList;
      temp=temp->next;
      temp->ID=i+1;
      temp->event=event;
      temp->next=NULL;
    }
    return true;
  }
  
  
}
