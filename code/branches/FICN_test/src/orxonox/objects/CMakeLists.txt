PROJECT(Orxonox)

ADD_SUBDIRECTORY(weapon)

SET( OBJECTS_SRC_FILES
  Ambient.cc
  Camera.cc
  Explosion.cc
  Fighter.cc
  Model.cc
  NPC.cc
  Projectile.cc
  Skybox.cc
  SpaceShip.cc
#  SpaceshipSteeringObject.cc
  WorldEntity.cc
  test1.cc
  test2.cc
  test3.cc
  ${WEAPON_SRC_FILES}
)

IF (NOT WIN32)
  ADD_LIBRARY(objects SHARED ${OBJECTS_SRC_FILES})

  TARGET_LINK_LIBRARIES( objects
    util
    orxonox
    tools
  )
ENDIF (NOT WIN32)
