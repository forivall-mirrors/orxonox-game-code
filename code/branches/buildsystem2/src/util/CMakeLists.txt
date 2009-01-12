SET(UTIL_SRC_FILES
  Clipboard.cc
  CRC32.cc
  Exception.cc
  ExprParser.cc
  Math.cc
  MultiType.cc
  OutputBuffer.cc
  OutputHandler.cc
  SignalHandler.cc
  String.cc
  SubString.cc
)

ADD_LIBRARY(util SHARED ${UTIL_SRC_FILES})
SET_TARGET_PROPERTIES(util PROPERTIES DEFINE_SYMBOL "UTIL_SHARED_BUILD")
TARGET_LINK_LIBRARIES(util ${OGRE_LIBRARY})

IF (NOT WIN32)
  INSTALL(TARGETS util LIBRARY DESTINATION lib)
ENDIF (NOT WIN32)
