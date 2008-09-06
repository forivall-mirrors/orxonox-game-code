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
#include "core/CoreIncludes.h"
#include <string>
#include <assert.h>

namespace network {
namespace packet {

#define PACKET_FLAGS_CLASSID  ENET_PACKET_FLAG_RELIABLE
#define _PACKETID             0
#define _CLASSID              _PACKETID + sizeof(ENUM::Type)
#define _CLASSNAMELENGTH      _CLASSID + sizeof(unsigned int)
#define _CLASSNAME            _CLASSNAMELENGTH + sizeof(classNameLength_)
  
  ClassID::ClassID( unsigned int classID, std::string className )
 : Packet()
{
  flags_ = flags_ | PACKET_FLAGS_CLASSID;
  classNameLength_=className.length()+1;
  assert(getSize());
  data_=new unsigned char[ getSize() ];
  assert(data_);
  *(ENUM::Type *)&data_[ _PACKETID ] = ENUM::ClassID;
  *(unsigned int *)&data_[ _CLASSID ] = classID;
  *(unsigned int *)&data_[ _CLASSNAMELENGTH ] = classNameLength_;
  memcpy( &data_[ _CLASSNAME ], (void *)className.c_str(), classNameLength_ );
}

ClassID::ClassID( unsigned char *data, int clientID )
  : Packet(data, clientID)
{
  memcpy( (void *)&classNameLength_, &data[ _CLASSNAMELENGTH ], sizeof(classNameLength_) );
}

ClassID::~ClassID()
{
}

unsigned int ClassID::getSize() const{
  return sizeof(network::packet::ENUM::Type) + 2*sizeof(unsigned int) + classNameLength_;
}

bool ClassID::process(){
  orxonox::Identifier *id=ID( std::string((const char*)&data_[ sizeof(network::packet::ENUM::Type) ]) );
  if(id==NULL)
    return false;
  id->setNetworkID( getClassID() );
  delete this;
  return true;
}

unsigned int ClassID::getClassID(){
  return *(unsigned int *)&data_[ _CLASSID ];
}

} //namespace packet
}//namespace network
