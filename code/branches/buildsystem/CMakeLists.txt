PROJECT(Orxonox)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# TODO: Test CMake Modules without policy
#IF (COMMAND cmake_policy)
#  cmake_policy(SET CMP0003 NEW)
#ENDIF (COMMAND cmake_policy)

# This sets where to look for modules (e.g. "Find*.cmake" files)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Check for special include paths
INCLUDE(CheckTardis)
INCLUDE(CheckOrxonoxWinDeps)

################ Test options ###################

OPTION(ENABLE_TESTS "Enable build tests.")
IF(ENABLE_TESTS)
  ENABLE_TESTING()
ENDIF(ENABLE_TESTS)

OPTION(NETWORK_TESTING_ENABLED "Build network testing tools: i.e. chatclient chatserver and alike.")
OPTION(NETWORKTRAFFIC_TESTING_ENABLED "Build dummyserver4 and dummyclient4.")

########## Compiler/Linker options ##############

# Set binary output directories
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/bin)
MARK_AS_ADVANCED(EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH)

# TODO: use CMake's options for build types
# Global compiler/linker flags.
SET(CMAKE_C_FLAGS "$ENV{CFLAGS} -Wall -Wextra -Wno-unused-parameter -g -ggdb")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall -Wextra -Wno-unused-parameter -g -ggdb")
#SET(CMAKE_C_FLAGS "$ENV{CFLAGS} -O3")
#SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3")

SET(CMAKE_LD_FLAGS "$ENV{LDFLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS " --no-undefined")
SET(CMAKE_SHARED_LINKER_FLAGS " --no-undefined")
SET(CMAKE_MODULE_LINKER_FLAGS " --no-undefined")

############### Library finding #################

# Performs the search and sets the variables
FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
# CEGUI_OGRE included in source
#FIND_PACKAGE(CEGUI_OGRE REQUIRED)
FIND_PACKAGE(ENet REQUIRED)
FIND_PACKAGE(Boost 1.34 REQUIRED thread filesystem)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(ALUT REQUIRED)
FIND_PACKAGE(OggVorbis REQUIRED)
FIND_PACKAGE(ZLIB REQUIRED)
FIND_PACKAGE(DirectX REQUIRED)
FIND_PACKAGE(Lua50 REQUIRED)
#FIND_PACKAGE(Lua51 QUIET)

#don't require the whole tcl rat tail
FIND_PACKAGE(TCL QUIET)
IF(NOT TCL_FOUND)
    MESSAGE(FATAL_ERROR "Tcl not found.")
ENDIF(NOT TCL_FOUND)

# Add main source dir
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(bin)
