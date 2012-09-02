ADD_SOURCE_FILES(NETWORK_SRC_FILES
COMPILATION_BEGIN PacketCompilation.cc
  Acknowledgement.cc
  Chat.cc
  ClassID.cc
  DeleteObjects.cc
  FunctionIDs.cc
  FunctionCalls.cc
  Gamestate.cc
  Packet.cc
  Welcome.cc
COMPILATION_END
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
