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

ADD_LIBRARY(network ${NETWORK_SRC_FILES} ${ENet_LABRARY})
TARGET_LINK_LIBRARIES(network ${ENet_LIBRARY})