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
  ConnectionManager.cc
  GamestateManager.cc
  GamestateClient.cc
  GamestateHandler.cc
  PacketBuffer.cc
  Server.cc
  TrafficControl.cc
  Host.cc
)
ADD_SUBDIRECTORY(packet)
GET_ALL_HEADER_FILES(NETWORK_HDR_FILES)
SET(NETWORK_FILES ${NETWORK_SRC_FILES} ${NETWORK_HDR_FILES})

GENERATE_SOURCE_GROUPS(${NETWORK_FILES})
ADD_SOURCE_DIRECTORY(NETWORK_SRC_FILES synchronisable)

ADD_LIBRARY(network SHARED ${NETWORK_FILES})
SET_TARGET_PROPERTIES(network PROPERTIES DEFINE_SYMBOL "NETWORK_SHARED_BUILD")
TARGET_LINK_LIBRARIES(network
  ${ZLIB_LIBRARY}
  ${ENET_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  util
  core
)

ORXONOX_INSTALL(network)

# build those parts only on request.
IF(NETWORK_TESTING_ENABLED)
  ##### test for gamestate stuff #####
  SET(TEST_SRC_FILES
    ${NETWORK_SRC_FILES}
  )
  ADD_EXECUTABLE(networktest ${TEST_SRC_FILES})
  TARGET_LINK_LIBRARIES(networktest
    ${OGRE_LIBRARY}
    network
    ${ENET_LIBRARY}
    ${ZLIB_LIBRARY}
  )
  ##### end test for gamestate stuff #####
ENDIF(NETWORK_TESTING_ENABLED)
