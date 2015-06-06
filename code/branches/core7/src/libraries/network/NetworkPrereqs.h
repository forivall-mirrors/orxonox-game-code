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
@brief
    Shared library macros, enums, constants and forward declarations for the network library
*/

#ifndef _NetworkPrereqs_H__
#define _NetworkPrereqs_H__

#include "OrxonoxConfig.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined( NETWORK_STATIC_BUILD )
#  ifdef NETWORK_SHARED_BUILD
#    define _NetworkExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _NetworkExport
#    else
#      define _NetworkExport __declspec(dllimport)
#    endif
#  endif
#  define _NetworkPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _NetworkExport  __attribute__ ((visibility("default")))
#  define _NetworkPrivate __attribute__ ((visibility("hidden")))
#else
#  define _NetworkExport
#  define _NetworkPrivate
#endif

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

namespace orxonox
{
  static const unsigned int GAMESTATEID_INITIAL       = static_cast<unsigned int>(-1);
  extern const char* LAN_DISCOVERY_MESSAGE;
  extern const char* LAN_DISCOVERY_ACK;
  static const unsigned int LAN_DISCOVERY_PORT          = 55558;
  static const unsigned int NETWORK_PEER_ID_SERVER      = 0;
  static const unsigned int NETWORK_PEER_ID_BROADCAST   = static_cast<unsigned int>(-1);
  static const unsigned int NETWORK_PEER_ID_UNKNOWN     = static_cast<unsigned int>(-2);
  static const unsigned int NETWORK_CHANNEL_DEFAULT     = 0;
  static const unsigned int NETWORK_CHANNEL_UNRELIABLE  = 1;
  static const unsigned int NETWORK_CHANNEL_COUNT       = 2;
}

//-----------------------------------------------------------------------
// Enums
//-----------------------------------------------------------------------

namespace orxonox
{
  namespace packet
  {
    namespace PacketFlag
    {
      enum Value
      {
        Reliable    = 1,
        Unsequenced = 2,
        NoAllocate  = 4
      };
    }
  }

  namespace StaticInitialization
  {
    typedef int Type;
    static const Type NETWORK_FUNCTION = 6;
  }
}

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

// from ENet
struct _ENetPeer;
typedef _ENetPeer     ENetPeer;
struct _ENetPacket;
typedef _ENetPacket   ENetPacket;
struct _ENetEvent;
typedef _ENetEvent    ENetEvent;
struct _ENetHost;
typedef _ENetHost     ENetHost;
struct _ENetAddress;
typedef _ENetAddress  ENetAddress;
typedef uint8_t       ENetChannelID;

namespace orxonox
{
  class Client;
  class ClientConnection;
  class ClientConnectionListener;
  class ClientInformation;
  class Connection;
  class FunctionCall;
  class FunctionCallManager;
  class GamestateClient;
  class GamestateHandler;
  class GamestateManager;
  class Host;
  class MasterServer;
  class MasterServerComm;
  class NetworkChatListener;
  class NetworkFunctionBase;
  struct NetworkFunctionPointer;
  class NetworkFunctionStatic;
  template <class T>
  class NetworkMemberFunction;
  class NetworkMemberFunctionBase;
  class PeerList;
  class Server;
  class ServerConnection;
  class TrafficControl;
  class WANDiscoverable;
  class WANDiscovery;

  // packet
  namespace packet
  {
    class Acknowledgement;
    class Chat;
    class ClassID;
    class DeleteObjects;
    class FunctionCalls;
    class FunctionIDs;
    class Gamestate;
    class Packet;
    class Welcome;
  }

  // synchronisable
  template <class T>
  class NetworkCallback;
  template <class T, class U>
  class NetworkCallbackNotify;
  class NetworkCallbackBase;
  class NetworkCallbackManager;
  class Synchronisable;
  class SynchronisableHeader;
  template <class T>
  class SynchronisableVariable;
  class SynchronisableVariableBase;
  template <class T>
  class SynchronisableVariableBidirectional;
}

namespace boost
{
  class mutex;
  class thread;
}

#endif /* _NetworkPrereqs_H__ */
