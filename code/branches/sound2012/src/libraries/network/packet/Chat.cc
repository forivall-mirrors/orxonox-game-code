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

#include <cstring>
#include <string>
#include "network/Host.h"

namespace orxonox {
namespace packet {

#define   PACKET_FLAGS_CHAT PacketFlag::Reliable

/* Some lengths */
#define _PACKETID         0
#define _SOURCEID         _PACKETID + sizeof(Type::Value)
#define _TARGETID         _SOURCEID + sizeof(uint32_t)
#define _MESSAGELENGTH    _TARGETID + sizeof(uint32_t)
#define _MESSAGE          _MESSAGELENGTH + sizeof(uint32_t)

Chat::Chat( const std::string& message, unsigned int sourceID, unsigned int targetID )
 : Packet()
{
  /* Add chat flag to packet flags */
  flags_ = flags_ | PACKET_FLAGS_CHAT;

  /* set message length to length of input string + 1 */
  messageLength_ = message.length()+1;

  /* allocate memory for the data */
  data_=new unsigned char[ getSize() ];

  *(Type::Value *)(data_ + _PACKETID ) = Type::Chat;
  *(unsigned int *)(data_ + _SOURCEID ) = sourceID;
  *(unsigned int *)(data_ + _TARGETID ) = targetID;
  *(unsigned int *)(data_ + _MESSAGELENGTH ) = messageLength_;

  /* cast the hell out of the message string, and copy it into the
   * data buffer.
   */
  memcpy( data_+_MESSAGE, static_cast<void*>(const_cast<char*>(message.c_str())), messageLength_ );
}

Chat::Chat( uint8_t* data, unsigned int clientID )
  : Packet(data, clientID)
{
  messageLength_ = *(uint32_t *)(data + _MESSAGELENGTH );
}

Chat::~Chat()
{
}

unsigned int Chat::getSize() const{
  return _MESSAGE + messageLength_;
}

bool Chat::process(orxonox::Host* host){
  host->doReceiveChat(std::string((const char*)data_+_MESSAGE), *(uint32_t *)(data_+_SOURCEID), *(uint32_t *)(data_+_TARGETID));
  delete this;
  return true;
}

unsigned char *Chat::getMessage(){
  return data_ + _MESSAGE;
}

} //namespace packet
} //namespace orxonox
