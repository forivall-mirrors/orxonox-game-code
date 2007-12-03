PROJECT(Orxonox)

SET( NETWORK_SRC_FILES
  Client.cc
  ClientConnection.cc
  ConnectionManager.cc
  GameStateManager.cc
  PacketBuffer.cc
  PacketDecoder.cc
  PacketGenerator.cc
  Server.cc
  Synchronisable.cc
)

ADD_LIBRARY(network SHARED ${NETWORK_SRC_FILES})
TARGET_LINK_LIBRARIES(network ${ENet_LIBRARY} ${Boost_thread_LIBRARIES})
