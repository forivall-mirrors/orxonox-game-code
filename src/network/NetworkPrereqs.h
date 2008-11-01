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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
  @file
  @brief Contains all the necessary forward declarations for all classes and structs.
*/

#ifndef _NetworkPrereqs_H__
#define _NetworkPrereqs_H__

#include "util/OrxonoxPlatform.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------
#if (ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32) && !defined( NETWORK_STATIC_BUILD )
#  ifdef NETWORK_SHARED_BUILD
#    define _NetworkExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _NetworkExport
#    else
#      define _NetworkExport __declspec(dllimport)
#    endif
#  endif
#elif defined ( ORXONOX_GCC_VISIBILITY )
#  define _NetworkExport  __attribute__ ((visibility("default")))
#else
#  define _NetworkExport
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------
namespace network
{
  class Client;
  class ClientConnection;
  class ClientConnectionListener;
  class ClientFrameListener;
  class ClientInformation;
  class ConnectionManager;
  class GamestateClient;
  class GamestateManager;
  class GamestateHandler;
  class PacketBuffer;
  class Server;
  class ServerFrameListener;
  class Synchronisable;
  struct ClientList;
  struct PacketEnvelope;
  struct QueueItem;
  struct syncData;
  struct synchronisableVariable;
  namespace packet{
    class Gamestate;
    class Packet;
    class Acknowledgement;
    class ClassID;
    class Welcome;
    class Chat;
  }
}

#endif /* _NetworkPrereqs_H__ */
