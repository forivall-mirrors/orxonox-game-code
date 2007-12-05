PROJECT(Orxonox)

ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(hud)
ADD_SUBDIRECTORY(objects)
ADD_SUBDIRECTORY(weapon)


SET( ORXONOX_SRC_FILES
  orxonox.cc
  orxonox_scene.cc
  orxonox_ship.cc
  run_manager.cc
  spaceship_steering.cc
  inertial_node.cc
)

ADD_EXECUTABLE(../../bin/main ${ORXONOX_SRC_FILES})

SET_TARGET_PROPERTIES(../../bin/main PROPERTIES LINK_FLAGS "--no-undefined" )

TARGET_LINK_LIBRARIES( ../../bin/main 
  ${OGRE_LIBRARIES}
  ${OIS_LIBRARIES}
  loader
  audio
  network
  core
  hud
  objects
  weapon
)

