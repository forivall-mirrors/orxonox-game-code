SET( NETWORK_SRC_FILES
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

ADD_SOURCE_DIRECTORY(NETWORK_SRC_FILES packet)
ADD_SOURCE_DIRECTORY(NETWORK_SRC_FILES synchronisable)


IF(WIN32)
  ADD_LIBRARY( network ${NETWORK_SRC_FILES} )
ELSE(WIN32)
  ADD_LIBRARY( network SHARED ${NETWORK_SRC_FILES} )
ENDIF(WIN32)

TARGET_LINK_LIBRARIES( network
  ${ZLIB_LIBRARY}
  ${ENet_LIBRARY}
  ${Boost_thread_LIBRARIES}
  core
)


# build those parts only on request.
IF(NETWORK_TESTING_ENABLED)
  ##### test for gamestate stuff #####
  SET( TEST_SRC_FILES
    ${NETWORK_SRC_FILES}
  )
  ADD_EXECUTABLE(networktest ${TEST_SRC_FILES})
  TARGET_LINK_LIBRARIES( networktest
    ${OGRE_LIBRARIES}
    network
    ${ENet_LIBRARY}
    ${ZLIB_LIBRARY}
    ${WINDOWS_ENET_DEPENDENCIES}
  )
  ##### end test for gamestate stuff #####
ENDIF(NETWORK_TESTING_ENABLED)
