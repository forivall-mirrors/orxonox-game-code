
PROJECT(Orxonox)
#set some global variables, which are used throughout the project

#Create some verbose output
SET(CMAKE_VERBOSE_MAKEFILE TRUE)

# set boost search path
SET(Boost_INCLUDE_DIR "/usr/include/boost/")

# force-set the compile on tardis machines, as default points to g++-3.3
# only run this test on a lunix/unix machine
IF (UNIX)

  FIND_PROGRAM(UNAME_CMD "uname"
  PATHS "/usr/bin /bin")
  IF(NOT UNAME_CMD)
    MESSAGE("Unable to find uname. Tardis-Check cannot be done.")
  ENDIF(NOT UNAME_CMD)

  # run uname -n to get nodename
  EXECUTE_PROCESS(
  COMMAND "${UNAME_CMD}" "-n"
  RESULT_VARIABLE UNAME_RV
  ERROR_VARIABLE UNAME_EV
  OUTPUT_VARIABLE UNAME_OV)

  IF (NOT "${UNAME_RV}" STREQUAL "0")
    MESSAGE("ERROR: uname terminated unclean.")
  ENDIF (NOT "${UNAME_RV}" STREQUAL "0")

  # check wheter we are on a tardis machine
  IF ("${UNAME_OV}" MATCHES "tardis")
    SET (IS_TARDIS "tardis")
  ENDIF ("${UNAME_OV}" MATCHES "tardis")

  # if on tardis change compiler
  IF (IS_TARDIS)
    MESSAGE("System is a TARDIS: Setting Compiler to g++-4.1.1")
    SET(CMAKE_CXX_COMPILER "g++-4.1.1")
    # reset eNet serach path
    SET(Boost_INCLUDE_DIR "/usr/pack/boost-1.34.1-sd/i686-debian-linux3.1/include")
  ENDIF(IS_TARDIS)

ENDIF (UNIX)

# pipe $FLAGS to the compiler, and add some local flags. force -O2!
SET(CMAKE_C_FLAGS "$ENV{CFLAGS} -O2 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O2 -Wall -g -ggdb")
SET(CMAKE_LD_FLAGS "$ENV{LDFLAGS}")



#This sets where to look for "Find*.cmake" files
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#Performs the search and sets the variables
FIND_PACKAGE(OGRE)
FIND_PACKAGE(OIS)
FIND_PACKAGE(CEGUI)
FIND_PACKAGE(CEGUI_OGRE)
FIND_PACKAGE(ENet)
FIND_PACKAGE(Boost)
FIND_PACKAGE(OpenAL)
FIND_PACKAGE(ALUT)
FIND_PACKAGE(OggVorbis)
FIND_PACKAGE(ZLIB)

#Sets the search paths for the linking
LINK_DIRECTORIES(${OGRE_LIB_DIR} ${OIS_LIB_DIR} ${CEGUI_LIB_DIR} ${CEGUI_OGRE_LIB_DIR} ${ENet_LIBRARY} ${Boost_LIBRARY_DIRS} ${Zlib_LIBRARY_DIR} core objects loader network weapon classHierarchy audio)
#Sets the search path for include files
INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIR} ${OIS_INCLUDE_DIR} ${CEGUI_INCLUDE_DIR} ${CEGUI_OGRE_INCLUDE_DIR} ${ENet_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${OPENAL_INCLUDE_DIR} ${ALUT_INCLUDE_DIR} ${VORBIS_INCLUDE_DIR} ${OGG_INCLUDE_DIR})

#add main source dir
ADD_SUBDIRECTORY(src)
