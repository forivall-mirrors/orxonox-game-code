ADD_SUBDIRECTORY(core)

SET( ORXONOX_SRC_FILES
  GraphicsEngine.cc
  Main.cc
  Orxonox.cc
  SpaceshipSteering.cc
  hud/Hud.cc
  particle/ParticleInterface.cc
  tools/BillboardSet.cc
  tools/Light.cc
  tools/Mesh.cc
  objects/Ambient.cc
  objects/Camera.cc
  objects/Explosion.cc
  objects/Fighter.cc
  objects/Model.cc
  objects/NPC.cc
  ojbects/Projectile.cc
  objects/Skybox.cc
  objects/SpaceShip.cc
# objects/SpaceshipSteeringObject.cc
# objects/test1.cc
# objects/test2.cc
# objects/test3.cc
  objects/WorldEntity.cc
  objects/weapon/AmmunitionDump.cc
  objects/weapon/BarrelGun.cc
  objects/weapon/BaseWeapon.cc
  objects/weapon/Bullet.cc
  objects/weapon/BulleManager.cc
  objects/weapon/WeaponStation.cc
)

ADD_EXECUTABLE( main ${ORXONOX_SRC_FILES} )

SET_TARGET_PROPERTIES( main PROPERTIES LINK_FLAGS "--no-undefined" )

TARGET_LINK_LIBRARIES( main
  ${OGRE_LIBRARIES}
  ${OIS_LIBRARIES}
  util
  core
  audio
  network
  loader
)

