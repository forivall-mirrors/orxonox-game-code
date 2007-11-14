PROJECT(Orxonox)

# create a few variables to simplify life
SET(SRC_FILES
	orxonox.cc
	main.cc
	camera_manager.cc
	inertial_node.cc
	ogre_control.cc
	orxonox_scene.cc
	orxonox_ship.cc
	run_manager.cc)

SET(INC_FILES
	camera_manager.h
	inertial_node.h
	ogre_control.h
	orxonox.h
	orxonox_prerequisites.h
	orxonox_scene.h
	orxonox_ship.h
	run_manager.h)

#Creates an executable
ADD_EXECUTABLE(../bin/main ${SRC_FILES, weapon/WPN_CC_FILES} ${INC_FILES, weapon/WPN_H_FILES})
#Links the executable against OGRE and OIS
TARGET_LINK_LIBRARIES(../bin/main ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_OGRE_LIBRARIES})

#add weapon source dir
ADD_SUBDIRECTORY(weapon)