ADD_SOURCE_FILES(NETWORK_SRC_FILES

BUILD_UNIT PacketBuildUnit.cc
  Acknowledgement.cc
  Chat.cc
  ClassID.cc
  DeleteObjects.cc
  FunctionIDs.cc
  FunctionCalls.cc
  Gamestate.cc
  Packet.cc
  Welcome.cc
END_BUILD_UNIT

  ServerInformation.cc
)

ADD_SOURCE_FILES(NETWORK_HDR_FILES
  Acknowledgement.h
  Chat.h
  ClassID.h
  DeleteObjects.h
  FunctionCalls.h
  FunctionIDs.h
  Gamestate.h
  Packet.h
  ServerInformation.h
  Welcome.h
)
