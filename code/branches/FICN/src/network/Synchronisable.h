//
// C++ Interface: synchronisable
//
// Description: 
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NETWORK_SYNCHRONISABLE_H
#define NETWORK_SYNCHRONISABLE_H

#include <list>
#include <iostream>
#include <string>

#include "orxonox/core/CoreIncludes.h"
#include "orxonox/core/OrxonoxClass.h"

namespace network {

enum variableType{
  DATA,
  STRING,
};
  
struct syncData{
  int length;
  int objectID;
  int classID;
  unsigned char *data;
};

typedef struct synchronisableVariable{
  int size;
  const void *var;
  variableType type;
}SYNCVAR;

  
/**
 * This class is the base class of all the Objects in the universe that need to be synchronised over the network
 * Every class, that inherits from this class has to link the DATA THAT NEEDS TO BE SYNCHRONISED into the linked list. Additionally it also has to provide a Constructor, that takes exactly the variables in this linked list.
 * @author Oliver Scheuss
*/
class Synchronisable : virtual public orxonox::OrxonoxClass{
public:

  virtual ~Synchronisable();
  int objectID;
  int classID;
    
  void registerVar(const void *var, int size, variableType t);
  syncData getData();
  syncData getData(unsigned char *mem);
  int getSize();
  bool updateData(syncData vars);
  virtual void registerAllVariables()=0;
  virtual bool create(){return true;}
protected:
  Synchronisable();
private:
/*  bool removeObject(Iterator<Synchronisable> it);*/
  
  std::list<SYNCVAR> syncList;
  int datasize;
};

}

#endif
