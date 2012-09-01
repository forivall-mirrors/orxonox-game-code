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
  Chat( const std::string& message, unsigned int playerID );
  Chat( uint8_t* data, unsigned int clientID );
  ~Chat();

  inline unsigned int getSize() const;
  bool process();

  unsigned int getMessageLength(){ return messageLength_; };
  unsigned char *getMessage();
private:
  uint32_t messageLength_;
  unsigned int clientID_;
};

} //namespace packet
} //namespace orxonox

#endif /* _NETWORK_Chat_H__ */
