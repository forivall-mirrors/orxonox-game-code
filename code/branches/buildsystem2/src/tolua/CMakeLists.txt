ADD_C_FLAGS("-wd4996" MSVC)

ADD_LIBRARY(tolualib_orxonox SHARED
  tolua_event.c
  tolua_is.c
  tolua_map.c
  tolua_push.c
  tolua_to.c
)
SET_TARGET_PROPERTIES(tolualib_orxonox PROPERTIES DEFINE_SYMBOL "TOLUA_SHARED_BUILD")
TARGET_LINK_LIBRARIES(tolualib_orxonox ${LUA_LIBRARY})

IF (NOT WIN32)
#  INSTALL(TARGETS tolualib_orxonox LIBRARY DESTINATION lib)
ENDIF (NOT WIN32)

SET(TOLUAGEN_SRC_FILES
  tolua.c
)

ADD_EXECUTABLE(toluaexe_orxonox ${TOLUAGEN_SRC_FILES})
TARGET_LINK_LIBRARIES(toluaexe_orxonox tolualib_orxonox ${LUA_LIBRARIES})

#IF("${LUA_VERSION}" VERSION_LESS 5.1)
IF("${LUA_VERSION}" MATCHES "^5.0")
  SET(TOLUA_ALL_PACK all-5.0.lua)
#ELSE("${LUA_VERSION}" VERSION_LESS 5.1)
ELSE("${LUA_VERSION}" MATCHES "^5.0")
  SET(TOLUA_ALL_PACK all-5.1.lua)
#ENDIF("${LUA_VERSION}" VERSION_LESS 5.1)
ENDIF("${LUA_VERSION}" MATCHES "^5.0")

# Set some variables to the cache in order to use them in the TOLUA macro
SET(TOLUA_PARSER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${TOLUA_ALL_PACK}" CACHE STRING "")
SET(TOLUA_PARSER_DEPENDENCIES
  toluaexe_orxonox
  ${CMAKE_CURRENT_SOURCE_DIR}/${TOLUA_ALL_PACK}
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/compat-5.1.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/compat.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/basic.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/feature.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/verbatim.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/code.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/typedef.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/container.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/package.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/module.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/namespace.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/define.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/enumerate.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/declaration.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/variable.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/array.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/function.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/operator.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/class.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/clean.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/doit.lua

  CACHE STRING ""
)
MARK_AS_ADVANCED(TOLUA_PARSER_SOURCE TOLUA_PARSER_DEPENDENCIES)
