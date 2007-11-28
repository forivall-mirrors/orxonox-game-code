PROJECT(Orxonox)

SET(SRC_FILES Client.cc ConnectionManager.cc PacketBuffer.cc PacketGenerator.cc Synchronisable.cc dummyclient2.cc ClientConnection.cc GameStateManager.cc PacketDecoder.cc Server.cc dummyclient.cc dummyserver.cc)
SET(INC_FILES Client.h ClientConnection.h ConnectionManager.h GameStateManager.h PacketBuffer.h PacketManager.h Server.h Synchronisable.h ${ENet_INCLUDE_DIR})

ADD_LIBRARY(network ${SRC_FILES} ${INC_FILES})
