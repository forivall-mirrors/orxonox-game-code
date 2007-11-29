PROJECT(Orxonox)

SET( ORXONOX_SRC_FILES
	orxonox.cc
	spaceship_steering.cc
)

ADD_EXECUTABLE(../../bin/main ${ORXONOX_SRC_FILES})

SET_TARGET_PROPERTIES(../../bin/main LINK_FLAGS "--no-undefined" )


ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(hud)
ADD_SUBDIRECTORY(objects)
ADD_SUBDIRECTORY(weapon)


TARGET_LINK_LIBRARIES( ../../bin/main
	${OGRE_LIBRARIES}
	${OIS_LIBRARIES}
	${ENet_LIBRARY}
	loader
	network
	core
	hud
	objects
	weapon
)
