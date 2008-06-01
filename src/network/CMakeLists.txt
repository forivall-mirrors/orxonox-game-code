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
)

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
    ConnectionManager.cc
    GameStateManager.cc
    PacketBuffer.cc
    PacketDecoder.cc
    PacketGenerator.cc
    ClientConnection.cc
    ClientInformation.cc
    diffTest.cc
    GameStateClient.cc
    Server.cc
    Client.cc  
    Synchronisable.cc  
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
# build this if you want dummyserver4 and dummyclient4
IF(NETWORKTRAFFIC_TESTING_ENABLED)
  ##### test network traffic #####
  SET( DUMMYSERVER4_SRC_FILES
    ConnectionManager.cc
    GameStateManager.cc
    PacketBuffer.cc
    PacketDecoder.cc
    PacketGenerator.cc
    ClientConnection.cc
    ClientInformation.cc
    GameStateClient.cc
    Server.cc
    Client.cc  
    Synchronisable.cc
    dummyserver4.cc
  )

  SET( DUMMYCLIENT4_SRC_FILES
    ConnectionManager.cc
    GameStateManager.cc
    PacketBuffer.cc
    PacketDecoder.cc
    PacketGenerator.cc
    ClientConnection.cc
    ClientInformation.cc
    GameStateClient.cc
    Server.cc
    Client.cc  
    Synchronisable.cc
    dummyclient4.cc
  )

  ADD_EXECUTABLE(dummyserver4 ${DUMMYSERVER4_SRC_FILES})
  TARGET_LINK_LIBRARIES( dummyserver4
    network
    ${ENet_LIBRARY}
    ${ZLIB_LIBRARY}
    ${WINDOWS_ENET_DEPENDENCIES}
    ${Boost_thread_LIBRARIES}
    core
    orxonoxs
    audio
  )

  ADD_EXECUTABLE(dummyclient4 ${DUMMYCLIENT4_SRC_FILES})
  TARGET_LINK_LIBRARIES( dummyclient4
    network
    ${ENet_LIBRARY}
    ${ZLIB_LIBRARY}
    ${WINDOWS_ENET_DEPENDENCIES}
    ${Boost_thread_LIBRARIES}
    core
    orxonoxs
    audio
  )
ENDIF(NETWORKTRAFFIC_TESTING_ENABLED)

IF(BLABLA)
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
ENDIF(BLABLA)

