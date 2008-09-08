SET(UTIL_SRC_FILES
  ArgReader.cc
  Clipboard.cc
  CRC32.cc
  ExprParser.cc
  Math.cc
  MultiTypePrimitive.cc
  MultiTypeString.cc
  MultiTypeMath.cc
  String.cc
  SubString.cc
)

ADD_LIBRARY(util SHARED ${UTIL_SRC_FILES})

IF(TESTING_ENABLED)
  ADD_SUBDIRECTORY(testing)
ENDIF(TESTING_ENABLED)

TARGET_LINK_LIBRARIES(util
  ${OGRE_LIBRARIES}
  ${Lua_LIBRARIES}
)
