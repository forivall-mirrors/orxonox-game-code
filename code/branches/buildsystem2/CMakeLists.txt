PROJECT(Orxonox)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

############### Various Options #################

# Keep devs from using the root directory as binary directory (messes up the source tree)
IF(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
  MESSAGE(FATAL_ERROR "Do not use the root directory as CMake output directory! mkdir build; cd build; cmake ..")
ENDIF(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})

# This sets where to look for modules (e.g. "Find*.cmake" files)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set binary output directories
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/bin)
MARK_AS_ADVANCED(EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH)

# Set Debug build to default when not having multi-config generator like msvc
IF(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  SET(CMAKE_BUILD_TYPE "Debug")
ENDIF(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

############## Platform Config ##################

# Configre platform specific options
INCLUDE(ConfigPlatforms)

################ Test options ###################

OPTION(ENABLE_TESTS "Enable build tests.")
IF(ENABLE_TESTS)
  ENABLE_TESTING()
ENDIF(ENABLE_TESTS)

OPTION(NETWORK_TESTING_ENABLED "Build network testing tools: i.e. chatclient chatserver and alike.")
OPTION(NETWORKTRAFFIC_TESTING_ENABLED "Build dummyserver4 and dummyclient4.")

############### Library finding #################

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

# Performs the search and sets the variables
FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
FIND_PACKAGE(ENet REQUIRED)
FIND_PACKAGE(Boost 1.34 REQUIRED thread filesystem)
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
ADD_SUBDIRECTORY(bin)
