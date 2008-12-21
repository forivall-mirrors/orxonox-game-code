PROJECT(Orxonox)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

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

# Compiler/Linker flags.
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug")
ENDIF(NOT CMAKE_BUILD_TYPE)
#SET(CMAKE_C_FLAGS   "$ENV{CFLAGS}   -Wall -Wextra -Wno-unused-parameter")
SET(CMAKE_C_FLAGS   "$ENV{CFLAGS}   -Wall")
#SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall -Wextra -Wno-unused-parameter")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall")
SET(CMAKE_C_FLAGS_DEBUG            "-g -ggdb -fPIC")
SET(CMAKE_CXX_FLAGS_DEBUG          "-g -ggdb -fPIC")
SET(CMAKE_C_FLAGS_RELEASE          "-O3 -DNDEBUG -fPIC")
SET(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG -fPIC")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -DNDEBUG -g -ggdb -fPIC")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -DNDEBUG -g -ggdb -fPIC")
SET(CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG -fPIC")
SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG -fPIC")

SET(CMAKE_LD_FLAGS "$ENV{LDFLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS " --no-undefined")
SET(CMAKE_SHARED_LINKER_FLAGS " --no-undefined")
SET(CMAKE_MODULE_LINKER_FLAGS " --no-undefined")

############### Library finding #################

# Performs the search and sets the variables
FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
# CEGUI_OGRE is already included in source
#FIND_PACKAGE(CEGUI_OGRE REQUIRED)
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

# Don't require the whole tcl rat tail
FIND_PACKAGE(TCL QUIET)
IF(NOT TCL_FOUND)
    MESSAGE(FATAL_ERROR "Tcl not found.")
ENDIF(NOT TCL_FOUND)

# Add main source dir
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(bin)
