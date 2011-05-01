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


#ifndef _Gamestate_H__
#define _Gamestate_H__

#include "network/NetworkPrereqs.h"

#include <cassert>
#include <cstring>
#include <list>
#include <vector>

#include "util/CRC32.h"
#include "network/TrafficControl.h"
#include "Packet.h"

namespace orxonox
{

namespace packet
{
    
static const uint8_t GAMESTATE_MODE_SERVER = 0x1;
static const uint8_t GAMESTATE_MODE_CLIENT = 0x2;

class _NetworkExport GamestateHeader
{
  public:
    GamestateHeader(){ data_=0; }
    GamestateHeader(uint8_t* data)
      { assert(data); data_ = data; *(uint32_t*)data_ = Type::Gamestate; }
    /*GamestateHeader(uint8_t* data, GamestateHeader* h)
      { assert(data); data_=data; memcpy(data_, h->data_, getSize()); }*/
    void setData(uint8_t* data)
      { assert(data); data_ = data; *(uint32_t*)data_ = Type::Gamestate; }
    static inline uint32_t getSize()
      { return 21; }

    inline uint32_t getID() const
      { assert(data_); return *(uint32_t*)(data_+4); }
    inline void setID(uint32_t id)
      { assert(data_); *(uint32_t*)(data_+4) = id; }

    inline uint32_t getBaseID() const
      { assert(data_); return *(uint32_t*)(data_+8); }
    inline void setBaseID(uint32_t id)
      { assert(data_); *(uint32_t*)(data_+8) = id; }

    inline uint32_t getDataSize() const
      { assert(data_); return *(uint32_t*)(data_+12); }
    inline void setDataSize(uint32_t size)
      { assert(data_); *(uint32_t*)(data_+12) = size; }

    inline uint32_t getCompSize() const
    { assert(data_); return *(uint32_t*)(data_+16); }
    inline void setCompSize(uint32_t size)
    { assert(data_); *(uint32_t*)(data_+16) = size; }

    inline bool isDiffed() const
      { assert(data_); return *(int8_t*)(data_+20) & 0x1; }
    inline void setDiffed(bool b)
      { assert(data_); *(int8_t*)(data_+20) = (b<<0) | (*(int8_t*)(data_+20) & 0x6 ); }

    inline bool isComplete() const
      { assert(data_); return *(int8_t*)(data_+20) & 0x2; }
    inline void setComplete(bool b)
      { assert(data_); *(int8_t*)(data_+20) = (b<<1) | (*(int8_t*)(data_+20) & 0x5 ); }

    inline bool isCompressed() const
      { assert(data_); return *(int8_t*)(data_+20) & 0x4; }
    inline void setCompressed(bool b)
      { assert(data_); *(int8_t*)(data_+20) = (b<<2) | (*(int8_t*)(data_+20) & 0x3 ); }

    inline void operator=(GamestateHeader& h)
      { assert(data_); assert(h.data_); memcpy( data_, h.data_, getSize()); }
  private:
    uint8_t* data_;

};

/**
    @author Oliver Scheuss
*/
class _NetworkExport Gamestate: public Packet
{
  public:
    Gamestate();
    Gamestate(uint8_t *data, unsigned int clientID);
    Gamestate(uint8_t *data);
    Gamestate(const Gamestate& g);

    ~Gamestate();

    bool collectData(int id, uint8_t mode=0x0);
    bool spreadData( uint8_t mode=0x0);
    inline uint32_t getID() const { return header_.getID(); }
    inline bool isDiffed() const { return header_.isDiffed(); }
    inline bool isCompressed() const { return header_.isCompressed(); }
    inline int32_t getBaseID() const { return header_.getBaseID(); }
    inline uint32_t getDataSize() const { return header_.getDataSize(); }
    Gamestate* diffVariables(Gamestate *base);
//     Gamestate* diffData(Gamestate *base);
//     Gamestate *undiff(Gamestate *base);
//     Gamestate* doSelection(unsigned int clientID, unsigned int targetSize);
    bool compressData();
    bool decompressData();
    bool operator ==(packet::Gamestate gs);

    // Packet functions
  private:
//     void rawDiff( uint8_t* newdata, uint8_t* data, uint8_t* basedata, uint32_t datalength, uint32_t baselength);
//     inline uint32_t findObject( const SynchronisableHeader& header, uint8_t* mem, uint32_t dataLength, uint32_t startPosition = 0 );
    virtual uint32_t getSize() const;
    virtual bool process(orxonox::Host* host);
    uint32_t calcGamestateSize(uint32_t id, uint8_t mode=0x0);
//     inline void diffObject( uint8_t*& newData, uint8_t*& origData, uint8_t*& baseData, SynchronisableHeader& objectHeader, std::vector<uint32_t>::iterator& sizes );
//     inline void copyObject( uint8_t*& newData, uint8_t*& origData, uint8_t*& baseData, SynchronisableHeader& objectHeader, std::vector<uint32_t>::iterator& sizes );
    
    std::list<obj>          dataVector_;
    GamestateHeader         header_;
    std::vector<uint32_t>   sizes_;
    uint32_t                nrOfVariables_;
};

}

}

#endif /* _Gamestate_H__ */
