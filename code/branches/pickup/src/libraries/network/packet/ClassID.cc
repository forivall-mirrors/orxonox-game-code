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
 *      Oliver Scheuss, (C) 2008
 *   Co-authors:
 *      ...
 *
 */

#include "ClassID.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <map>
#include <queue>
#include <string>

#include "core/CoreIncludes.h"

namespace orxonox {
namespace packet {


#define PACKET_FLAGS_CLASSID  PacketFlag::Reliable
#define _PACKETID             0


ClassID::ClassID( ) : Packet(){
  Identifier *id;
  std::string classname;
  unsigned int nrOfClasses=0; 
  unsigned int packetSize=2*sizeof(uint32_t); //space for the packetID and for the nrofclasses
  uint32_t network_id;
  flags_ = flags_ | PACKET_FLAGS_CLASSID;
  std::queue<std::pair<uint32_t, std::string> > tempQueue;
  
  //calculate total needed size (for all strings and integers)
  std::map<std::string, Identifier*>::const_iterator it = Factory::getFactoryMapBegin();
  for(;it != Factory::getFactoryMapEnd();++it){
    id = (*it).second;
    if(id == NULL)
      continue;
    classname = id->getName();
    network_id = id->getNetworkID();
    if(network_id==0)
      COUT(3) << "we got a null class id: " << id->getName() << std::endl;
    // now push the network id and the classname to the stack
    tempQueue.push( std::pair<unsigned int, std::string>(network_id, classname) );
    ++nrOfClasses;
    packetSize += (classname.size()+1)+sizeof(uint32_t)+sizeof(uint32_t);
  }
  
  this->data_=new uint8_t[ packetSize ];
  //set the appropriate packet id
  assert(this->data_);
  *(Type::Value *)(this->data_ + _PACKETID ) = Type::ClassID;
  
  uint8_t *temp=data_+sizeof(uint32_t);
  // save the number of all classes
  *(uint32_t*)temp = nrOfClasses;
  temp += sizeof(uint32_t);
  
  // now save all classids and classnames
  std::pair<uint32_t, std::string> tempPair;
  while( !tempQueue.empty() ){
    tempPair = tempQueue.front();
    tempQueue.pop();
    *(uint32_t*)temp = tempPair.first;
    *(uint32_t*)(temp+sizeof(uint32_t)) = tempPair.second.size()+1;
    memcpy(temp+2*sizeof(uint32_t), tempPair.second.c_str(), tempPair.second.size()+1);
    temp+=2*sizeof(uint32_t)+tempPair.second.size()+1;
  }
  
  COUT(5) << "classid packetSize is " << packetSize << endl;
  
}

ClassID::ClassID( uint8_t* data, unsigned int clientID )
  : Packet(data, clientID)
{
}

ClassID::~ClassID()
{
}

uint32_t ClassID::getSize() const{
  uint8_t *temp = data_+sizeof(uint32_t); // packet identification
  uint32_t totalsize = sizeof(uint32_t); // packet identification
  uint32_t nrOfClasses = *(uint32_t*)temp;
  temp += sizeof(uint32_t);
  totalsize += sizeof(uint32_t); // storage size for nr of all classes
  
  for(unsigned int i=0; i<nrOfClasses; i++){
    totalsize += 2*sizeof(uint32_t) + *(uint32_t*)(temp + sizeof(uint32_t));
  }
  return totalsize;
}


bool ClassID::process(){
  int nrOfClasses;
  uint8_t *temp = data_+sizeof(uint32_t); //skip the packetid
  uint32_t networkID;
  uint32_t stringsize;
  unsigned char *classname;
  
  
  //clean the map of network ids
  Factory::cleanNetworkIDs();
  
  COUT(4) << "=== processing classids: " << endl;
  std::pair<uint32_t, std::string> tempPair;
  Identifier *id;
  // read the total number of classes
  nrOfClasses = *(uint32_t*)temp;
  temp += sizeof(uint32_t);
  
  for( int i=0; i<nrOfClasses; i++){
    networkID = *(uint32_t*)temp;
    stringsize = *(uint32_t*)(temp+sizeof(uint32_t));
    classname = temp+2*sizeof(uint32_t);
    id=ClassByString( std::string((const char*)classname) );
    COUT(3) << "processing classid: " << networkID << " name: " << classname << " id: " << id << std::endl;
    if(id==NULL){
      COUT(0) << "Recieved a bad classname" << endl;
      abort();
    }
    id->setNetworkID( networkID );
    temp += 2*sizeof(uint32_t) + stringsize;
  }
  delete this;
  return true;
}


} //namespace packet
}//namespace orxonox
