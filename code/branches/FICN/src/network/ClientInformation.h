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

namespace network {

/**
 * This class implements a list for client informations
 * @author Oliver Scheuss
*/
class ClientInformation{
public:
  ClientInformation();
//   ClientInformation(ClientInformation *prev, ClientInformation *next);
//   ClientInformation(ClientInformation *prev);
  ~ClientInformation();
  ClientInformation *next();
  ClientInformation *prev();
  void setNext(ClientInformation *next);
  void setPrev(ClientInformation *prev);
  void insertAfter(ClientInformation *ins);
  void insertBefore(ClientInformation *ins);
  
private:
  ClientInformation *preve;
  ClientInformation *nexte;
};

}

#endif
