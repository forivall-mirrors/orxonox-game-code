SET( NETWORK_SRC_FILES
  Client.cc
  ClientConnection.cc
  ClientInformation.cc
  ConnectionManager.cc
  GamestateManager.cc
  GamestateClient.cc
  GamestateHandler.cc
  PacketBuffer.cc
  Server.cc
  Synchronisable.cc
  Host.cc
  packet/Packet.cc
  packet/Chat.cc
  packet/ClassID.cc
  packet/Acknowledgement.cc
  packet/Gamestate.cc
  packet/Welcome.cc
)

IF(WIN32)
  ADD_LIBRARY( network ${NETWORK_SRC_FILES} )
ELSE(WIN32)
  ADD_LIBRARY( network SHARED ${NETWORK_SRC_FILES} )
ENDIF(WIN32)

TARGET_LINK_LIBRARIES( network
  ${ZLIB_LIBRARY}
  ${ENet_LIBRARY}
  ${Boost_THREAD_LIBRARY}
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
