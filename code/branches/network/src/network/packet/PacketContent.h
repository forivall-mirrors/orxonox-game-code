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
#ifndef NETWORKPACKETCONTENT_H
#define NETWORKPACKETCONTENT_H

#include <enet/enet.h>
#include <string.h>

#define PACKET_FLAG_DEFAULT ENET_PACKET_FLAG_NO_ALLOCATE

namespace network {

namespace packet{
  
/**
	@author Oliver Scheuss <scheusso [at] ee.ethz.ch>
*/
class PacketContent{
  public:
    virtual ~PacketContent()
    { if(data_) delete[] data_; }
    virtual unsigned char *getData()=0;
    virtual unsigned int getSize() const =0;
    virtual bool process()=0;
    enet_uint32 getFlags(){ return flags_; }
  protected:
    PacketContent()
        { flags_ = PACKET_FLAG_DEFAULT;
        data_=0; }
    PacketContent( unsigned char *data )
        { flags_ = PACKET_FLAG_DEFAULT; data_=data; }
    PacketContent(const PacketContent& p){
      flags_=p.flags_;
      if(p.data_){
        data_ = new unsigned char[p.getSize()];
        memcpy(data_, p.data_, p.getSize());
      }else
        data_=0;
    }
    
    enet_uint32 flags_;
    unsigned char *data_;
  private:
};

} //namespace packet

} //namespace network

#endif
