SET(CEGUILUA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ceguilua-${CEGUI_VERSION}/ceguilua)
SET(CEGUILUA_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

SET(CEGUILUA_FILES
  ${CEGUILUA_DIR}/CEGUILua.cpp
  ${CEGUILUA_DIR}/CEGUILuaFunctor.cpp
  ${CEGUILUA_DIR}/required.cpp
  ${CEGUILUA_BINARY_DIR}/lua_CEGUI.cpp

  ${CEGUILUA_DIR}/CEGUILua.h
  ${CEGUILUA_DIR}/CEGUILuaFunctor.h
  ${CEGUILUA_DIR}/required.h
)

INCLUDE(CompareVersionStrings)

# Copy package files incrementally until the version is met
SET(CEGUILUA_VERSIONS 0.5.0 0.6.0 0.6.1 0.6.2)
FOREACH(_version ${CEGUILUA_VERSIONS})
  COMPARE_VERSION_STRINGS(${_version} ${CEGUI_VERSION} _compare_result)
  IF(_compare_result EQUAL 1)
    BREAK() # _version > CEGUI_VERSION
  ENDIF(_compare_result EQUAL 1)
  ADD_SUBDIRECTORY(ceguilua-${_version})
ENDFOREACH(_version)

# Create the tolua bind file. We could use the orignal file though, but it is 1.6MB...
ADD_CUSTOM_COMMAND(
  OUTPUT ${CEGUILUA_BINARY_DIR}/lua_CEGUI.cpp
  COMMAND toluaexe_orxonox -n CEGUI
                             -w ${CEGUILUA_BINARY_DIR}
                             -o lua_CEGUI.cpp
                             -L exceptions.lua
                             -s ${TOLUA_PARSER_SOURCE}
                                CEGUI.pkg
  DEPENDS              ${TOLUA_PARSER_DEPENDENCIES}
  WORKING_DIRECTORY    ${ORXONOX_LIBRARY_BIN_DIR}
  COMMENT "Generating tolua bind files for package CEGUILua"
)

ADD_CXX_FLAGS("-w44996" MSVC)

ADD_LIBRARY(ceguilua_orxonox SHARED ${CEGUILUA_FILES})
SET_TARGET_PROPERTIES(ceguilua_orxonox PROPERTIES DEFINE_SYMBOL "CEGUILUA_EXPORTS")
TARGET_LINK_LIBRARIES(ceguilua_orxonox
  tolualib_orxonox
  ${LUA_LIBRARIES}
  ${CEGUI_LIBRARY}
)

IF (NOT WIN32)
  INSTALL(TARGETS ceguilua_orxonox LIBRARY DESTINATION lib)
ENDIF (NOT WIN32)
