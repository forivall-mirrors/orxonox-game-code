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


SET (TOLUAGEN_SRC_FILES
  tolua.c
  toluabind.c
)

ADD_EXECUTABLE (toluagen ${TOLUAGEN_SRC_FILES})

TARGET_LINK_LIBRARIES (toluagen
  ${Lua_LIBRARIES}
  tolualib
  m
)


IF (Lua_VERSION EQUAL 5.0)
  SET (TOLUA_PACKAGE "../../src/tolua/tolua-5.0.pkg")
ELSE (Lua_VERSION EQUAL 5.0)
  SET (TOLUA_PACKAGE "../../src/tolua/tolua-5.1.pkg")
ENDIF (Lua_VERSION EQUAL 5.0)

GET_TARGET_PROPERTY(TOLUAGEN_EXE toluagen LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/toluabind_orxonox.c
  COMMAND ${TOLUAGEN_EXE} -n tolua -o ../../src/tolua/toluabind_orxonox.c -H ../../src/tolua/toluabind_orxonox.h ${TOLUA_PACKAGE}
  DEPENDS toluagen
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib
)

SET (TOLUAEXE_SRC_FILES
  tolua.c
  toluabind_orxonox.c
)

ADD_EXECUTABLE (tolua ${TOLUAEXE_SRC_FILES})

TARGET_LINK_LIBRARIES (tolua
  ${Lua_LIBRARIES}
  tolualib
  m
)
