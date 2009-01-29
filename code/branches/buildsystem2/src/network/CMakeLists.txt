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
  Synchronisable.cc
  Host.cc
)
ADD_SUBDIRECTORY(packet)
GET_ALL_HEADER_FILES(NETWORK_HDR_FILES)
SET(NETWORK_FILES ${NETWORK_SRC_FILES} ${NETWORK_HDR_FILES})

GENERATE_SOURCE_GROUPS(${NETWORK_FILES})

IF(MINGW)
  ADD_LIBRARY(network ${NETWORK_FILES})
ELSE(MINGW)
  ADD_LIBRARY(network SHARED ${NETWORK_FILES})
ENDIF(MINGW)

SET_TARGET_PROPERTIES(network PROPERTIES DEFINE_SYMBOL "NETWORK_SHARED_BUILD")
TARGET_LINK_LIBRARIES(network
  ${ZLIB_LIBRARY}
  ${ENET_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  util
  core
)

IF (NOT WIN32)
  INSTALL(TARGETS network LIBRARY DESTINATION lib)
ENDIF (NOT WIN32)

# build those parts only on request.
IF(NETWORK_TESTING_ENABLED)
  ##### test for gamestate stuff #####
  SET( TEST_SRC_FILES
    ${NETWORK_SRC_FILES}
  )
  ADD_EXECUTABLE(networktest ${TEST_SRC_FILES})
  TARGET_LINK_LIBRARIES( networktest
    ${OGRE_LIBRARY}
    network
    ${ENET_LIBRARY}
    ${ZLIB_LIBRARY}
  )
  ##### end test for gamestate stuff #####
ENDIF(NETWORK_TESTING_ENABLED)
