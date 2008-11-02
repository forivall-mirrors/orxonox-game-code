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


#ifndef NETWORK_PACKETGAMESTATE_H
#define NETWORK_PACKETGAMESTATE_H

#include "../NetworkPrereqs.h"

#include "Packet.h"
#include "network/Synchronisable.h"
#include <map>
#ifndef NDEBUG
#include "util/CRC32.h"
#endif

namespace orxonox {

namespace packet {

struct _NetworkExport GamestateHeader{
  ENUM::Type packetType;
  int32_t id; // id of the gamestate
  uint32_t compsize;
  uint32_t datasize;
  int32_t base_id; // id of the base-gamestate diffed from
  bool diffed:1; // wheter diffed or not
  bool complete:1; // wheter it is a complete gamestate or only partial
  bool compressed:1;
#ifndef NDEBUG
  uint32_t crc32;
#endif
};

/**
	@author Oliver Scheuss
*/
class _NetworkExport Gamestate: public Packet{
  public:
    Gamestate();
    Gamestate(uint8_t *data, unsigned int clientID);
    Gamestate(uint8_t *data);

    ~Gamestate();

    bool collectData(int id, int mode=0x0);
    bool spreadData(int mode=0x0);
    int getID();
    bool isDiffed();
    bool isCompressed();
    int getBaseID();
    Gamestate *diff(Gamestate *base);
    Gamestate* intelligentDiff(Gamestate *base, unsigned int clientID);
    Gamestate *undiff(Gamestate *base);
    Gamestate* intelligentUnDiff(Gamestate *base);
    Gamestate* doSelection(unsigned int clientID);
    bool compressData();
    bool decompressData();

    // Packet functions
  private:
    virtual unsigned int getSize() const;
    virtual bool process();

    bool operator ==(packet::Gamestate gs);
  private:
    unsigned int calcGamestateSize(unsigned int id, int mode=0x0);
    void removeObject(ObjectListIterator<Synchronisable> &it);
    std::map<unsigned int, Synchronisable*> dataMap_;
};

}

}

#endif
