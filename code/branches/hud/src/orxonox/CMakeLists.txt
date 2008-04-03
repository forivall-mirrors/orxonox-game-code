SET( ORXONOX_SRC_FILES
  GraphicsEngine.cc
  Main.cc
  Orxonox.cc
  SpaceshipSteering.cc
  hud/HUD.cc
  hud/Bar.cc
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
  objects/Projectile.cc
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
  objects/weapon/BulletManager.cc
  objects/weapon/WeaponStation.cc
)

ADD_EXECUTABLE( orxonox ${ORXONOX_SRC_FILES} )

SET_TARGET_PROPERTIES( orxonox PROPERTIES LINK_FLAGS "--no-undefined" )

TARGET_LINK_LIBRARIES( orxonox
  ${OGRE_LIBRARIES}
  ${OIS_LIBRARIES}
  util
  core
  audio
  network
  loader
)

