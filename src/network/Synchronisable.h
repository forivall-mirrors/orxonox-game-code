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
#include <map>
#include <queue>
#include "core/OrxonoxClass.h"
#include "core/XMLIncludes.h"
#include "NetworkCallback.h"

#define REGISTERDATA(varname) registerVar(&varname, sizeof(varname), network::DATA)
#define REGISTERDATA_WITHDIR(varname, mode) registerVar(&varname, sizeof(varname), network::DATA, mode)
#define REGISTERSTRING(stringname) registerVar(&stringname, stringname.length()+1, network::STRING)
#define REGISTERSTRING_WITHDIR(stringname, mode) registerVar(&stringname, stringname.length()+1, network::STRING, mode)

//TODO: this is only a very ugly hack ...
namespace orxonox{
class SpaceShip;
}

namespace network
{
  namespace direction{
    enum syncdirection{
      toclient=0x1,
      toserver=0x2,
      bidirectional=0x3
    };
  }
  
  namespace syncmode{
    enum mode{
      one=0,
      always=1
    };
  }
  
  enum variableType{
    DATA,
    STRING,
  };

  struct synchronisableHeader{
    uint32_t size:31;
    bool dataAvailable:1;
    uint32_t objectID;
    uint32_t classID;
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
   * Every class, that inherits from this class has to link the DATA THAT NEEDS TO BE SYNCHRONISED into the linked list.
  * @author Oliver Scheuss
  */
  class _NetworkExport Synchronisable : virtual public orxonox::OrxonoxClass{
  public:
    friend class packet::Gamestate;
    friend class GamestateClient;
    friend class Server;
    friend class orxonox::SpaceShip;
    virtual ~Synchronisable();

    
    virtual bool create();
    static void setClient(bool b);
    
    static Synchronisable *fabricate(uint8_t*& mem, int mode=0x0);
    static bool deleteObject(unsigned int objectID);
    static Synchronisable *getSynchronisable(unsigned int objectID);
    static unsigned int getNumberOfDeletedObject(){ return deletedObjects_.size(); }
    static unsigned int popDeletedObject(){ unsigned int i = deletedObjects_.front(); deletedObjects_.pop(); return i; }
    
    inline unsigned int getObjectID(){return objectID;}
    inline unsigned int getClassID(){return classID;}
  protected:
    Synchronisable();
    void registerVar(void *var, int size, variableType t, int mode=1, NetworkCallbackBase *cb=0);
    void setObjectMode(int mode);
    void setObjectFrequency(unsigned int freq){ objectFrequency_ = freq; }
    virtual void registerAllVariables()=0;
    
    
  private:
    bool getData(uint8_t*& men, unsigned int id, int mode=0x0);
    uint32_t getSize(unsigned int id, int mode=0x0);
    bool updateData(uint8_t*& mem, int mode=0x0);
    bool isMyData(uint8_t* mem);
    bool doSelection(unsigned int id);
    bool isMyTick(unsigned int id);
    
    unsigned int objectID;
    unsigned int classID;
    
    std::list<synchronisableVariable *> *syncList;
    static int state_; // detemines wheter we are server (default) or client
    bool backsync_; // if true the variables with mode > 1 will be synchronised to server (client -> server)
    unsigned int objectFrequency_;
    int objectMode_;
    static std::map<unsigned int, Synchronisable *> objectMap_;
    static std::queue<unsigned int> deletedObjects_;
  };
}

#endif /* _Synchronisable_H__ */
