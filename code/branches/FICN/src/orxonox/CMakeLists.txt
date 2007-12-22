PROJECT(Orxonox)

ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(hud)
IF(NOT WIN32)
  ADD_SUBDIRECTORY(objects)
ENDIF(NOT WIN32)
ADD_SUBDIRECTORY(particle)


SET( ORXONOX_SRC_FILES
  Orxonox.cc
  SpaceshipSteering.cc
  Main.cc
  GraphicsEngine.cc
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
#    objects/SceneNode.cc
#    objects/Entity.cc
    objects/Camera.cc
#    objects/SpaceshipSteeringObject.cc
    objects/Model.cc
    objects/Mesh.cc
#    objects/BaseEntity.cc
    objects/SpaceShip.cc
    objects/NPC.cc
    objects/BillboardSet.cc
#    objects/Light.cc
    objects/Projectile.cc
    objects/weapon_system/AmmunitionDump.cc
    objects/weapon_system/BarrelGun.cc
    objects/weapon_system/BaseWeapon.cc
    objects/weapon_system/Bullet.cc
    objects/weapon_system/BulletManager.cc
    objects/weapon_system/WeaponStation.cc
    objects/Explosion.cc
  )
ELSE(WIN32)
  SET(LINK_OBJECTS_LIBRARY objects)
ENDIF(WIN32)

IF(WIN32)
#  ADD_LIBRARY(orxonox ${ORXONOX_SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(orxonox SHARED ${ORXONOX_SRC_FILES})
ENDIF(WIN32)

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
  xml
  particle
)

