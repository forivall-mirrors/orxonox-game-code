PROJECT(Orxonox)

INCLUDE_DIRECTORIES(../tools)

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
  weapon_system/AmmunitionDump.cc
  weapon_system/BarrelGun.cc
  weapon_system/BaseWeapon.cc
  weapon_system/Bullet.cc
  weapon_system/BulletManager.cc
  weapon_system/WeaponStation.cc
)

ADD_LIBRARY(objects SHARED ${OBJECTS_SRC_FILES})

TARGET_LINK_LIBRARIES( objects
  util
  orxonox
  tools
)
