SET (TOLUALIB_SRC_FILES
  tolua_event.c
  tolua_is.c
  tolua_map.c
  tolua_push.c
  tolua_to.c
)

ADD_LIBRARY (tolualib_orxonox SHARED ${TOLUALIB_SRC_FILES})

TARGET_LINK_LIBRARIES (tolualib_orxonox
  lua_orxonox
)


SET (TOLUAGEN_SRC_FILES
  tolua.c
  toluabind.c
)

ADD_EXECUTABLE (toluagen_orxonox ${TOLUAGEN_SRC_FILES})

TARGET_LINK_LIBRARIES (toluagen_orxonox
  lua_orxonox
  tolualib_orxonox
  m
)


IF (Lua_VERSION EQUAL 5.0)
  SET (TOLUA_PACKAGE "../../src/tolua/tolua-5.0.pkg")
ELSE (Lua_VERSION EQUAL 5.0)
  SET (TOLUA_PACKAGE "../../src/tolua/tolua-5.1.pkg")
ENDIF (Lua_VERSION EQUAL 5.0)

GET_TARGET_PROPERTY(TOLUAGEN_EXE toluagen_orxonox LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/toluabind_orxonox.c
  COMMAND ${TOLUAGEN_EXE} -n tolua -o ../../src/tolua/toluabind_orxonox.c -H ../../src/tolua/toluabind_orxonox.h ${TOLUA_PACKAGE}
  DEPENDS toluagen_orxonox
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib
)

SET (TOLUAEXE_SRC_FILES
  tolua.c
  toluabind_orxonox.c
)

ADD_EXECUTABLE (tolua_orxonox ${TOLUAEXE_SRC_FILES})

TARGET_LINK_LIBRARIES (tolua_orxonox
  lua_orxonox
  tolualib_orxonox
  m
)
