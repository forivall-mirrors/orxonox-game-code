ADD_SOURCE_FILES(
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

ADD_SOURCE_DIRECTORY(packet)

WRITE_SOURCE_FILES(NETWORK_SRC_FILES)
IF(WIN32)
  ADD_LIBRARY( network ${NETWORK_SRC_FILES} )
ELSE(WIN32)
  ADD_LIBRARY( network SHARED ${NETWORK_SRC_FILES} )
ENDIF(WIN32)

TARGET_LINK_LIBRARIES( network
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
