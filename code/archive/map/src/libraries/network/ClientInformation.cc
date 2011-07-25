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
#define WIN32_LEAN_AND_MEAN
#include <enet/enet.h>
#include "ClientConnectionListener.h"

namespace orxonox
{


  ClientInformation *ClientInformation::head_=0;

  ClientInformation::ClientInformation() {
    if(!head_)
      head_=this;
    gamestateID_=GAMESTATEID_INITIAL;
    preve=0;
    nexte=0;
    partialGamestateID_=GAMESTATEID_INITIAL-1;
    synched_=false;
  }

  ClientInformation::~ClientInformation() {
    if(prev()!=0)
      prev()->setNext(this->next());
    if(next()!=0)
      next()->setPrev(this->prev());
    if(this==head_)
      head_=next();
    ClientConnectionListener::broadcastClientDisconnected( this->getID() );
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
    if(this==head_)
      head_=prev;
    this->preve = prev;
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

  bool ClientInformation::setGamestateID(int id){
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

  unsigned int ClientInformation::getID() {
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

  int ClientInformation::getFailures(){
    return failures_;
  }
  void ClientInformation::addFailure(){
    failures_++;
  }
  void ClientInformation::resetFailures(){
    failures_=0;
  }

  uint32_t ClientInformation::getRTT(){
    return this->peer_->roundTripTime;
  }

  double ClientInformation::getPacketLoss(){
    return static_cast<double>(this->peer_->packetLoss)/ENET_PEER_PACKET_LOSS_SCALE;
  }

  unsigned int ClientInformation::getGamestateID() {
    if(this)
      return gamestateID_;
    else
      return static_cast<unsigned int>(-1);
  }

  unsigned int ClientInformation::getPartialGamestateID() {
    if(this)
      return partialGamestateID_;
    else
      return static_cast<unsigned int>(-1);
  }

  ClientInformation *ClientInformation::insertBack(ClientInformation *ins) {
    ClientInformation *temp = head_;
    if(temp==ins){
      return head_;
    }
    while(temp->next()!=0){
      temp = temp->next();
    }
    temp->setNext(ins);
    ins->setPrev(temp);
    return ins;
  }

  bool ClientInformation::removeClient(unsigned int clientID) {
    if(clientID==CLIENTID_UNKNOWN)
      return false;
    ClientInformation *temp = head_;
    while(temp!=0 && temp->getID()!=clientID)
      temp = temp->next();
    if(temp==0)
      return false;
    delete temp;
    return true;
  }

  bool ClientInformation::removeClient(ENetPeer *peer) {
    if(!peer)
      return false;
    ClientInformation *temp = head_;
    while(temp!=0){
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
  ClientInformation *ClientInformation::findClient(unsigned int clientID, bool look_backwards) {
    ClientInformation *temp = head_;
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
    ClientInformation *temp = head_;
    while(temp!=0){
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
