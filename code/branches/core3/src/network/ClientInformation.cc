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

#include <iostream> //debug

namespace network
{
  
  ClientInformation::ClientInformation() {
    gamestateID_=GAMESTATEID_INITIAL;
    preve=0;
    nexte=0;
    partialGamestateID_=GAMESTATEID_INITIAL-1;
    this->head_=false;
    synched_=false;
  }

  ClientInformation::ClientInformation(bool head) {
    gamestateID_=GAMESTATEID_INITIAL;
    preve=0;
    nexte=0;
    partialGamestateID_=GAMESTATEID_INITIAL-1;
    this->head_=head;
    synched_=false;
  }

  // ClientInformation::ClientInformation(ClientInformation *prev) {
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

  ClientInformation::~ClientInformation() {
    if(prev()!=0)
      prev()->setNext(this->next());
    if(next()!=0)
      next()->setPrev(this->prev());
  }

  ClientInformation *ClientInformation::next() {
    if(this!=0)
      return this->nexte;
    else
      return 0;
  }
  ClientInformation *ClientInformation::prev() {
    if(this!=0)
      return this->preve;
    else
      return 0;
  }

  bool ClientInformation::setPrev(ClientInformation *prev) {
    if(!head_)
      this->preve = prev;
    else
      return false;
    return true;
  }

  bool ClientInformation::setNext(ClientInformation *next) {
    this->nexte = next;
    return true;
  }

  ClientInformation *ClientInformation::insertAfter(ClientInformation *ins) {
    this->next()->setPrev(ins);
    ins->setNext(this->next());
    ins->setPrev(this);
    this->nexte = ins;
    return ins;
  }

  ClientInformation *ClientInformation::insertBefore(ClientInformation *ins){
    if(!this)
      return NULL;
    this->prev()->setNext(ins);
    ins->setPrev(this->prev());
    this->preve=ins;
    ins->setNext(this);
    return ins;
  }

  void ClientInformation::setID(int clientID){
    if(!this)
      return;
    clientID_ = clientID;
  }

  bool ClientInformation::setPeer(ENetPeer *peer){
    if(!this)
      return false;
    peer_ = peer;
    return true;
  }

  bool ClientInformation::setGameStateID(int id){
    if(!this)
      return false;
    gamestateID_=id;
    return true;
  }
  
  bool ClientInformation::setPartialGamestateID(int id){
    if(!this)
      return false;
    partialGamestateID_=id;
    return true;
  }

  int ClientInformation::getID() {
    if(!this)
      return CLIENTID_UNKNOWN;
    else
      return clientID_;
  }

  ENetPeer *ClientInformation::getPeer() {
    if(this)
      return peer_;
    else
      return NULL;
  }
  
  bool ClientInformation::getHead(){
    return head_;
  }
  
  void ClientInformation::setHead(bool h){
    head_=h;
  }
  
  int ClientInformation::getFailures(){
    return failures_;
  }
  void ClientInformation::addFailure(){
    failures_++;
  }
  void ClientInformation::resetFailures(){
    failures_=0;
  }
  
  enet_uint32 ClientInformation::getRTT(){
    return peer_->roundTripTime;
  }
  
  enet_uint32 ClientInformation::getPacketLoss(){
    return peer_->packetLoss;
  }

  int ClientInformation::getGamestateID() {
    if(this)
      return gamestateID_;
    else
      return -1;
  }
  
  int ClientInformation::getPartialGamestateID() {
    if(this)
      return partialGamestateID_;
    else
      return -1;
  }

  ClientInformation *ClientInformation::insertBack(ClientInformation *ins) {
    if(!this)
      return NULL;
    ClientInformation *temp = this;
    while(temp->next()!=0){
      temp = temp->next();
    }
    temp->setNext(ins);
    ins->setPrev(temp);
    return ins;
  }

  bool ClientInformation::removeClient(int clientID) {
    if(!this || clientID==CLIENTID_UNKNOWN)
      return false;
    ClientInformation *temp = this;
    while(temp!=0 && temp->getID()!=clientID)
      temp = temp->next();
    if(temp==0)
      return false;
    delete temp;
    return true;
  }

  bool ClientInformation::removeClient(ENetPeer *peer) {
    if(!this || !peer)
      return false;
    ClientInformation *temp = this;
    while(temp!=0){
      if(!temp->head_)
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
  * @return pointer to the last element in the list or 0 if the search was unsuccessfull
  */
  ClientInformation *ClientInformation::findClient(int clientID, bool look_backwards) {
    ClientInformation *temp = this;
    if (temp->head_)
      temp=temp->next();
    while(temp!=0 && temp->getID()!=clientID){
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
  ClientInformation *ClientInformation::findClient(ENetAddress *address, bool look_backwards) {
    ClientInformation *temp = this;
    while(temp!=0){
      if(temp->head_){
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

  bool ClientInformation::setSynched(bool s) {
    if(!this)
      return false;
    synched_=s;
    return true;
  }

  bool ClientInformation::getSynched() {
    if(this)
      return synched_;
    else
      return false;
  }

}
