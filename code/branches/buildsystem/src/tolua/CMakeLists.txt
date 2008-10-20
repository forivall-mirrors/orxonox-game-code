# TODO: only build the object files once

SET (TOLUALIB_SRC_FILES
  tolua_event.c
  tolua_is.c
  tolua_map.c
  tolua_push.c
  tolua_to.c
)

ADD_LIBRARY (tolualib_orxonox SHARED ${TOLUALIB_SRC_FILES})
TARGET_LINK_LIBRARIES (tolualib_orxonox ${LUA_LIBRARIES})

SET (TOLUAGEN_SRC_FILES
  tolua.c
  toluabind.c
)

ADD_EXECUTABLE (toluagen_orxonox ${TOLUAGEN_SRC_FILES} ${TOLUALIB_SRC_FILES})
TARGET_LINK_LIBRARIES (toluagen_orxonox ${LUA_LIBRARIES})

SET (TOLUA_PACKAGE "${CMAKE_CURRENT_SOURCE_DIR}/tolua-5.1.pkg")
GET_TARGET_PROPERTY(TOLUAGEN_EXE toluagen_orxonox LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/toluabind_orxonox.c
  COMMAND ${TOLUAGEN_EXE} -n tolua -o ${CMAKE_CURRENT_BINARY_DIR}/toluabind_orxonox.c -H ${CMAKE_CURRENT_BINARY_DIR}/toluabind_orxonox.h ${TOLUA_PACKAGE}
  DEPENDS
    toluagen_orxonox
    tolua-5.1.pkg
    lua/compat-5.1.lua
    lua/compat.lua
    lua/basic.lua
    lua/feature.lua
    lua/verbatim.lua
    lua/code.lua
    lua/typedef.lua
    lua/container.lua
    lua/package.lua
    lua/module.lua
    lua/namespace.lua
    lua/define.lua
    lua/enumerate.lua
    lua/declaration.lua
    lua/variable.lua
    lua/array.lua
    lua/function.lua
    lua/operator.lua
    lua/class.lua
    lua/clean.lua
    lua/doit.lua
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (TOLUAEXE_SRC_FILES
  tolua.c
  toluabind_orxonox.c
)

ADD_EXECUTABLE (tolua_orxonox ${TOLUAEXE_SRC_FILES} ${TOLUALIB_SRC_FILES})
TARGET_LINK_LIBRARIES (tolua_orxonox ${LUA_LIBRARIES})
