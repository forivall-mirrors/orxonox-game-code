//
// C++ Implementation: synchronisable
//
// Description: 
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "network/Synchronisable.h"

namespace network {

Synchronisable::Synchronisable()
{
  registerAllVariables();
}


Synchronisable::~Synchronisable()
{
  
}

void Synchronisable::registerVar(const void *var, int size){
  synchronisableVariable temp={size, var};
  syncList.push_back(temp);
}
syncData Synchronisable::getData(){
  std::list<synchronisableVariable>::iterator i;
  int totalsize=0;
  //figure out size of data to be allocated
  for(i=syncList.begin(); i!=syncList.end(); i++){
    // increase size (size of variable and size of size of variable ;)
    totalsize+=sizeof(int)+i->size;
  }
  syncData retVal;
  retVal.objectID=this->objectID;
  retVal.classID=this->classID;
  retVal.length=totalsize;
  // allocate memory
  retVal.data = (unsigned char *)malloc(totalsize);
  // copy to location
  for(int n=0, i=syncList.begin(); n<totalsize && i!=syncList.end(); i++){
    std::memcpy(retVal.data+n, i->size, sizeof(int));
    n+=sizeof(int);
    std::memcpy(retVal.data+n, *(*i.var), *i.size);
    n+=i->size;
  }
  return retVal;
}
bool Synchronisable::updateData(syncData vars){
  unsigned char *data=vars.data;
  std::list<synchronisableVariable>::iterator i;
  for(i=syncList.begin(); i!=syncList.end(); i++){
    if((int)*data==i->size){
      data+=sizeof(int);
      memcpy(i->data, data, i->size);
      data+=i->size;
    } else
      return false; //there was some problem with registerVar
  }
}

}
