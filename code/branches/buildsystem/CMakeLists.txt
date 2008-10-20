PROJECT(Orxonox)

cmake_minimum_required(VERSION 2.4)

IF (COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
ENDIF (COMMAND cmake_policy)

# Allow for simple IF constructs (makes code a lot more readable)
#SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

# This sets where to look for modules (e.g. "Find*.cmake" files)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Check whether we are on a tardis box and set customized paths
INCLUDE(CheckTardis)

############## Testing options ##################

#OPTION(TESTING_ENABLED "Do you want to enable Testing")
#IF (TESTING_ENABLED)
#  ENABLE_TESTING()
#ENDIF(TESTING_ENABLED)
#
#OPTION (NETWORK_TESTING_ENABLED "Do you want to build network testing tools: i.e. chatclient chatserver and alike")
#OPTION (NETWORKTRAFFIC_TESTING_ENABLED "Do you want to build dummyserver4 and dummyclient4")

########## Compiler/Linker options ##############

# Set binary output directories
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/bin)

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

# Uncomment the following line if you want the include and library paths to be shown
# Alternatively use the "-D VERBOSE_FIND:BOOL=true" option when executing cmake

# SET (VERBOSE_FIND true)

# Set the library directory when using precompiled tarballs for the dependencies under windows
IF (WIN32)
  IF (MSVC)
    IF ($ENV{ORXONOX_DEV})
      SET (DEPENDENCY_DIR ${CMAKE_SOURCE_DIR}/../lib_dist/dependencies/)
    ELSE ($ENV{ORXONOX_DEV})
      SET (DEPENDENCY_DIR ${CMAKE_SOURCE_DIR}/dependencies/)
    ENDIF ($ENV{ORXONOX_DEV})
    SET(Boost_LIB_SUFFIX_DEBUG vc80-mt-gd-1_35)
    #MESSAGE(STATUS ${DEPENDENCY_DIR})
  ENDIF (MSVC)
ENDIF (WIN32)

# Performs the search and sets the variables
FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
#FIND_PACKAGE(CEGUI_OGRE REQUIRED)
FIND_PACKAGE(ENet REQUIRED)
FIND_PACKAGE(Boost REQUIRED thread filesystem)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(ALUT REQUIRED)
FIND_PACKAGE(OggVorbis REQUIRED)
FIND_PACKAGE(ZLIB REQUIRED)
FIND_PACKAGE(TCL REQUIRED)
FIND_PACKAGE(DirectX REQUIRED)

FIND_PACKAGE(Lua51)
IF(NOT LUA_FOUND)
  FIND_PACKAGE(Lua50 REQUIRED)
ENDIF(NOT LUA_FOUND)

# Add main source dir
ADD_SUBDIRECTORY(src)
