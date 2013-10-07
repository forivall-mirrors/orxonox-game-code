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
 *      Oliver Scheuss <scheusso [at] ee.ethz.ch>
 *   Co-authors:
 *      ...
 *
 */

#ifndef _NETWORK_Chat_H__
#define _NETWORK_Chat_H__

#include "network/NetworkPrereqs.h"
#include "Packet.h"

namespace orxonox {
namespace packet {
/**
    @author
*/
class _NetworkExport Chat : public Packet
{
public:
  /* constructors */
  Chat( const std::string& message, unsigned int sourceID, unsigned int targetID );
  Chat( uint8_t* data, unsigned int clientID );

  /* destructor */
  ~Chat();

  /* get size of packet */
  inline unsigned int getSize() const;

  /* process chat message packet and remove it afterwards */
  virtual bool process(orxonox::Host* host);

  /* Get the length of the message (not the full size of the packet) */
  unsigned int getMessageLength(){ return messageLength_; };

  /* return message content */
  unsigned char *getMessage();

private:

  /* Message length */
  uint32_t messageLength_;

  /* Client ID (an integral value for identification) */
  unsigned int clientID_;
};

} //namespace packet
} //namespace orxonox

#endif /* _NETWORK_Chat_H__ */
