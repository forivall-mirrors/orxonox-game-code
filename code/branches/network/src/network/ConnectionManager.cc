//
// C++ Interface: ConnectionManager
//
// Description: The Class ConnectionManager manages the servers conenctions to the clients. 
// each connection is provided by a new process. communication between master process and 
// connection processes is provided by ...
//
//
// Author:  Dumeni, Oli
//

// enet library for networking support
#include enet/enet.h
// boost.thread library for multithreading support
#include boost/thread/thread.hpp
// headerfile
#include "network/ConnectionManager.h"

namespace network{
  ConnectionManager::ConnectionManager(){
    quit=false;
    client=NULL;
    bindAddress.host = std::ENET_HOST_ANY;
    bindAddress.port = NETWORK_PORT;
  }
  
  ConnectionManager::ConnectionManager(int port, int address){
    quit=false;
    client=NULL;
    bindAddress.host = address;
    bindAddress.port = NETWORK_PORT;
  }
  
  void ConnectionManager::loop(){
    if(!quit && !buffer.isEmpty()){
      // to be implemented =================
      
    }
  }
  
  bool ConnectionManager::createListener(){
    threads.create_thread(&peerLoop);
    return true;
  }
  
  bool ConnectionManager::quitServer(){
    quit=true;
    threads.join_all();
    return true;
  }
  
  bool ConnectionManager::peerLoop(){
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
      case std::EVENT_TYPE_CONNECT:
        addClient(event);
        break;
      case std::EVENT_TYPE_RECEIVE:
        processData(event);
        break;
      case std::EVENT_TYPE_DISCONNECT:
        // add some error/log handling here
        clientDisconnect(event.peer);
        break;
      }
    }
    // if we're finishied, destroy server
    enet_host_destroy(server);
  }
  
  ConnectionManager::processData(ENetEvent event){
//     insert packetdecode and buffer stuff here
    
  }
  
  bool ConnectionManager::clientDisconnect(ENetPeer peer){
    ClientList *temp=client;
    if(temp->event.peer==peer){
      if(temp->next==NULL){
        client=NULL;
      } else{
        client=temp->next;
      }
      delete temp;
      return true;
    } else {
      while(temp->next!=NULL){
        if(temp->next->event.peer==peer){
          ClientList *temp2=temp->next;
          temp->next=temp2->next;
          delete temp2;
          return true;
        } else
          temp=temp->next;
      }
    }
    return false;
  }
  
  bool ConnectionManager::addClient(ENetEvent event){
    if(client==NULL){
      client =new ClientList;
      client->ID=1;
      client->next=NULL;
      client->event = event;
    } else {
      ClienList *temp = client;
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
  
  void ConnectionManager::test(){
    while(!quit)
    
    
  }
}
