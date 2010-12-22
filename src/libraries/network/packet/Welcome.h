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
#ifndef _NETWORK_Welcome_H__
#define _NETWORK_Welcome_H__

#include "network/NetworkPrereqs.h"
#include "Packet.h"

namespace orxonox {
namespace packet {

/**
    @author
*/
class _NetworkExport Welcome : public Packet
{
public:
  Welcome( uint32_t clientID, uint32_t shipID );
  Welcome( uint8_t* data, uint32_t clientID );
  virtual ~Welcome();

  uint8_t *getData();
  inline unsigned int getSize() const;
  virtual bool process(orxonox::Host* host);

private:
};

} //namespace packet
} //namespace orxonox

#endif /* _NETWORK_Welcome_H__ */
