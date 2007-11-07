PROJECT(Orxonox)

# create a few variables to simplify life
SET(SRC_FILES
	ammunition_dump.cc
	bullet.cc
	bullet_manager.cc
	weapon_manager.cc)
SET(INC_FILES
	ammunition_dump.h
	bullet.h
	bullet_manager.h
	weapon.h
	weapon_manager.h)

#Creates an executable
ADD_EXECUTABLE(../../bin/main ${SRC_FILES} ${INC_FILES})
#Links the executable against OGRE and OIS
TARGET_LINK_LIBRARIES(../../bin/main ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_OGRE_LIBRARIES})