//
// C++ Interface: ClientInformation
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NETWORKCLIENTINFORMATION_H
#define NETWORKCLIENTINFORMATION_H

#include <enet/enet.h>
#include <iostream> //debug

#define GAMESTATEID_INITIAL -1

namespace network {

/**
 * This class implements a list for client informations
 * @author Oliver Scheuss
*/
class ClientInformation{
public:
  ClientInformation();
  ClientInformation(bool head);
//   ClientInformation(ClientInformation *prev, ClientInformation *next);
//   ClientInformation(ClientInformation *prev);
  ~ClientInformation();
  ClientInformation *next();
  ClientInformation *prev();
  bool setNext(ClientInformation *next);
  bool setPrev(ClientInformation *prev);
  ClientInformation *insertAfter(ClientInformation *ins);
  ClientInformation *insertBefore(ClientInformation *ins);
  ClientInformation *insertBack(ClientInformation *ins);
  void setID(int clientID);
  void setPeer(ENetPeer *peer);
  void setGamestateID(int id);
  int getID();
  ENetPeer *getPeer();
  int getGamestateID();
  bool removeClient(int clientID);
  bool removeClient(ENetPeer *peer);
  ClientInformation *findClient(int clientID, bool look_backwards=false);
  ClientInformation *findClient(ENetAddress *address, bool look_backwards=false);
  bool head;
  
private:
  ClientInformation *preve;
  ClientInformation *nexte;
  //actual information:
  ENetPeer *peer_;
  int clientID_;
  int gamestateID_;
};

}

#endif
