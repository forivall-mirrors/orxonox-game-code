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
#elif defined ( ORXONOX_GCC_VISIBILITY )
#  define _NetworkExport  __attribute__ ((visibility("default")))
#else
#  define _NetworkExport
#endif

//-----------------------------------------------------------------------
// Library global contants
//-----------------------------------------------------------------------
namespace orxonox
{
  static const unsigned int GAMESTATEID_INITIAL = (unsigned int)-1;
  static const unsigned int CLIENTID_UNKNOWN    = (unsigned int)-2;
  static const uint32_t     OBJECTID_UNKNOWN    = (uint32_t)(-1);
}

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

// from ENet
struct _ENetPeer;
typedef _ENetPeer ENetPeer;
struct _ENetPacket;
typedef _ENetPacket ENetPacket;
struct _ENetEvent;
typedef _ENetEvent ENetEvent;
struct _ENetHost;
typedef _ENetHost ENetHost;
struct _ENetAddress;
typedef _ENetAddress ENetAddress;

namespace orxonox
{
  class Client;
  class ClientConnection;
  class ClientConnectionListener;
  class ClientFrameListener;
  class ClientInformation;
  class Connection;
  class FunctionCallManager;
  class GamestateClient;
  class GamestateManager;
  class GamestateHandler;
  class NetworkCallbackBase;
  template <class T> class NetworkCallback;
  class NetworkCallbackManager;
  class NetworkFunctionBase;
  class NetworkFunctionStatic;
  class NetworkMemberFunctionBase;
  template <class T> class NetworkMemeberFunction;
  struct NetworkFunctionPointer;
  class PacketBuffer;
  class Server;
  class ServerConnection;
  class ServerFrameListener;
  class Synchronisable;
  class SynchronisableVariableBase;
  template <class T> class SynchronisableVariable;
  template <class T> class SynchronisableVariableBidirectional;
  struct ClientList;
  struct PacketEnvelope;
  struct QueueItem;
  struct syncData;
  class TrafficControl;
  class obj;
  class objInfo;

  namespace packet
  {
    class Acknowledgement;
    class Chat;
    class ClassID;
    class FunctionCalls;
    class FunctionIDs;
    class Gamestate;
    class NetworkIDs;
    class Packet;
    class Welcome;

    namespace PacketFlag
    {
      enum Value
      {
        Reliable   = 1,
        Unsequence = 2,
        NoAllocate = 4
      };
    }
  }
}

#endif /* _NetworkPrereqs_H__ */
