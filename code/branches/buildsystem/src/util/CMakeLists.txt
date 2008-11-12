SET(UTIL_SRC_FILES
  Clipboard.cc
  CRC32.cc
  Exception.cc
  ExprParser.cc
  Math.cc
  MultiType.cc
  OutputBuffer.cc
  OutputHandler.cc
  String.cc
  SubString.cc
)

ADD_LIBRARY(util SHARED ${UTIL_SRC_FILES})
TARGET_LINK_LIBRARIES(util ${OGRE_LIBRARY})

ADD_SUBDIRECTORY(testing)
