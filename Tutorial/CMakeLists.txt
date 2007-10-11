PROJECT(Tutorial)

#force-set the compile on tardis machines, as default points to g++-3.3
IF (UNIX)
  FIND_PROGRAM(UNAME_CMD "uname"
	PATHS "/usr/bin /bin")
  EXECUTE_PROCESS(
	COMMAND "${UNAME_CMD}" "-n"
	RESULT_VARIABLE UNAME_RV
	ERROR_VARIABLE UNAME_EV
	OUTPUT_VARIABLE UNAME_OV)
#MESSAGE("Print: UNAME_RV ${UNAME_RV}")
  IF (NOT "${UNAME_RV}" STREQUAL "0")
	MESSAGE("ERROR: Unable to find uname. System cannot be marked as
	Tardis if needed.")
  ENDIF (NOT "${UNAME_RV}" STREQUAL "0")
  #STRING(REGEX MATCH "tardis" UNAME_OV "${UNAME_OV}")
  IF ("${UNAME_OV}" MATCHES "tardis")
    SET (IS_TARDIS "tardis")
  ENDIF ("${UNAME_OV}" MATCHES "tardis")
  #MESSAGE("Print: UNAME_OV ${UNAME_OV}")
  IF (IS_TARDIS)
  MESSAGE("System is a TARDIS: Setting Compiler to g++-3.4.3")
  SET(CMAKE_CXX_COMPILER "g++-3.4.3")
  ENDIF(IS_TARDIS)
ENDIF (UNIX)

SET(SRC_FILES src/main.cpp)

SET(INC_FILES src/ExampleApplication.h  src/ExampleFrameListener.h  src/ExampleLoadingBar.h)

#This sets where to look for "Find*.cmake" files
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#Performs the search and sets the variables
FIND_PACKAGE(OGRE)
FIND_PACKAGE(OIS)

#Sets the search paths for the linking
LINK_DIRECTORIES(${OGRE_LIB_DIR} ${OIS_LIB_DIR})
#Sets the search path for include files
INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIR} ${OIS_INCLUDE_DIR})

#Creates an executable
ADD_EXECUTABLE(main ${SRC_FILES} ${INC_FILES})
#Links the executable against Ogre
TARGET_LINK_LIBRARIES(main ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

#Sets the install path
INSTALL(TARGETS main DESTINATION bin)
