 #
 #             ORXONOX - the hottest 3D action shooter ever to exist
 #                             > www.orxonox.net <
 #
 #        This program is free software; you can redistribute it and/or
 #         modify it under the terms of the GNU General Public License
 #        as published by the Free Software Foundation; either version 2
 #            of the License, or (at your option) any later version.
 #
 #       This program is distributed in the hope that it will be useful,
 #        but WITHOUT ANY WARRANTY; without even the implied warranty of
 #        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #                 GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License along
 #      with this program; if not, write to the Free Software Foundation,
 #     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #

SET_SOURCE_FILES(NETWORK_SRC_FILES
  Client.cc
  ClientConnection.cc
  ClientConnectionListener.cc
  Connection.cc
  FunctionCall.cc
  FunctionCallManager.cc
  GamestateManager.cc
  #GamestateClient.cc
  GamestateHandler.cc
  LANDiscoverable.cc
  LANDiscovery.cc
  WANDiscoverable.cc
  WANDiscovery.cc
  MasterServerComm.cc
  NetworkFunction.cc
  NetworkFunctionManager.cc
  Host.cc
  Server.cc
  MasterServer.cc
  PeerList.cc
  ServerList.cc
  ServerConnection.cc
  TrafficControl.cc
)

ADD_SUBDIRECTORY(packet)
ADD_SUBDIRECTORY(synchronisable)

ORXONOX_ADD_LIBRARY(network
  FIND_HEADER_FILES
  TOLUA_FILES
    Client.h
    LANDiscovery.h
    WANDiscovery.h
  PCH_FILE
    NetworkPrecompiledHeaders.h
  LINK_LIBRARIES
    ${ZLIB_LIBRARY}
    #${ENET_LIBRARY}
    enet_orxonox
    ${Boost_THREAD_LIBRARY}
    ${CEGUI_TOLUA_LIBRARY}
    util
    core
  SOURCE_FILES
    ${NETWORK_SRC_FILES}
)
