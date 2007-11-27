PROJECT(Orxonox)

# create a few variables to simplify life
SET( ORXONOX_SRC
	orxonox.cc
)

#Creates an executable
ADD_EXECUTABLE(../bin/main ${ORXONOX_SRC} )

#add weapon source dir
ADD_SUBDIRECTORY(hud)

#Links the executable against OGRE and OIS
TARGET_LINK_LIBRARIES(../bin/main
	HUD
	${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_OGRE_LIBRARIES}
)