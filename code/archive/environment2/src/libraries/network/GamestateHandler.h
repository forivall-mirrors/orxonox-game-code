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

#ifndef _GamestateHandler_H__
#define _GamestateHandler_H__

#include "NetworkPrereqs.h"

#include <cassert>

namespace orxonox {

/**
    @author Oliver Scheuss
*/
class _NetworkExport GamestateHandler
{
  private:


  protected:
    GamestateHandler();
    virtual ~GamestateHandler();

  public:
    virtual bool      addGamestate(packet::Gamestate* gs, unsigned int clientID) = 0;
    virtual bool      ackGamestate(unsigned int gamestateID, unsigned int clientID) = 0;
    virtual uint32_t  getLastReceivedGamestateID( unsigned int clientID )=0;
    virtual uint32_t  getCurrentGamestateID()=0;
};

}

#endif /* _GamestateHandler_H__ */
