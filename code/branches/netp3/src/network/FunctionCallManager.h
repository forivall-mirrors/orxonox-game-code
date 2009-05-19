
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
  static void addCallStatic(uint32_t functionID, uint32_t clientID);
  static void addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1);
  static void addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2);
  static void addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3);
  static void addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4);
  static void addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5);
  
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5);
  
  static void sendCalls();
  
  static std::map<uint32_t, packet::FunctionCalls*> clientMap_;
protected:
  FunctionCallManager();
  ~FunctionCallManager();
};

} //namespace orxonox

#endif
