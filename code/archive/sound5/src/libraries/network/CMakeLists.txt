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
  ChatListener.cc
  Client.cc
  ClientConnection.cc
  ClientInformation.cc
  ClientConnectionListener.cc
  Connection.cc
  FunctionCallManager.cc
  GamestateManager.cc
  GamestateClient.cc
  GamestateHandler.cc
  NetworkFunction.cc
  Host.cc
  Server.cc
  ServerConnection.cc
  TrafficControl.cc
)

SET_SOURCE_FILES(NETWORK_HDR_FILES
  ChatListener.h
  Client.h
  ClientConnection.h
  ClientConnectionListener.h
  ClientInformation.h
  Connection.h
  FunctionCallManager.h
  GamestateClient.h
  GamestateHandler.h
  GamestateManager.h
  Host.h
  NetworkFunction.h
  NetworkPrecompiledHeaders.h
  NetworkPrereqs.h
  Server.h
  ServerConnection.h
  TrafficControl.h
)

ADD_SUBDIRECTORY(packet)
ADD_SUBDIRECTORY(synchronisable)

ORXONOX_ADD_LIBRARY(network
  DEFINE_SYMBOL
    "NETWORK_SHARED_BUILD"
  PCH_FILE
    NetworkPrecompiledHeaders.h
  LINK_LIBRARIES
    ${ZLIB_LIBRARY}
    ${ENET_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    util
    core
  SOURCE_FILES
    ${NETWORK_SRC_FILES} ${NETWORK_HDR_FILES}
)
