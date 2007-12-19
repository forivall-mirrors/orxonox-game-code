PROJECT(Orxonox)

SET( OBJECTS_SRC_FILES
  BaseObject.cc
  WorldEntity.cc
  test1.cc
  test2.cc
  test3.cc
  Ambient.cc
  Skybox.cc
  SceneNode.cc
  BaseEntity.cc
  Entity.cc
  Camera.cc
#  SpaceshipSteeringObject.cc
  Model.cc
  Mesh.cc
  SpaceShip.cc
  NPC.cc
  BillboardSet.cc
  Light.cc
  Projectile.cc
  weapon_system/ammunition_dump.cc
  weapon_system/barrel_gun.cc
  weapon_system/base_weapon.cc
  weapon_system/bullet.cc
  weapon_system/bullet_manager.cc
  weapon_system/weapon_station.cc
)

ADD_LIBRARY(objects SHARED ${OBJECTS_SRC_FILES})

TARGET_LINK_LIBRARIES( objects
  xml
  orxonox
)
