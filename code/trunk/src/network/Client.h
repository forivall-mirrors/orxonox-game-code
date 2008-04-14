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
// C++ Interface: Client
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _Client_H__
#define _Client_H__

#include "NetworkPrereqs.h"

#include <string>

#include "core/Tickable.h"
#include "ClientConnection.h"
#include "PacketManager.h"
#include "GameStateClient.h"
//#include "NetworkFrameListener.h"



namespace network
{
  /**
  network::Client *client;
  * The network/Client class
  * This class implements all necessary function for the network communication
  * It is the root class of the network module
  *
  */
  class _NetworkExport Client : PacketDecoder, public orxonox::Tickable{
  public:
    Client();
    Client(std::string address, int port);
    Client(const char *address, int port);

    bool establishConnection();
    bool closeConnection();

    bool sendMouse(double x, double y);
    bool sendKeyboard(char key_code);
    bool sendChat( std::string message );

    bool addMouse(double x, double y);
    bool addKeyboard(char key_code);

    bool sendPackets();

    void tick(float time);

  private:
    ClientConnection client_connection;
    PacketGenerator pck_gen;
    GameStateClient gamestate;
    bool isConnected;

    // implement data processing functions of PacketDecoder
    void processGamestate( GameStateCompressed *data);
    void processClassid(classid *clid);
    void processChat( chat *data);
  };


}

#endif /* _Client_H__ */
