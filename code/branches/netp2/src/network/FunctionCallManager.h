
#ifndef NETWORKFUNCTIONCALLMANAGER_H
#define NETWORKFUNCTIONCALLMANAGER_H

#include "NetworkPrereqs.h"
#include "packet/FunctionCalls.h"
#include <map>


namespace orxonox {
/**
	@author
*/

class MultiType;

class _NetworkExport FunctionCallManager
{
public:
  static void addCallStatic(uint32_t functionID, uint32_t clientID, MultiType* mt1=0, MultiType* mt2=0, MultiType* mt3=0, MultiType* mt4=0, MultiType* mt5=0);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, MultiType* mt1=0, MultiType* mt2=0, MultiType* mt3=0, MultiType* mt4=0, MultiType* mt5=0);
  static void sendCalls();
  
  static std::map<uint32_t, packet::FunctionCalls*> clientMap_;
protected:
  FunctionCallManager();
  ~FunctionCallManager();
};

} //namespace orxonox

#endif
