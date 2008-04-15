SET (TOLUA_SRC_FILES
  tolua.c
  tolua_event.c
  tolua_is.c
  tolua_map.c
  tolua_push.c
  tolua_to.c
  toluabind.c
)

ADD_EXECUTABLE( tolua ${TOLUA_SRC_FILES} )

TARGET_LINK_LIBRARIES( tolua
  ${Lua_LIBRARIES}
)
