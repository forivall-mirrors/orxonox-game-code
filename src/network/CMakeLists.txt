SET( NETWORK_SRC_FILES
  Client.cc
  ClientConnection.cc
  ClientInformation.cc
  ConnectionManager.cc
  GameStateManager.cc
  GameStateClient.cc
  PacketBuffer.cc
  PacketDecoder.cc
  PacketGenerator.cc
  Server.cc
  Synchronisable.cc
  dummyserver3.cc
)

IF(WIN32)
  ADD_LIBRARY( network ${NETWORK_SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY( network SHARED ${NETWORK_SRC_FILES})
ENDIF(WIN32)

SET_TARGET_PROPERTIES( network PROPERTIES LINK_FLAGS "--no-undefined" )

TARGET_LINK_LIBRARIES( network
  ${ZLIB_LIBRARY}
  ${ENet_LIBRARY}
  ${Boost_thread_LIBRARIES}
  core
)

# build those parts only on request.
IF(NETWORK_TESTING_ENABLED)
  SET( CHATCLIENT_SRC_FILES
    ClientConnection.cc
    Client.cc
    PacketBuffer.cc
    PacketDecoder.cc
    PacketGenerator.cc
    dummyclient3.cc
  )

  SET( CHATSERVER_SRC_FILES
    ConnectionManager.cc
    GameStateManager.cc
    PacketBuffer.cc
    PacketDecoder.cc
    PacketGenerator.cc
    dummyserver3.cc
  )

  # chatserver for testing purposes
  ADD_EXECUTABLE(chatserver ${CHATSERVER_SRC_FILES})
  TARGET_LINK_LIBRARIES( chatserver
    ${OGRE_LIBRARIES}
    network
    core
    #objects
    ${ENet_LIBRARY}
    ${ZLIB_LIBRARY}
    ${WINDOWS_ENET_DEPENDENCIES}
  )

  # chatclient for testing purpose
  ADD_EXECUTABLE(chatclient ${CHATCLIENT_SRC_FILES})
  TARGET_LINK_LIBRARIES( chatclient
    ${OGRE_LIBRARIES}
    network
    core
    #objects
    ${ENet_LIBRARY}
    ${ZLIB_LIBRARY}
    ${WINDOWS_ENET_DEPENDENCIES}
  )

  # client for testing purpose
  ADD_EXECUTABLE(client dummyclient2.cc)
  TARGET_LINK_LIBRARIES( client
    ${OGRE_LIBRARIES}
    network
    core
    #objects
    ${ENet_LIBRARY}
    ${ZLIB_LIBRARY}
    ${WINDOWS_ENET_DEPENDENCIES}
  )

  # server for testing purpose
  ADD_EXECUTABLE(server dummyserver.cc)
  TARGET_LINK_LIBRARIES( server
    ${OGRE_LIBRARIES}
    network
    core
    #objects
    ${ENet_LIBRARY}
    ${ZLIB_LIBRARY}
    ${WINDOWS_ENET_DEPENDENCIES}
  )
ENDIF(NETWORK_TESTING_ENABLED)
