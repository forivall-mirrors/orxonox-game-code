PROJECT(Orxonox)

# create a few variables to simplify life
SET(SRC_FILES
	orxonox.cc
	main.cc
	ammunition_dump.cc
	barrel_gun.cc
	bullet.cc
	camera_manager.cc
	ogre_control.cc
	orxonox_scene.cc
	orxonox_ship.cc
	run_manager.cc
	weapon_manager.cc)
SET(INC_FILES
	ammunition_dump.h
	barrel_gun.h
	bullet.h
	camera_manager.h
	ogre_control.h
	orxonox.h
	orxonox_prerequisites.h
	orxonox_scene.h
	orxonox_ship.h
	run_manager.h
	weapon.h
	weapon_manager.h)

#Creates an executable
ADD_EXECUTABLE(../../bin/main ${SRC_FILES} ${INC_FILES})
#Links the executable against OGRE and OIS
TARGET_LINK_LIBRARIES(../../bin/main ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_OGRE_LIBRARIES})