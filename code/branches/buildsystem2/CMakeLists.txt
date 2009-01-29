PROJECT(Orxonox C CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

SET(ORXONOX_VERSION_MAJOR 0)
SET(ORXONOX_VERSION_MINOR 1)
SET(ORXONOX_VERSION_PATCH 0)
SET(ORXONOX_VERSION 0.1.0)

# Keep devs from using the root directory as binary directory (messes up the source tree)
IF(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
  MESSAGE(FATAL_ERROR "Do not use the root directory as CMake output directory! mkdir build; cd build; cmake ..")
ENDIF(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})

# This sets where to look for modules (e.g. "Find*.cmake" files)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Set binary output directories
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set Debug build to default when not having multi-config generator like msvc
IF(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  SET(CMAKE_BUILD_TYPE "Debug")
ENDIF(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

############## Platform Config ##################

# Configure platform specific options
INCLUDE(ConfigPlatforms)

################ Test options ###################

OPTION(ENABLE_TESTS "Enable build tests.")
IF(ENABLE_TESTS)
  ENABLE_TESTING()
ENDIF(ENABLE_TESTS)

OPTION(NETWORK_TESTING_ENABLED "Build network testing tools: i.e. chatclient chatserver and alike.")
OPTION(NETWORKTRAFFIC_TESTING_ENABLED "Build dummyserver4 and dummyclient4.")

############### Library finding #################

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

############### Orxonox Source ##################

ADD_SUBDIRECTORY(src)

# Configure the binary output directory
ADD_SUBDIRECTORY(bin-config)
