 #
 #             ORXONOX - the hottest 3D action shooter ever to exist
 #                             > www.orxonox.net <
 #
 #        This program is free software; you can redistribute it and/or
 #         modify it under the terms of the GNU General Public License
 #        as published by the Free Software Foundation; either version 2
 #            of the License, or (at your option) any later version.
 #
 #       This program is distributed in the hope that it will be useful,
 #        but WITHOUT ANY WARRANTY; without even the implied warranty of
 #        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #                 GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License along
 #      with this program; if not, write to the Free Software Foundation,
 #     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #

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
  ENDIF()
  
  ADD_SUBDIRECTORY(ceguilua-${_version}) # Sets parent scope variable
  FOREACH(_file ${_package_files})
    CONFIGURE_FILE(ceguilua-${_version}/package/${_file} ${CEGUILUA_BINARY_DIR}/${_file} COPYONLY)
  ENDFOREACH(_file)
ENDFOREACH(_version)

# Create the tolua bind file. We could use the orignal file though, but it is 1.6MB...
ADD_CUSTOM_COMMAND(
  OUTPUT ${CEGUILUA_BINARY_DIR}/lua_CEGUI.cpp
  COMMAND tolua++app_orxonox -n CEGUI
                             -w ${CEGUILUA_BINARY_DIR}
                             -o lua_CEGUI.cpp
                             -L exceptions.lua
                             -s ${TOLUA_PARSER_SOURCE}
                                CEGUI.pkg
  DEPENDS              ${TOLUA_PARSER_DEPENDENCIES}
  WORKING_DIRECTORY    ${ORXONOX_RUNTIME_LIBRARY_DIRECTORY}
  COMMENT "Generating tolua bind files for package CEGUILua"
)

ADD_COMPILER_FLAGS("-w44996" MSVC)

SOURCE_GROUP("Source" FILES ${CEGUILUA_FILES})

SET(CEGUILUA_LIBRARY ceguilua_orxonox)
SET(CEGUILUA_LIBRARY ${CEGUILUA_LIBRARY} PARENT_SCOPE)
ADD_LIBRARY(${CEGUILUA_LIBRARY} SHARED ${CEGUILUA_FILES})
SET_TARGET_PROPERTIES(${CEGUILUA_LIBRARY} PROPERTIES DEFINE_SYMBOL "CEGUILUA_EXPORTS")
TARGET_LINK_LIBRARIES(${CEGUILUA_LIBRARY}
  tolua++_orxonox
  ${LUA_LIBRARIES}
  ${CEGUI_LIBRARY}
)

IF(NOT WIN32)
  INSTALL(TARGETS ceguilua_orxonox LIBRARY DESTINATION lib)
ENDIF()
