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

#include "Packet.h"
#include "network/Synchronisable.h"
//#include "util/Bytestream.h"
#include "core/CoreIncludes.h"

#ifndef NETWORK_PACKETGAMESTATE_H
#define NETWORK_PACKETGAMESTATE_H

namespace network {

namespace packet {
  
struct GamestateHeader{
  ENUM::Type packetType;
  int id; // id of the gamestate
  unsigned int compsize;
  unsigned int normsize;
  int base_id; // id of the base-gamestate diffed from
  bool diffed; // wheter diffed or not
  bool complete; // wheter it is a complete gamestate or only partial
  bool compressed;
};

/**
	@author 
*/
class Gamestate: public Packet{
  public:
    Gamestate();
    Gamestate(unsigned char *data, int clientID);
    
    ~Gamestate();
    
    bool collectData(int id, int mode=0x0);
    bool spreadData(int mode=0x0);
    int getID();
    bool isDiffed();
    bool isCompressed();
    int getBaseID();
    Gamestate *diff(Gamestate *base);
    Gamestate *undiff(Gamestate *base);
    bool compressData();
    bool decompressData();
    
    // Packet functions
    virtual unsigned int getSize() const;
    virtual bool process();

    
  private:
    unsigned int calcGamestateSize(int mode=0x0);
    void removeObject(orxonox::Iterator<Synchronisable> &it);

    
    //Bytestream *bs_;
    //GamestateHeader *header_;
};

}

}

#endif
