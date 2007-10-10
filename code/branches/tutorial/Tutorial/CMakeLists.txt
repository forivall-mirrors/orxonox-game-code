PROJECT(Tutorial)

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
ADD_EXECUTABLE(build/main ${SRC_FILES} ${INC_FILES})
#Links the executable against Ogre
TARGET_LINK_LIBRARIES(build/main ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

#Sets the install path
INSTALL(TARGETS build/main DESTINATION bin)