/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

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
  gamestateID_=GAMESTATEID_INITIAL;
  preve=0;
  nexte=0;
  this->head=false;
}

ClientInformation::ClientInformation(bool head)
{
  gamestateID_=GAMESTATEID_INITIAL;
  preve=0;
  nexte=0;
  this->head=head;
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
  if(this!=0)
    return this->nexte;
  else
    return 0;
}
ClientInformation *ClientInformation::prev(){
  if(this!=0)
    return this->preve;
  else
    return 0;
}

bool ClientInformation::setPrev(ClientInformation *prev){
  if(!head)
    this->preve = prev;
  else
    return false;
  return true;
}

bool ClientInformation::setNext(ClientInformation *next){
  this->nexte = next;
  return true;
}

ClientInformation *ClientInformation::insertAfter(ClientInformation *ins){
  this->nexte->setPrev(ins);
  ins->setNext(this->nexte);
  ins->setPrev(this);
  this->nexte = ins;
  return ins;
}

ClientInformation *ClientInformation::insertBefore(ClientInformation *ins){
  this->prev()->setNext(ins);
  ins->setPrev(this->preve);
  this->preve=ins;
  ins->setNext(this);
  return ins;
}

void ClientInformation::setID(int clientID){
  clientID_ = clientID;
}
void ClientInformation::setPeer(ENetPeer *peer){
  peer_ = peer;
}

void ClientInformation::setGamestateID(int id){
  gamestateID_=id;
}

int ClientInformation::getID(){
  return clientID_;
}
ENetPeer *ClientInformation::getPeer(){
  return peer_;
}

int ClientInformation::getGamestateID(){
  return gamestateID_;
}

ClientInformation *ClientInformation::insertBack(ClientInformation *ins){
  ClientInformation *temp = this;
  while(temp->next()!=0){
    temp = temp->next();
  }
  temp->setNext(ins);
  ins->setPrev(temp);
  return ins;
}

bool ClientInformation::removeClient(int clientID){
  ClientInformation *temp = this;
  while(temp!=0 && temp->getID()!=clientID)
    temp = temp->next();
  if(temp==0)
    return false;
  delete temp;
  return true;
}

bool ClientInformation::removeClient(ENetPeer *peer){
  ClientInformation *temp = this;
  while(temp!=0){
    if(!temp->head)
      if(temp->getPeer()->address.host==peer->address.host && temp->getPeer()->address.port==peer->address.port)
        break;
    temp = temp->next();
  }
  if(temp==0)
    return false;
  delete temp;
  return true;
}

/**
 * This function goes forward through the list and looks for an element with clientID
 * This function should only be applied to the head of the list
 * @param clientID id to look for
 * @return pointer to the element in the list or 0 if the search was unsuccessfull
 */
ClientInformation *ClientInformation::findClient(int clientID, bool look_backwards){
  ClientInformation *temp = this;
  while(temp!=0 && temp->getID()!=clientID){
    if (temp->head)
      continue;
    temp = temp->next();
  }
  // returns 0 if nothing has been found
  return temp;
}

/**
 * This function goes forward through the list and looks for an element with clientID
 * This function should only be applied to the head of the list
 * @param peer peer to look for
 * @return pointer to the element in the list
 */
ClientInformation *ClientInformation::findClient(ENetAddress *address, bool look_backwards){
  ClientInformation *temp = this;
  while(temp!=0){
    if(temp->head){
      temp = temp->next();
      continue;
    }
    if(temp->getPeer()->address.host==address->host && temp->getPeer()->address.port == address->port)
      break;
    temp = temp->next();
  }
  // returns 0 if nothing has been found
  return temp;
}

}
