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
 *      Sandro 'smerkli' Merkli
 *   Co-authors:
 *      ...
 *
 */

#ifndef _MasterServer_H__
#define _MasterServer_H__

#include "NetworkPrereqs.h"

/* orxonox includes */
#include <enet/enet.h>
#include <network/packet/Chat.h>
#include <network/packet/ClassID.h>
#include <network/packet/DeleteObjects.h>
#include <network/packet/FunctionIDs.h>
#include <network/packet/Gamestate.h>
#include <network/packet/Welcome.h>
#include <network/MasterServerProtocol.h>

/* my includes */
#include "ServerList.h"
#include "PeerList.h"

/* c compatibility */
#include <cstdio>

#include <OgreStringConverter.h>


namespace orxonox
{
  /* singleton */
  class _NetworkExport MasterServer
  {
    public:
      MasterServer();
      ~MasterServer();

      /* main routine */
      int run();

      /* static pointer for commands */
      static MasterServer *instance;
      static MasterServer *getInstance()
        { return instance; }
      static void setInstance( MasterServer *setto )
        { instance = setto;  }

      /* functions for commands */
      static void listServers( void );
      static void delServer( std::string todeladdr );

    private:
      /* methods */
      int eventConnect( ENetEvent *event );
      int eventDisconnect( ENetEvent *event );
      int eventData( ENetEvent *event );

      /* helpers */
      void helper_sendlist( ENetEvent *event );
      void helper_cleanupServers( void );

      /* members */
      ENetAddress address;
      ENetHost *server;
      ServerList mainlist;

      unsigned int port;
      bool quit;

  };
}

#endif /* _MasterServer_H__ */
