PROJECT(Orxonox)

# create a few variables to simplify life

SET(SRC_FILES orxonox.cc loader/LevelLoader.cc xml/xmlParser.cc spaceship_steering.cc)
SET(INC_FILES loader/LevelLoader.h xml/xmlParser.h spaceship_steering.h )

#Creates an executable
ADD_EXECUTABLE(../bin/main ${SRC_FILES} ${INC_FILES})
#Links the executable against OGRE and OIS
TARGET_LINK_LIBRARIES(../bin/main ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_OGRE_LIBRARIES})

