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
 *      Oliver Scheuss, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Synchronisable_H__
#define _Synchronisable_H__

#include "NetworkPrereqs.h"

#include <list>
#include "core/OrxonoxClass.h"
#include "NetworkCallback.h"

namespace network
{
  enum variableType{
    DATA,
    STRING,
  };

  struct synchronisableHeader{
    unsigned int size;
    unsigned int objectID;
    unsigned int classID;
  };

  typedef struct synchronisableVariable{
    unsigned int size;
    int mode; // this determines in which direction the variable gets synchronised
    void *var;
    variableType type;
    NetworkCallbackBase *callback;
  }SYNCVAR;


  /**
  * This class is the base class of all the Objects in the universe that need to be synchronised over the network
   * Every class, t
  int mode;hat inherits from this class has to link the DATA THAT NEEDS TO BE SYNCHRONISED into the linked list. Additionally it also has to provide a Constructor, that takes exactly the variables in this linked list.
  * @author Oliver Scheuss
  */
  class _NetworkExport Synchronisable : virtual public orxonox::OrxonoxClass{
  public:

    virtual ~Synchronisable();
    unsigned int objectID;
    unsigned int classID;

    void registerVar(void *var, int size, variableType t, int mode=1, NetworkCallbackBase *cb=0);
    bool getData(unsigned char*& men, unsigned int id, int mode=0x0);
    int getSize2(unsigned int id, int mode=0x0);
    bool updateData(unsigned char*& mem, int mode=0x0);
    bool isMyData(unsigned char* mem);
    void setBacksync(bool sync);
    bool getBacksync();
    virtual void registerAllVariables()=0;
    virtual bool create();
    static void setClient(bool b);
    
    static bool fabricate(unsigned char*& mem, int mode=0x0);
  protected:
    Synchronisable();
  private:
    int getSize(unsigned int id, int mode=0x0);
    bool isMyTick(unsigned int id);
    std::list<synchronisableVariable *> *syncList;
    int datasize;
    static int state_; // detemines wheter we are server (default) or client
    bool backsync_; // if true the variables with mode > 1 will be synchronised to server (client -> server)
    unsigned int objectFrequency_;
  };
}

#endif /* _Synchronisable_H__ */
