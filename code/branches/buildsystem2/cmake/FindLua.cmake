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


# Find Lua 5.1
FIND_PATH(LUA_5.1_INCLUDE_DIR lua.h
  PATHS $ENV{LUA_DIR}
  PATH_SUFFIXES include/lua51 include/lua5.1 include/lua include
)
IF(LUA_5.1_INCLUDE_DIR)
  DETERMINE_LUA_VERSION(${LUA_5.1_INCLUDE_DIR}/lua.h LUA_5.1_VERSION)
  COMPARE_VERSION_STRINGS("${LUA_5.1_VERSION}" "5.1" _version_compare TRUE)
  IF(NOT _version_compare EQUAL 0)
    # Incorrect version found, abort search
    SET(LUA_5.1_INCLUDE_DIR "LUA_5.1_INCLUDE_DIR-NOTFOUND" CACHE PATH "" FORCE)
  ELSE()
    FIND_LIBRARY(LUA_5.1_LIBRARY_OPTIMIZED
      NAMES lua51 lua5.1 lua
      PATHS $ENV{LUA_DIR}
      PATH_SUFFIXES lib64 lib
    )
    FIND_LIBRARY(LUA_5.1_LIBRARY_DEBUG
      NAMES lua51d lua51_d lua5.1d lua5.1_d luad lua_d
      PATHS $ENV{LUA_DIR}
      PATH_SUFFIXES lib64 lib
    )
    HANDLE_LIBRARY_TYPES(LUA_5.1)
    SET(LUA_5.1_LIBRARIES ${LUA_5.1_LIBRARY})
    IF(LUA_5.1_LIBRARIES)
      SET(LUA_5.1_FOUND TRUE)
    ENDIF()
  ENDIF(NOT _version_compare EQUAL 0)
ENDIF(LUA_5.1_INCLUDE_DIR)


# Find Lua 5.0
FIND_PATH(LUA_5.0_INCLUDE_DIR lua.h
  PATHS $ENV{LUA_DIR}
  PATH_SUFFIXES include/lua50 include/lua5.0 include/lua5 include/lua include
)
IF(LUA_5.0_INCLUDE_DIR)
  DETERMINE_LUA_VERSION(${LUA_5.0_INCLUDE_DIR}/lua.h LUA_5.0_VERSION)
  COMPARE_VERSION_STRINGS("${LUA_5.0_VERSION}" "5.0" _version_compare TRUE)
  IF(NOT _version_compare EQUAL 0)
    # Incorrect version found, abourt search
    SET(LUA_5.0_INCLUDE_DIR "LUA_5.0_INCLUDE_DIR-NOTFOUND" CACHE PATH "" FORCE)
  ELSE()
    FIND_LIBRARY(LUA_5.0_LUA_LIBRARY_OPTIMIZED
      NAMES lua50 lua5.0 lua5 lua
      PATHS $ENV{LUA_DIR}
      PATH_SUFFIXES lib64 lib
    )
    FIND_LIBRARY(LUA_5.0_LUA_LIBRARY_DEBUG
      NAMES lua50d lua50_d lua5.0d lua5.0_d lua5d lua5_d luad lua_d
      PATHS $ENV{LUA_DIR}
      PATH_SUFFIXES lib64 lib
    )
    HANDLE_LIBRARY_TYPES(LUA_5.0_LUA)

    # In an OS X framework, lualib is usually included as part of the framework
    # (like GLU in OpenGL.framework)
    IF(${LUA_5.0_LUA_LIBRARY} MATCHES "framework")
      SET(LUA_5.0_LIBRARIES ${LUA_5.0_LUA_LIBRARY})
    ELSE()
      FIND_LIBRARY(LUA_5.0_LUALIB_LIBRARY_OPTIMIZED
        NAMES lualib50 lualib5.0 lualib5 lualib
        PATHS $ENV{LUALIB_DIR} $ENV{LUA_DIR}
        PATH_SUFFIXES lib64 lib
      )
      FIND_LIBRARY(LUA_5.0_LUALIB_LIBRARY_DEBUG
        NAMES lualib50d lualib50_d lualib5.0d lualib5.0_d
	      lualib5d lualib5_d lualibd lualib_d
        PATHS $ENV{LUALIB_DIR} $ENV{LUA_DIR}
        PATH_SUFFIXES lib64 lib
      )
      HANDLE_LIBRARY_TYPES(LUA_5.0_LUALIB)
      # Both libraries are required for Lua 5.0 to be found
      IF(LUA_5.0_LUA_LIBRARY AND LUA_5.0_LUALIB_LIBRARY)
        SET(LUA_5.0_LIBRARIES ${LUA_5.0_LUA_LIBRARY} ${LUA_5.0_LUALIB_LIBRARY})
      ENDIF()
    ENDIF(${LUA_5.0_LUA_LIBRARY} MATCHES "framework")
    
    IF(LUA_5.0_LIBRARIES)
      SET(LUA_5.0_FOUND TRUE)
    ENDIF()
  ENDIF(NOT _version_compare EQUAL 0)
ENDIF(LUA_5.0_INCLUDE_DIR)

# Pick the right version
IF(Lua_FIND_VERSION_EXACT AND NOT Lua_FIND_VERSION MATCHES "^[0-9]*$")
  STRING(REGEX REPLACE "^([0-9]+\\.[0-9]+)(\\..*)?$" "\\1" LUA_VERSION_SELECTION ${Lua_FIND_VERSION})
ELSE()
  IF(LUA_5.1_FOUND) # Prefer version 5.1 if found
    SET(LUA_VERSION_SELECTION "5.1")
  ELSEIF(LUA_5.0_FOUND)
    SET(LUA_VERSION_SELECTION "5.0")
  ENDIF()
ENDIF()

SET(LUA_INCLUDE_DIR "${LUA_${LUA_VERSION_SELECTION}_INCLUDE_DIR}")
SET(LUA_LIBRARIES "${LUA_${LUA_VERSION_SELECTION}_LIBRARIES}")
SET(LUA_VERSION_LONG "${LUA_${LUA_VERSION_SELECTION}_VERSION}")
SET(LUA_VERSION "${LUA_VERSION_SELECTION}")

FIND_PACKAGE_HANDLE_ADVANCED_ARGS(Lua DEFAULT_MSG "${LUA_VERSION_LONG}"
  LUA_INCLUDE_DIR
  LUA_LIBRARIES
)

# Include the math library for Unix only
IF(LUA_FOUND)
  IF(UNIX AND NOT APPLE)
    FIND_LIBRARY(UNIX_MATH_LIBRARY m)
    SET(LUA_LIBRARIES ${LUA_LIBRARIES} ${UNIX_MATH_LIBRARY})
  ENDIF()
ENDIF(LUA_FOUND)

MARK_AS_ADVANCED(
  LUA_5.0_INCLUDE_DIR
  LUA_5.0_LUA_LIBRARY_OPTIMIZED
  LUA_5.0_LUA_LIBRARY_DEBUG
  LUA_5.0_LUALIB_LIBRARY_OPTIMIZED
  LUA_5.0_LUALIB_LIBRARY_DEBUG
  LUA_5.1_INCLUDE_DIR
  LUA_5.1_LIBRARY_OPTIMIZED
  LUA_5.1_LIBRARY_DEBUG
  UNIX_MATH_LIBRARY
)
