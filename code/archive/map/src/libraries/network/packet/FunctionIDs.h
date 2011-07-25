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

#ifndef _FunctionIDs_H__
#define _FunctionIDs_H__

#include "network/NetworkPrereqs.h"
#include "Packet.h"

namespace orxonox {
namespace packet {


/**
    @author
*/
class _NetworkExport FunctionIDs : public Packet
{
public:
  FunctionIDs( );
  FunctionIDs( uint8_t* data, unsigned int clientID );
  ~FunctionIDs();

  uint32_t getSize() const;
  bool process();

private:
};

} //namespace packet
} //namespace orxonox

#endif /* _FunctionIDs_H__ */
