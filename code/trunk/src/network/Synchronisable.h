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

#include <list>

#include "NetworkPrereqs.h"
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
    const void *var;
    variableType type;
  }SYNCVAR;


  /**
  * This class is the base class of all the Objects in the universe that need to be synchronised over the network
  * Every class, that inherits from this class has to link the DATA THAT NEEDS TO BE SYNCHRONISED into the linked list. Additionally it also has to provide a Constructor, that takes exactly the variables in this linked list.
  * @author Oliver Scheuss
  */
  class _NetworkExport Synchronisable : virtual public orxonox::OrxonoxClass{
  public:

      virtual ~Synchronisable();
    int objectID;
    int classID;

    void registerVar(const void *var, int size, variableType t);
    //  syncData getData();
    syncData getData(unsigned char *mem);
    int getSize();
    bool updateData(syncData vars);
    virtual void registerAllVariables()=0;
    virtual bool create()=0;
  protected:
    Synchronisable();
  private:
    /*  bool removeObject(Iterator<Synchronisable> it);*/

    std::list<SYNCVAR> syncList;
    int datasize;
  };
}

#endif /* _Synchronisable_H__ */
