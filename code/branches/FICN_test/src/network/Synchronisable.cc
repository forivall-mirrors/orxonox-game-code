//
// C++ Implementation: synchronisable
//
// Description:
//
//
// Author:  Dumeni, Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//

#include "Synchronisable.h"
#include "orxonox/core/CoreIncludes.h"


namespace network {

/**
 * Constructor:
 * calls registarAllVariables, that has to be implemented by the inheriting classID
 */
Synchronisable::Synchronisable()
{
  RegisterRootObject(Synchronisable);
  static int idCounter=0;
  datasize=0;
  objectID=idCounter++;
  //registerAllVariables();
}


Synchronisable::~Synchronisable()
{

}

/**
 * This function is used to register a variable to be synchronized
 * also counts the total datasize needed to save the variables
 * @param var pointer to the variable
 * @param size size of the datatype the variable consists of
 */
void Synchronisable::registerVar(const void *var, int size, variableType t){
  // create temporary synch.Var struct
  synchronisableVariable temp={size, var, t};
  // increase datasize
  datasize+=sizeof(int)+size;
  // push temp to syncList (at the bottom)
  syncList.push_back(temp);
}

/**
 * note: only use this function for debug use, because it's inefficient (in order to produce a gamestate, you have to copy the whole data again to another memory location after this process)
 * This function takes all SynchronisableVariables out of the Synchronisable and saves it into a syncData struct
 * structure of the bitstream:
 * (var1_size,var1,var2_size,var2,...)
 * varx_size: size = sizeof(int)
 * varx: size = varx_size
 * @return data containing all variables and their sizes
 */
// syncData Synchronisable::getData(){
//   std::list<synchronisableVariable>::iterator i;
//   int totalsize=0;
//   //figure out size of data to be allocated
//   for(i=syncList.begin(); i!=syncList.end(); i++){
//     // increase size (size of variable and size of size of variable ;)
//     if(i->type == STRING)
//       totalsize+=sizeof(int)+((std::string *)i->var)->length()+1;
//     else
//       totalsize+=sizeof(int)+i->size;
//   }
//   syncData retVal;
//   retVal.objectID=this->objectID;
//   retVal.classID=this->classID;
//   retVal.length=totalsize;
//   // allocate memory
//   retVal.data = (unsigned char *)malloc(totalsize);
//   // copy to location
//   //CHANGED: REMOVED DECLARATION int n=0 FROM LOOP
//   int n=0;
//   for(i=syncList.begin(); n<totalsize && i!=syncList.end(); i++){
//     std::memcpy(retVal.data+n, (const void*)(i->size), sizeof(int));
//     n+=sizeof(int);
//     switch(i->type){
//     case STRING:
//       std::memcpy(retVal.data+n, (const void *)(((std::string *)i->var)->c_str()), ((std::string *)i->var)->length()+1);
//       n+=((std::string *)i->var)->length()+1;
//       break;
//     case DATA:
//       std::memcpy(retVal.data+n, ((const void*)i->var), i->size);
//       n+=i->size;
//       break;
//     }
//   }
//   return retVal;
// }
/**
 * This function takes all SynchronisableVariables out of the Synchronisable and saves it into a syncData struct
 * Difference to the above function:
 * takes a pointer to already allocated memory (must have at least getSize bytes length)
 * structure of the bitstream:
 * (var1_size,var1,var2_size,var2,...)
 * varx_size: size = sizeof(int)
 * varx: size = varx_size
 * @return data containing all variables and their sizes
 */
syncData Synchronisable::getData(unsigned char *mem){
  std::list<synchronisableVariable>::iterator i;
  syncData retVal;
  retVal.objectID=this->objectID;
  retVal.classID=this->classID;
  retVal.length=getSize();
  retVal.data=mem;
  // copy to location
  int n=0;
  for(i=syncList.begin(); n<datasize && i!=syncList.end(); ++i){
    //COUT(2) << "size of variable: " << i->size << std::endl;
    //(std::memcpy(retVal.data+n, (const void*)(&(i->size)), sizeof(int));
    memcpy( (void *)(retVal.data+n), (const void*)&(i->size), sizeof(int) );
    n+=sizeof(int);
    switch(i->type){
      case DATA:
        std::memcpy( (void *)(retVal.data+n), (const void*)(i->var), i->size);
        n+=i->size;
        break;
      case STRING:
        std::memcpy( retVal.data+n, (const void*)( ( (std::string *) i->var)->c_str()), ( (std::string *)i->var )->length()+1);
        n+=((std::string *) i->var)->length()+1;
        break;
    }
  }
  return retVal;
}

/**
 * This function takes a syncData struct and takes it to update the variables
 * @param vars data of the variables
 * @return true/false
 */
bool Synchronisable::updateData(syncData vars){
  unsigned char *data=vars.data;
  std::list<synchronisableVariable>::iterator i;
  for(i=syncList.begin(); i!=syncList.end(); i++){
    if((int)*data==i->size || i->type==STRING){
      switch(i->type){
      case DATA:
        data+=sizeof(int);
        memcpy((void*)i->var, data, i->size);
        data+=i->size;
        break;
      case STRING:
        i->size = (int)*data;
        data+=sizeof(int);
        *((std::string *)i->var) = std::string((const char*)data);
        data += i->size;
        break;
      }
    } else
      return false; //there was some problem with registerVar
  }
  return true;
}

/**
 * This function returns the total amount of bytes needed by getData to save the whole content of the variables
 * @return amount of bytes
 */
int Synchronisable::getSize(){
  int tsize=0;
  std::list<synchronisableVariable>::iterator i;
  for(i=syncList.begin(); i!=syncList.end(); i++){
    switch(i->type){
    case DATA:
      tsize+=sizeof(int);
      tsize+=i->size;
      break;
    case STRING:
      tsize+=sizeof(int);
      tsize+=((std::string *)i->var)->length()+1;
      break;
    }
  }
  return tsize;
}

}
