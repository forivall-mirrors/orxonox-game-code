PROJECT(Orxonox)

SET(SRC_FILES Client.cc ConnectionManager.cc PacketBuffer.cc PacketGenerator.cc Synchronisable.cc dummyclient2.cc ClientConnection.cc GameStateManager.cc PacketDecoder.cc Server.cc dummyclient.cc dummyserver.cc)

ADD_LIBRARY(network ${SRC_FILES})
