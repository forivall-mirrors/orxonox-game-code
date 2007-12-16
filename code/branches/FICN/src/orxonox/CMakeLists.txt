PROJECT(Orxonox)

ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(hud)
IF(NOT WIN32)
  ADD_SUBDIRECTORY(objects)
ENDIF(NOT WIN32)
ADD_SUBDIRECTORY(weapon)
ADD_SUBDIRECTORY(particle)


SET( ORXONOX_SRC_FILES
  orxonox.cc
  orxonox_scene.cc
  orxonox_ship.cc
  run_manager.cc
  spaceship_steering.cc
  inertial_node.cc
  main.cc
  graphicsEngine.cc
)

IF(WIN32)
  SET( OBJECTS_SRC_FILES
    objects/BaseObject.cc
    objects/WorldEntity.cc
    objects/test1.cc
    objects/test2.cc
    objects/test3.cc
    objects/Ambient.cc
    objects/Skybox.cc
    objects/SceneNode.cc
    objects/Entity.cc
    objects/Camera.cc
    objects/SpaceshipSteeringObject.cc
  )
ELSE(WIN32)
  SET(LINK_OBJECTS_LIBRARY objects)
ENDIF(WIN32)

ADD_LIBRARY(orxonox ${ORXONOX_SRC_FILES})

ADD_EXECUTABLE(../../bin/main ${ORXONOX_SRC_FILES} ${OBJECTS_SRC_FILES})

SET_TARGET_PROPERTIES(../../bin/main PROPERTIES LINK_FLAGS "--no-undefined" )

TARGET_LINK_LIBRARIES( ../../bin/main
  ${OGRE_LIBRARIES}
  ${OIS_LIBRARIES}
  loader
  audio
  network
  core
  hud
  ${LINK_OBJECTS_LIBRARY}
  weapon
  xml
  particle
)

