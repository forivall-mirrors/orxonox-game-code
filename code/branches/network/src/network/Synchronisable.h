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
#ifndef _Synchronisable_H__
#define _Synchronisable_H__

#include "NetworkPrereqs.h"

#include <list>
#include "core/OrxonoxClass.h"

namespace network
{
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
    int mode; // this determines in which direction the variable gets synchronised
    void *var;
    variableType type;
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
    int objectID;
    int classID;

    void registerVar(void *var, int size, variableType t, int mode=1);
    //  syncData getData();
    syncData getData(unsigned char *mem);
    int getSize();
    bool updateData(syncData vars);
    void setBacksync(bool sync);
    bool getBacksync();
    virtual void registerAllVariables()=0;
    virtual bool create();
    static void setClient(bool b);
  protected:
    Synchronisable();
  private:
    /*  bool removeObject(Iterator<Synchronisable> it);*/

    std::list<synchronisableVariable *> *syncList;
    int datasize;
    static int state_; // detemines wheter we are server (default) or client
    bool backsync_; // if true the variables with mode > 1 will be synchronised to server (client -> server)
  };
}

#endif /* _Synchronisable_H__ */
