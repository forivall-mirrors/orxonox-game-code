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
 *      Oliver Scheuss <scheusso [at] ee.ethz.ch>, (C) 2008
 *   Co-authors:
 *      ...
 *
 */

#include "Chat.h"
#include <assert.h>

namespace network {
namespace packet {
  
#define PACKET_FLAGS_CHAT ENET_PACKET_FLAG_RELIABLE
#define _PACKETID         0
#define _MESSAGELENGTH    _PACKETID + sizeof(ENUM::Type)
#define _MESSAGE          _MESSAGELENGTH + sizeof(unsigned int)

Chat::Chat( std::string& message, int clientID )
 : Packet()
{
  flags_ = flags_ | PACKET_FLAGS_CHAT;
  messageLength_ = message.length()+1;
  data_=new unsigned char[ getSize() ];
  *(ENUM::Type *)&data_[ _PACKETID ] = ENUM::Chat;
  *(unsigned int *)&data_[ _MESSAGELENGTH ] = messageLength_;
  memcpy( &data_[ _MESSAGE ], (void *)message.c_str(), messageLength_ );
  clientID_=clientID;
}

Chat::Chat( unsigned char *data, int clientID )
  : Packet(data, clientID)
{
  messageLength_ = *(unsigned int *)&data[ _MESSAGELENGTH ];
}

Chat::~Chat()
{
}

unsigned int Chat::getSize() const{
  return _MESSAGE + messageLength_;
}

bool Chat::process(){
  //TODO: change this !!!
  assert(0);
  delete this;
  return true;
}

unsigned char *Chat::getMessage(){
  return &data_[ _MESSAGE ];
}

} //namespace packet
} //namespace network
