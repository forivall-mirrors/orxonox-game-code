PROJECT(Orxonox)

#set some global variables, which are use throughout the project

#Create some verbose output
SET(CMAKE_VERBOSE_MAKEFILE TRUE)

#force-set the compile on tardis machines, as default points to g++-3.3
# only run this test on a lunix/unix machine
IF (UNIX)
  FIND_PROGRAM(UNAME_CMD "uname"
	PATHS "/usr/bin /bin")
  IF(NOT UNAME_CMD)
	MESSAGE("Unable to find uname. Tardis-Check cannot be done.")
  ENDIF(NOT UNAME_CMD)
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
  MESSAGE("System is a TARDIS: Setting Compiler to g++-3.4.3")
  SET(CMAKE_CXX_COMPILER "g++-3.4.3")
  ENDIF(IS_TARDIS)
ENDIF (UNIX)


#This sets where to look for "Find*.cmake" files
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#Performs the search and sets the variables
FIND_PACKAGE(OGRE)
FIND_PACKAGE(OIS)
FIND_PACKAGE(CEGUI)
FIND_PACKAGE(CEGUI_OGRE)

#Sets the search paths for the linking
LINK_DIRECTORIES(${OGRE_LIB_DIR} ${OIS_LIB_DIR} ${CEGUI_LIB_DIR} ${CEGUI_OGRE_LIB_DIR})

#Sets the search path for include files
#INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIR} ${OIS_INCLUDE_DIR} ${CEGUI_INCLUDE_DIR} ${CEGUI_OGRE_INCLUDE_DIR} -I/usr/include/enet)
INCLUDE_DIRECTORIES(
	src
	${OGRE_INCLUDE_DIR} ${OIS_INCLUDE_DIR} ${CEGUI_INCLUDE_DIR} ${CEGUI_OGRE_INCLUDE_DIR}
	)

#add main source dir
ADD_SUBDIRECTORY(src)


