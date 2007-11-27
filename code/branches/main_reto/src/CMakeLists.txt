PROJECT(Orxonox)

# create a few variables to simplify life
SET( ORXONOX_SRC
	camera_manager.cc
	inertial_node.cc
	main.cc
	ogre_control.cc
	orxonox.cc
	orxonox_scene.cc
	orxonox_ship.cc
	run_manager.cc

	camera_manager.h
	inertial_node.h
	ogre_control.h
	orxonox.h
	orxonox_prerequisites.h
	orxonox_scene.h
	orxonox_ship.h
	run_manager.h
   )

#Creates an executable
ADD_EXECUTABLE(../bin/main ${ORXONOX_SRC} )

#add weapon source dir
ADD_SUBDIRECTORY(weapon)
ADD_SUBDIRECTORY(class_hierarchy)
ADD_SUBDIRECTORY(hud)

#Links the executable against OGRE and OIS
TARGET_LINK_LIBRARIES(../bin/main
	WEAPON
	CLASS_HIERARCHY
	HUD
	${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_OGRE_LIBRARIES}
	)
#TARGET_LINK_LIBRARIES(../bin/main ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_OGRE_LIBRARIES})
