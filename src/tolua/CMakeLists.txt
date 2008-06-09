SET (TOLUALIB_SRC_FILES
  tolua_event.c
  tolua_is.c
  tolua_map.c
  tolua_push.c
  tolua_to.c
)

ADD_LIBRARY (tolualib SHARED ${TOLUALIB_SRC_FILES})

TARGET_LINK_LIBRARIES (tolualib
  ${Lua_LIBRARIES}
)

SET (TOLUAEXE_SRC_FILES
  tolua.c
  toluabind.c
)

ADD_EXECUTABLE (tolua ${TOLUAEXE_SRC_FILES})

TARGET_LINK_LIBRARIES (tolua
  ${Lua_LIBRARIES}
  tolualib
  m
)
