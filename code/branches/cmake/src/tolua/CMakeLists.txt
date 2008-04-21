SET(TOLUA_SRC_FILES
  tolua.c
  tolua_event.c
  tolua_is.c
  tolua_map.c
  tolua_push.c
  tolua_to.c
  toluabind.c
)


ADD_EXECUTABLE(tolua ${TOLUA_SRC_FILES})

TARGET_LINK_LIBRARIES(tolua
  ${Lua_LIBRARIES}
  ${Lua_LIBRARY}
  m
)



SET(TOLUALIB_SRC_FILES
  tolua_event.c
  tolua_is.c
  tolua_map.c
  tolua_push.c
  tolua_to.c
  tolua_bind.cc
  tolua_bind.h
)

SET_SOURCE_FILES_PROPERTIES(tolua_bind.h
  PROPERTIES
  OBJECT_DEPENDS tolua_bind.h
  OBJECT_DEPENDS tolua_bind.cc
  GENERATED true
  HEADER_FILE_ONLY true
)

GET_TARGET_PROPERTY(TOLUA_EXE tolua LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT tolua_bind.h tolua_bind.cc
  COMMAND ${TOLUA_EXE} -n orxonox -o tolua/tolua_bind.cc -H tolua/tolua_bind.h tolua/tolua.pkg
  DEPENDS tolua
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
)

ADD_LIBRARY(tolualib ${TOLUALIB_SRC_FILES})

TARGET_LINK_LIBRARIES(tolualib
  ${Lua_LIBRARIES}
  ${Lua_LIBRARY}
)

