PROJECT(ToLua)

#This sets where to look for modules (e.g. "Find*.cmake" files)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#Check whether we are on a tardis box
INCLUDE(CheckTardis)

########## Compiler/Linker options ##############

# if on tardis change compiler and reset boost include directory
IF(IS_TARDIS)
  MESSAGE("System is a TARDIS: Setting Compiler to g++-4.1.1")
  # force-set the compiler on tardis machines, as default points to g++-3.3
  SET(CMAKE_CXX_COMPILER "g++-4.1.1")
ENDIF(IS_TARDIS)

#set binary output directories
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/bin/lib)

# global compiler/linker flags. force -O2!
SET(CMAKE_C_FLAGS "$ENV{CFLAGS} -O2 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O2 -Wall -g -ggdb")
SET(CMAKE_LD_FLAGS "$ENV{LDFLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS " --no-undefined")
SET(CMAKE_SHARED_LINKER_FLAGS " --no-undefined")
SET(CMAKE_MODULE_LINKER_FLAGS " --no-undefined")

#Create verbose makefile output when compiling
SET(CMAKE_VERBOSE_MAKEFILE TRUE)


############### Library finding #################

#Performs the search and sets the variables
FIND_PACKAGE(Lua)

#Set the search paths for the linking
LINK_DIRECTORIES(
  ${Lua_LIBRARIES}
)

#Set the search paths for include files
INCLUDE_DIRECTORIES(
  ${Lua_INCLUDE_DIR}
)


################ Source files ###################

#add main source dir
ADD_SUBDIRECTORY(src/util/tolua)
