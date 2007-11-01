PROJECT(Orxonox)

# create a few variables to simplify life
SET(SRC_FILES orxonox.cc loader/LevelLoader.cc xml/xmlParser.cc)
SET(INC_FILES ExampleApplication.h  ExampleFrameListener.h loader/LevelLoader.h xml/xmlParser.h)

#Creates an executable
ADD_EXECUTABLE(../bin/main ${SRC_FILES} ${INC_FILES})
#Links the executable against OGRE and OIS
TARGET_LINK_LIBRARIES(../bin/main ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

