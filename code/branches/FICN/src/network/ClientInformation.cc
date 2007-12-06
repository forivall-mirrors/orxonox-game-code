//
// C++ Implementation: ClientInformation
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ClientInformation.h"

namespace network {

ClientInformation::ClientInformation()
{
  preve=0;
  nexte=0;
}
//

// ClientInformation::ClientInformation(ClientInformation *prev){
//   if(prev->next()!=0){
//     this->nexte=prev->next();
//     this->nexte->setPrev(this);
//   }
//   else
//     this->nexte = 0;
//   prev->setNext(this);
//   this->preve = pref;
// }
// 
// ClientInformation::ClientInformation(ClientInformation *prev, ClientInformation *next){
//   this->nexte = next;
//   this->preve = prev;
//   this->preve->setNext(this);
//   this->nexte->setPrev(this);
// }

ClientInformation::~ClientInformation()
{
  if(preve!=0)
    preve->setNext(this->nexte);
  if(nexte!=0)
    nexte->setPrev(this->preve);
}

ClientInformation *ClientInformation::next(){
  return this->nexte;
}
ClientInformation *ClientInformation::prev(){
  return this->preve;
}

void ClientInformation::setPrev(ClientInformation *prev){
  this->preve = prev;
}

void ClientInformation::setNext(ClientInformation *next){
  this->nexte = next;
}

void ClientInformation::insertAfter(ClientInformation *ins){
  this->nexte->setPrev(ins);
  ins->setNext(this->nexte);
  ins->setPrev(this);
  this->nexte = ins;
}

void ClientInformation::insertBefore(ClientInformation *ins){
  this->prev()->setNext(ins);
  ins->setPrev(this->preve);
  this->preve=ins;
  ins->setNext(this);
}

}
