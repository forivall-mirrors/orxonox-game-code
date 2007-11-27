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

namespace network {

/**
 * Constructor:
 * calls registarAllVariables, that has to be implemented by the inheriting classID
 */
Synchronisable::Synchronisable()
{
  datasize=0;
  registerAllVariables();
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
void Synchronisable::registerVar(const void *var, int size){
  // create temporary synch.Var struct
  synchronisableVariable temp={size, var};
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
  //CHANGED: REMOVED DECLARATION int n=0 FROM LOOP
  int n=0;
  for(i=syncList.begin(); n<totalsize && i!=syncList.end(); i++){
	//CHANGED: i->size TO (const void*)(&(i->size)) memcpy WANTS A CONST VOID* SO CONVERT INT TO CONST VOID*
    std::memcpy(retVal.data+n, (const void*)(i->size), sizeof(int));
    n+=sizeof(int);
    //CHANGED: i->var TO (const void*)(&(i->var)) SINCE var IS A POINTER, NO & BEFORE i
    std::memcpy(retVal.data+n, (const void*)(i->var), i->size);
    n+=i->size;
  }
  return retVal;
}
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
  retVal.length=datasize;
  retVal.data=mem;
  // copy to location
  //CHANGED: REMOVED DECLARATION int n=0 FROM LOOP
  int n=0;
  for(i=syncList.begin(); n<datasize && i!=syncList.end(); i++){
        //CHANGED: i->size TO (const void*)(&(i->size)) memcpy WANTS A CONST VOID* SO CONVERT INT TO CONST VOID*
    std::memcpy(retVal.data+n, (const void*)(i->size), sizeof(int));
    n+=sizeof(int);
    //CHANGED: i->var TO (const void*)(&(i->var)) SINCE var IS A POINTER, NO & BEFORE i
    std::memcpy(retVal.data+n, (const void*)(i->var), i->size);
    n+=i->size;
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
    if((int)*data==i->size){
      data+=sizeof(int);
      //CHANGED: THIS FROM i->var TO (void*)i->var SINCE var IS A CONST VOID* AND memcpy NEEDS A VOID* AS FIRST ARGUMENT
      memcpy((void*)i->var, data, i->size);
      data+=i->size;
    } else
      return false; //there was some problem with registerVar
  }
}

/**
 * This function returns the total amount of bytes needed by getData to save the whole content of the variables
 * @return amount of bytes
 */
int Synchronisable::getSize(){
  return datasize;
}

}
