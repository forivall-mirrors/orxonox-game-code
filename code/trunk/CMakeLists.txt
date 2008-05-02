PROJECT(Orxonox)

cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#This sets where to look for modules (e.g. "Find*.cmake" files)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#Check whether we are on a tardis box
INCLUDE(CheckTardis)


############## Testing options ##################

OPTION(TESTING_ENABLED "Do you want to enable Testing")
IF (TESTING_ENABLED)
  ENABLE_TESTING()
ENDIF(TESTING_ENABLED)

OPTION(NETWORK_TESTING_ENABLED "Do you want to build network testing tools: i.e. chatclient chatserver and alike")


########## Compiler/Linker options ##############

# if on tardis change compiler and reset boost include directory
IF(IS_TARDIS)
  MESSAGE("System is a TARDIS: Setting Compiler to g++-4.1.1")
  # force-set the compiler on tardis machines, as default points to g++-3.3
  SET(CMAKE_CXX_COMPILER "g++-4.1.1")
  # reset Boost serach path
  SET(Boost_INCLUDE_DIR "/usr/pack/boost-1.34.1-sd/i686-debian-linux3.1/include")
ELSE (IS_TARDIS)
  SET(Boost_INCLUDE_DIR "/usr/include/boost")
ENDIF(IS_TARDIS)

#set binary output directories
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/bin/lib)

# global compiler/linker flags. force -O2!
SET(CMAKE_C_FLAGS "$ENV{CFLAGS} -O2 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O2 -Wall -g -ggdb")
SET(CMAKE_LD_FLAGS "$ENV{LDFLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS " --no-undefined")
SET(CMAKE_SHARED_LINKER_FLAGS " --no-undefined")
SET(CMAKE_MODULE_LINKER_FLAGS " --no-undefined")

#use 'cmake -D make_verb:1 path' to get verbose make output when compiling
IF (MAKE_VERB)
 SET(CMAKE_VERBOSE_MAKEFILE TRUE)
ENDIF (MAKE_VERB)


############### Library finding #################

#Performs the search and sets the variables
FIND_PACKAGE(OGRE)
#FIND_PACKAGE(OIS)
# FIND_PACKAGE(CEGUI)
# FIND_PACKAGE(CEGUI_OGRE)
FIND_PACKAGE(ENet)
FIND_PACKAGE(Boost REQUIRED thread)
FIND_PACKAGE(OpenAL)
FIND_PACKAGE(ALUT)
FIND_PACKAGE(OggVorbis)
FIND_PACKAGE(ZLIB)
FIND_PACKAGE(Lua)
FIND_PACKAGE(TCL)

#Set the search paths for the linking
LINK_DIRECTORIES(
  ${OGRE_LIB_DIR}
#  ${OIS_LIB_DIR}
#  ${CEGUI_LIB_DIR} ${CEGUI_OGRE_LIB_DIR}
  ${ENet_LIBRARY}
  ${Boost_LIBRARY_DIRS}
  ${Zlib_LIBRARY_DIR}
  ${TCL_LIBRARY}
)

#Set the search paths for include files
INCLUDE_DIRECTORIES(
  ${OGRE_INCLUDE_DIR}
#  ${OIS_INCLUDE_DIR}
#  ${CEGUI_INCLUDE_DIR} ${CEGUI_OGRE_INCLUDE_DIR}
  ${ENet_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${OPENAL_INCLUDE_DIR}
  ${ALUT_INCLUDE_DIR}
  ${VORBIS_INCLUDE_DIR}
  ${OGG_INCLUDE_DIR}
  ${Lua_INCLUDE_DIR}
  ${TCL_INCLUDE_PATH}
)

#add main source dir
ADD_SUBDIRECTORY(src)

