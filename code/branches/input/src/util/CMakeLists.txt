SET(UTIL_SRC_FILES
  ArgReader.cc
  Clipboard.cc
  ExprParser.cc
  Math.cc
  MultiTypePrimitive.cc
  MultiTypeString.cc
  MultiTypeMath.cc
  String.cc
  SubString.cc

  tinyxml/ticpp.cc
  tinyxml/tinystr.cc
  tinyxml/tinyxml.cc
  tinyxml/tinyxmlerror.cc
  tinyxml/tinyxmlparser.cc

  tolua/tolua_event.c
  tolua/tolua_is.c
  tolua/tolua_map.c
  tolua/tolua_push.c
  tolua/tolua_to.c
)

ADD_LIBRARY(util SHARED ${UTIL_SRC_FILES})

IF(TESTING_ENABLED)
  ADD_SUBDIRECTORY(testing)
ENDIF(TESTING_ENABLED)

TARGET_LINK_LIBRARIES(util
  ${OGRE_LIBRARIES}
  ${Lua_LIBRARIES}
) 

ADD_SUBDIRECTORY(tolua)

