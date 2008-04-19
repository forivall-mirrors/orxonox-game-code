AUX_SOURCE_DIRECTORY(tinyxml TINYXML_SRC_FILES)

SET (UTIL_SRC_FILES
  ArgReader.cc
  Math.cc
  String.cc
  Clipboard.cc
  SubString.cc
  FloatParser.cc
  MultiTypePrimitive.cc
  MultiTypeString.cc
  MultiTypeMath.cc
  tolua/tolua_event.c
  tolua/tolua_is.c
  tolua/tolua_map.c
  tolua/tolua_push.c
  tolua/tolua_to.c
  ${TINYXML_SRC_FILES}
)

ADD_LIBRARY( util SHARED ${UTIL_SRC_FILES} )

IF(TESTING_ENABLED)
  ADD_SUBDIRECTORY(testing)
ENDIF(TESTING_ENABLED)

TARGET_LINK_LIBRARIES( util
  ${OGRE_LIBRARIES}
  ${Lua_LIBRARIES}
  ${Lua_LIBRARY}
) 

