INCLUDE(LibraryConfigMinGW)
INCLUDE(LibraryConfigMSVC)
INCLUDE(LibraryConfigTardis)

# Performs the search and sets the variables

# Expand the next statement if newer boost versions than 1.36.1 are released
SET(Boost_ADDITIONAL_VERSIONS 1.37 1.37.0 CACHE STRING "")
FIND_PACKAGE(Boost 1.34 REQUIRED thread filesystem)
# With MSVC, automatic linking is performed for boost. So wee need to tell
# the linker where to find them. Also note that when running FindBoost for the
# first time, it will set ${Boost_LIBRARIES} to "" but afterwards to the libs.
IF (MSVC)
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF (MSVC)
FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
FIND_PACKAGE(ENet REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(ALUT REQUIRED)
FIND_PACKAGE(OggVorbis REQUIRED)
FIND_PACKAGE(ZLIB REQUIRED)
FIND_PACKAGE(DirectX REQUIRED)

# Require Lua 5.0 or 5.1
FIND_PACKAGE(Lua50 QUIET)
IF(NOT LUA50_FOUND)
  # Remove variables set by Lua50 and try with Lua51
  SET(LUA_INCLUDE_DIR)
  SET(LUA_LIBRARY_lua)
  SET(LUA_LIBRARY_lualib)
  SET(LUA_LIBRARIES)
  FIND_PACKAGE(Lua51 REQUIRED)
ENDIF(NOT LUA50_FOUND)
# Determine Lua version (Lua50 may also find Lua51)
FILE(STRINGS "${LUA_INCLUDE_DIR}/lua.h" LUA_VERSION REGEX "LUA_VERSION")
STRING(REGEX REPLACE "^.*\"Lua (.*)\".*$" "\\1" LUA_VERSION "${LUA_VERSION}")

# QUIET: Don't require the whole tcl rat tail
FIND_PACKAGE(TCL QUIET)
IF(NOT TCL_FOUND)
  MESSAGE(FATAL_ERROR "Tcl was not found.")
ENDIF(NOT TCL_FOUND)

# Hide variables created by CMake FindXX scripts
MARK_AS_ADVANCED(
  LUA_LIBRARY_lua
  LUA_LIBRARY_lualib
  OPENAL_INCLUDE_DIR
  OPENAL_LIBRARY
)
