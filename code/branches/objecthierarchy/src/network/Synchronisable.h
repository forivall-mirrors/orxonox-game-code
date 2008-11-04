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
#include "util/Integers.h"
#include "core/OrxonoxClass.h"
#include "core/XMLIncludes.h"
#include "NetworkCallback.h"
#include "util/Integers.h"

#define REGISTERDATA(varname, ...) \
    registerVar((void*)&varname, sizeof(varname), DATA, __VA_ARGS__)
#define REGISTERSTRING(stringname, ...) \
    registerVar(&stringname, stringname.length()+1, STRING, __VA_ARGS__)

namespace orxonox
{
  static const unsigned int OBJECTID_UNKNOWN = (unsigned int)-1;

  namespace direction{
    enum syncdirection{
      toclient=0x1,
      toserver=0x2,
      bidirectional=0x3,
      serverMaster=0x3,
      clientMaster=0x7
    };
  }

  namespace syncmode{
    enum mode{
      once=0,
      always=1
    };
  }

  enum variableType{
    DATA,
    STRING,
  };

  struct _NetworkExport synchronisableHeader{
    uint32_t size:31;
    bool dataAvailable:1;
    uint32_t objectID;
    uint32_t creatorID;
    uint32_t classID;
  };

  struct _NetworkExport synchronisableVariable{
    size_t size;
    uint8_t mode; // this determines in which direction the variable gets synchronised
    void *var;
    variableType type;
    NetworkCallbackBase *callback;
    void *varBuffer;
    uint8_t varReference;
  };

  /**
  * This class is the base class of all the Objects in the universe that need to be synchronised over the network
   * Every class, that inherits from this class has to link the DATA THAT NEEDS TO BE SYNCHRONISED into the linked list.
  * @author Oliver Scheuss
  */
  class _NetworkExport Synchronisable : virtual public OrxonoxClass{
  public:
    friend class packet::Gamestate;
//     friend class Server;
    virtual ~Synchronisable();


    virtual bool create();
    static void setClient(bool b);

    static Synchronisable *fabricate(uint8_t*& mem, uint8_t mode=0x0);
    static bool deleteObject(unsigned int objectID);
    static Synchronisable *getSynchronisable(unsigned int objectID);
    static unsigned int getNumberOfDeletedObject(){ return deletedObjects_.size(); }
    static unsigned int popDeletedObject(){ unsigned int i = deletedObjects_.front(); deletedObjects_.pop(); return i; }

    inline unsigned int getObjectID(){return objectID;}
    inline unsigned int getClassID(){return classID;}
  protected:
    Synchronisable(BaseObject* creator);
    void registerVar(void *var, int size, variableType t, uint8_t mode=0x1, NetworkCallbackBase *cb=0);
    void setObjectMode(uint8_t mode);
    void setObjectFrequency(unsigned int freq){ objectFrequency_ = freq; }


  private:
    bool getData(uint8_t*& men, unsigned int id, uint8_t mode=0x0);
    uint32_t getSize(unsigned int id, uint8_t mode=0x0);
    bool updateData(uint8_t*& mem, uint8_t mode=0x0, bool forceCallback=false);
    bool isMyData(uint8_t* mem);
    bool doSelection(unsigned int id);
    bool doSync(unsigned int id, uint8_t mode=0x0);

    unsigned int objectID;
    unsigned int creatorID;
    unsigned int classID;

    std::list<synchronisableVariable *> *syncList;
    static uint8_t state_; // detemines wheter we are server (default) or client
    bool backsync_; // if true the variables with mode > 1 will be synchronised to server (client -> server)
    unsigned int objectFrequency_;
    int objectMode_;
    static std::map<unsigned int, Synchronisable *> objectMap_;
    static std::queue<unsigned int> deletedObjects_;
  };
}

#endif /* _Synchronisable_H__ */
