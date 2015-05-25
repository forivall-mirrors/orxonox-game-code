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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

#include "FunctionIDs.h"

#include <cassert>
#include <cstring>
#include <queue>
#include <string>

#include "util/Output.h"
#include "core/object/ObjectList.h"
#include "network/NetworkFunction.h"
#include "network/NetworkFunctionManager.h"

namespace orxonox {
namespace packet {


#define PACKET_FLAGS_FUNCTIONIDS  PacketFlag::Reliable
#define _PACKETID                 0


FunctionIDs::FunctionIDs( ) : Packet()
{
  unsigned int nrOfFunctions=0;
  unsigned int packetSize=2*sizeof(uint32_t); //space for the packetID and for the nroffunctions
  uint32_t networkID;
  flags_ = flags_ | PACKET_FLAGS_FUNCTIONIDS;
  std::queue<std::pair<uint32_t, std::string> > tempQueue;

  //calculate total needed size (for all strings and integers)
  ObjectList<NetworkFunctionBase>::iterator it;
  for(it = ObjectList<NetworkFunctionBase>::begin(); it; ++it)
  {
    const std::string& functionname = it->getName();
    networkID = it->getNetworkID();
    // now push the network id and the classname to the stack
    tempQueue.push( std::pair<unsigned int, std::string>(networkID, functionname) );
    ++nrOfFunctions;
    packetSize += (functionname.size()+1)+sizeof(uint32_t)+sizeof(uint32_t); // reserver size for the functionname string, the functionname length and the networkID
  }

  this->data_=new uint8_t[ packetSize ];
  //set the appropriate packet id
  assert(this->data_);
  *(Type::Value *)(this->data_ + _PACKETID ) = Type::FunctionIDs;

  uint8_t *temp=data_+sizeof(uint32_t);
  // save the number of all classes
  *(uint32_t*)temp = nrOfFunctions;
  temp += sizeof(uint32_t);

  // now save all classids and classnames
  std::pair<uint32_t, std::string> tempPair;
  while( !tempQueue.empty() )
  {
    tempPair = tempQueue.front();
    tempQueue.pop();
    *(uint32_t*)temp = tempPair.first;
    *(uint32_t*)(temp+sizeof(uint32_t)) = tempPair.second.size()+1;
    memcpy(temp+2*sizeof(uint32_t), tempPair.second.c_str(), tempPair.second.size()+1);
    temp+=2*sizeof(uint32_t)+tempPair.second.size()+1;
  }

  orxout(verbose_more, context::packets) << "FunctionIDs packetSize is " << packetSize << endl;

}

FunctionIDs::FunctionIDs( uint8_t* data, unsigned int clientID )
  : Packet(data, clientID)
{
}

FunctionIDs::~FunctionIDs()
{
}

uint32_t FunctionIDs::getSize() const
{
  assert(this->data_);
  uint8_t *temp = data_+sizeof(uint32_t); // packet identification
  uint32_t totalsize = sizeof(uint32_t); // data size
  uint32_t nrOfFunctions = *(uint32_t*)temp;
  temp += sizeof(uint32_t);
  totalsize += sizeof(uint32_t); // storage size for nr of all classes

  for(unsigned int i=0; i<nrOfFunctions; i++){
    totalsize += 2*sizeof(uint32_t) + *(uint32_t*)(temp + sizeof(uint32_t)); // for each network function add size for id, sizeof(string) and length of string itself to totalsize
    temp += 2*sizeof(uint32_t) + *(uint32_t*)(temp + sizeof(uint32_t));
  }
  return totalsize;
}


bool FunctionIDs::process(orxonox::Host* host)
{
  int nrOfFunctions;
  uint8_t *temp = data_+sizeof(uint32_t); //skip the packetid
  uint32_t networkID;
  uint32_t stringsize;
  unsigned char *functionname;

  orxout(verbose, context::packets) << "=== processing functionids: " << endl;
  std::pair<uint32_t, std::string> tempPair;
  // read the total number of classes
  nrOfFunctions = *(uint32_t*)temp;
  temp += sizeof(uint32_t);

  for( int i=0; i<nrOfFunctions; i++)
  {
    networkID = *(uint32_t*)temp;
    stringsize = *(uint32_t*)(temp+sizeof(uint32_t));
    functionname = temp+2*sizeof(uint32_t);
    orxout(internal_info, context::packets) << "processing functionid: " << networkID << " name: " << functionname << endl;
    NetworkFunctionManager::setNetworkID((const char*)functionname, networkID);
    temp += 2*sizeof(uint32_t) + stringsize;
  }
  delete this;
  return true;
}


} //namespace packet
}//namespace orxonox
