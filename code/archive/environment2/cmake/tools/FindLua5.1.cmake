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
 #
 #  Author:
 #    Reto Grieder
 #  Description:
 #    Finds Lua 5.1 on the system.
 #    Handles REQUIRED and QUIET arguments.
 #    When the search was successful, the following variables are set:
 #    LUA5.1_INCLUDE_DIR, LUA5.1_LIBRARY, LUA5.1_VERSION, LUA5.1_FOUND
 #

INCLUDE(CompareVersionStrings)
INCLUDE(FindPackageHandleAdvancedArgs)
INCLUDE(HandleLibraryTypes)

# Macro that determines Lua version. Should work for versions 2.2 and above (current release: 5.1.4)
FUNCTION(DETERMINE_LUA_VERSION _file _varname)
  IF(EXISTS ${_file})
    FILE(STRINGS ${_file} _file_content REGEX "LUA_VERSION|LUA_RELEASE")
  ELSE()
    SET(${_varname} "0" PARENT_SCOPE)
    RETURN()
  ENDIF()
  STRING(REGEX REPLACE "^.*LUA_RELEASE[ \t]+\"Lua[ \t]+([.0-9]+)\".*$" "\\1" ${_varname} "${_file_content}")
  IF(${_varname} STREQUAL "${_file_content}")
    # At most version 5.1.0
    STRING(REGEX REPLACE "^.*LUA_VERSION[ \t]+\"Lua[ \t]+([.0-9]+)\".*$" "\\1" ${_varname} "${_file_content}")
    IF(${_varname} STREQUAL "${_file_content}")
      MESSAGE(FATAL_ERROR "Could not determine Lua version which means this script has a bug")
    ENDIF()
    IF(${_varname} MATCHES "^[0-9]+\\.[0-9]+$")
      SET(${_varname} "${${_varname}}.0") # E.g. "3.2" is "3.2.0" actually
    ENDIF()
  ENDIF()
  SET(${_varname} "${${_varname}}" PARENT_SCOPE)
ENDFUNCTION(DETERMINE_LUA_VERSION)


# Find include path
FIND_PATH(LUA5.1_INCLUDE_DIR lua.h
  PATHS $ENV{LUA5.1_DIR} $ENV{LUA_DIR}
  PATH_SUFFIXES include/lua51 include/lua5.1 include/lua include
)

# Check whether we really found Lua version 5.1
IF(LUA5.1_INCLUDE_DIR)
  DETERMINE_LUA_VERSION(${LUA5.1_INCLUDE_DIR}/lua.h LUA5.1_VERSION)
  COMPARE_VERSION_STRINGS("${LUA5.1_VERSION}" "5.1" _version_compare TRUE)
  IF(NOT _version_compare EQUAL 0)
    # Wrong version
    MESSAGE(SEND_ERROR "Version of Lua header found is not 5.1")
    SET(LUA5.1_INCLUDE_DIR "LUA5.1_INCLUDE_DIR-NOTFOUND" CACHE PATH "" FORCE)
  ENDIF(NOT _version_compare EQUAL 0)
ENDIF(LUA5.1_INCLUDE_DIR)

# Find library
FIND_LIBRARY(LUA5.1_LIBRARY_OPTIMIZED
  NAMES lua51 lua5.1 lua
  PATHS $ENV{LUA5.1_DIR} $ENV{LUA_DIR}
  PATH_SUFFIXES lib64 lib
)
FIND_LIBRARY(LUA5.1_LIBRARY_DEBUG
  NAMES lua51d lua51_d lua5.1d lua5.1_d
        lua51D lua51_D lua5.1D lua5.1_D
        luad lua_d luad lua_D
  PATHS $ENV{LUA5.1_DIR} $ENV{LUA_DIR}
  PATH_SUFFIXES lib64 lib
)

# Handle the REQUIRED argument and set LUA5.1_FOUND
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Lua5.1 DEFAULT_MSG
  LUA5.1_LIBRARY_OPTIMIZED
  LUA5.1_INCLUDE_DIR
)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(LUA5.1)

MARK_AS_ADVANCED(
  LUA5.1_INCLUDE_DIR
  LUA5.1_LIBRARY_OPTIMIZED
  LUA5.1_LIBRARY_DEBUG
)
