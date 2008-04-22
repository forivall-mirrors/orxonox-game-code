SET( ORXONOX_SRC_FILES
  GraphicsEngine.cc
  Main.cc
  Orxonox.cc
#  SpaceshipSteering.cc
  hud/HUD.cc
  particle/ParticleInterface.cc
  tools/BillboardSet.cc
  tools/Light.cc
  tools/Mesh.cc
  tools/Timer.cc
  objects/Ambient.cc
  objects/Camera.cc
  objects/Explosion.cc
#  objects/Fighter.cc
  objects/Model.cc
  objects/NPC.cc
  objects/Projectile.cc
  objects/Skybox.cc
  objects/SpaceShip.cc
#  objects/SpaceshipSteeringObject.cc
#  objects/test1.cc
#  objects/test2.cc
#  objects/test3.cc
  objects/WorldEntity.cc
#  objects/weapon/AmmunitionDump.cc
#  objects/weapon/BarrelGun.cc
#  objects/weapon/BaseWeapon.cc
#  objects/weapon/Bullet.cc
#  objects/weapon/BulletManager.cc
#  objects/weapon/WeaponStation.cc
  tolua/tolua_bind.cc
)

GET_TARGET_PROPERTY(TOLUA_EXE tolua LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT tolua/tolua_bind.cc
  COMMAND ${TOLUA_EXE} -n orxonox -o orxonox/tolua/tolua_bind.cc -H orxonox/tolua/tolua_bind.h orxonox/tolua/tolua.pkg
  DEPENDS tolua
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
)
	
ADD_EXECUTABLE( orxonox ${ORXONOX_SRC_FILES} )

TARGET_LINK_LIBRARIES( orxonox
  ${OGRE_LIBRARIES}
  ${Lua_LIBRARIES}
  util
  core
  audio
  network
)

