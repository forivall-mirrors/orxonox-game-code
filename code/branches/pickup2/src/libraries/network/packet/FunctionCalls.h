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

#ifndef _FunctionCalls_H__
#define _FunctionCalls_H__

#include "network/NetworkPrereqs.h"

#include <cassert>
#include "util/UtilPrereqs.h"
#include "Packet.h"

namespace orxonox {

namespace packet {
/**
    @author
*/


class _NetworkExport FunctionCalls : public Packet
{
public:
  FunctionCalls();
  FunctionCalls( uint8_t* data, unsigned int clientID );
  ~FunctionCalls();

  inline unsigned int getSize() const
    { assert(!this->isDataENetAllocated()); return currentSize_; }
  bool process();

  void addCallStatic( uint32_t networkID, const MultiType* mt1=0, const MultiType* mt2=0, const MultiType* mt3=0, const MultiType* mt4=0, const MultiType* mt5=0);
  void addCallMember( uint32_t networkID, uint32_t objectID, const MultiType* mt1=0, const MultiType* mt2=0, const MultiType* mt3=0, const MultiType* mt4=0, const MultiType* mt5=0);
private:
  uint32_t nrOfCalls_;
  unsigned int clientID_;
  uint32_t currentSize_;
  uint32_t currentMemBlocks_; // this saves the number of memory blocks (of size FUNCTIONCALLS_MEM_ALLOCATION) allocated
};

} //namespace packet
} //namespace orxonox

#endif /* _FunctionCalls_H__ */
