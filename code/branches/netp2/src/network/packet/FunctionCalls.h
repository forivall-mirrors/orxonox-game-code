
#ifndef NETWORKFUNCTIONCALLS_H
#define NETWORKFUNCTIONCALLS_H

#include "../NetworkPrereqs.h"

#include <string>
#include <cstring>

#include "Packet.h"
#include <cassert>

namespace orxonox {

class MultiType;

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

  void addCallStatic( uint32_t networkID, MultiType* mt1=0, MultiType* mt2=0, MultiType* mt3=0, MultiType* mt4=0, MultiType* mt5=0);
  void addCallMember( uint32_t networkID, uint32_t objectID, MultiType* mt1=0, MultiType* mt2=0, MultiType* mt3=0, MultiType* mt4=0, MultiType* mt5=0);
private:
  uint32_t nrOfCalls_;
  unsigned int clientID_;
  uint32_t currentSize_;
  uint32_t currentMemBlocks_; // this saves the number of memory blocks (of size FUNCTIONCALLS_MEM_ALLOCATION) allocated
};

} //namespace packet
} //namespace orxonox

#endif
