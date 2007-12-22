PROJECT(Orxonox)

SET( OBJECTS_SRC_FILES
  BaseObject.cc
  WorldEntity.cc
  test1.cc
  test2.cc
  test3.cc
  Ambient.cc
  Skybox.cc
#  SceneNode.cc
  BaseEntity.cc
#  Entity.cc
  Camera.cc
#  SpaceshipSteeringObject.cc
  Model.cc
  Mesh.cc
  SpaceShip.cc
  NPC.cc
  BillboardSet.cc
#  Light.cc
  Projectile.cc
  weapon_system/AmmunitionDump.cc
  weapon_system/BarrelGun.cc
  weapon_system/BaseWeapon.cc
  weapon_system/Bullet.cc
  weapon_system/BulletManager.cc
  weapon_system/WeaponStation.cc
  Explosion.cc
  Fighter.cc
)

ADD_LIBRARY(objects SHARED ${OBJECTS_SRC_FILES})

TARGET_LINK_LIBRARIES( objects
  xml
  orxonox
)
