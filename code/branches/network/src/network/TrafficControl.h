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
 *      Oliver Scheuss <scheusso [at] ee.ethz.ch>, (C) 2008
 *   Co-authors:
 *      ...
 *
 */
#ifndef NETWORKTRAFFICCONTROL_H
#define NETWORKTRAFFICCONTROL_H

#include <string>
#include <vector>
#include <map>

#include "NetworkPrereqs.h"
#include "Synchronisable.h"

namespace network {

/**
*
*/
class _NetworkExport Host{
  private:



  protected:
    TrafficControl();
    virtual ~TrafficControl();
    static TrafficControl *instance_;

  public:
    void processObjectList(unsigned int clientID, unsigned int gamestateID, std::vector<unsigned int> *list); //gets a pointer to the vector (containing objectIDs) and sorts it
    void processAck(unsigned int clientID, unsigned int gamestateID);	// this function gets called when the server receives an ack from the client
    void deleteObject(unsigned int objectID);				// this function gets called when an object has been deleted (in order to clean up lists and maps)
};

}

#endif

