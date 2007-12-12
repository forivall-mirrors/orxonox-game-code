PROJECT(Orxonox)

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

ADD_LIBRARY(network SHARED ${NETWORK_SRC_FILES})
TARGET_LINK_LIBRARIES(network ${ENet_LIBRARY} ${Boost_thread_LIBRARIES})

ADD_EXECUTABLE(chatserver ${CHATSERVER_SRC_FILES})
TARGET_LINK_LIBRARIES( chatserver
  ${OGRE_LIBRARIES}
  network
  core
  objects
)

ADD_EXECUTABLE(chatclient ${CHATCLIENT_SRC_FILES})
TARGET_LINK_LIBRARIES( chatclient
  ${OGRE_LIBRARIES}
  network
  core
  objects
)

ADD_EXECUTABLE(client dummyclient2.cc)
ADD_EXECUTABLE(server dummyserver.cc)
TARGET_LINK_LIBRARIES( client
  ${OGRE_LIBRARIES}
  network
  core
  objects
)
TARGET_LINK_LIBRARIES( server
  ${OGRE_LIBRARIES}
  network
  core
  objects
)

