################### Tolua++ library ###################

SET(TOLUA++_FILES
  tolua_event.h
  tolua++.h

  tolua_event.c
  tolua_is.c
  tolua_map.c
  tolua_push.c
  tolua_to.c
)
GENERATE_SOURCE_GROUPS(${TOLUA++_FILES})

ADD_COMPILER_FLAGS("-w44996" MSVC)

ADD_LIBRARY(tolua++_orxonox SHARED ${TOLUA++_FILES})
SET_TARGET_PROPERTIES(tolua++_orxonox PROPERTIES DEFINE_SYMBOL "TOLUA_SHARED_BUILD")
TARGET_LINK_LIBRARIES(tolua++_orxonox ${LUA_LIBRARIES})

IF(NOT WIN32)
#  INSTALL(TARGETS tolualib_orxonox LIBRARY DESTINATION lib)
ENDIF()


################## Tolua++ generator ##################

ADD_EXECUTABLE(tolua++app_orxonox tolua.c)
TARGET_LINK_LIBRARIES(tolua++app_orxonox tolua++_orxonox ${LUA_LIBRARIES})

# Set some variables to the cache in order to use them in the TOLUA macro
SET(TOLUA_PARSER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/all-${LUA_VERSION}.lua" CACHE INTERNAL "")
SET(TOLUA_PARSER_DEPENDENCIES
  tolua++app_orxonox
  ${CMAKE_CURRENT_SOURCE_DIR}/all-${LUA_VERSION}.lua
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

  CACHE INTERNAL ""
)
