PROJECT(Orxonox)

SET(SRC_FILES Client.cc ConnectionManager.cc PacketBuffer.cc PacketGenerator.cc Synchronisable.cc ClientConnection.cc GameStateManager.cc PacketDecoder.cc Server.cc)

INCLUDE_DIRECTORIES(..)

ADD_LIBRARY(network ${SRC_FILES} ${ENet_LIBRARY})
