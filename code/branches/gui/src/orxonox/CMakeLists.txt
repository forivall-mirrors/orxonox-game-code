SET( ORXONOX_SRC_FILES
  GraphicsEngine.cc
  Main.cc
  Orxonox.cc
  Radar.cc
  RadarListener.cc
  RadarViewable.cc
  Settings.cc

  gui/GUIManager.cc
  gui/OgreCEGUIRenderer.cpp
  gui/OgreCEGUIResourceProvider.cpp
  gui/OgreCEGUITexture.cpp

  overlays/OrxonoxOverlay.cc
  overlays/OverlayGroup.cc
  overlays/OverlayText.cc

  overlays/console/InGameConsole.cc

  overlays/debug/DebugFPSText.cc
  overlays/debug/DebugRTRText.cc

  overlays/hud/HUDBar.cc
  overlays/hud/HUDNavigation.cc
  overlays/hud/HUDRadar.cc
  overlays/hud/HUDSpeedBar.cc

  tolua/tolua_bind.cc

  tools/BillboardSet.cc
  tools/Light.cc
  tools/Mesh.cc
  tools/ParticleInterface.cc
  tools/TextureGenerator.cc
  tools/Timer.cc
  tools/WindowEventListener.cc

  objects/Ambient.cc
  objects/Backlight.cc
  objects/Camera.cc
  objects/CameraHandler.cc
  objects/Model.cc
  objects/NPC.cc
  objects/ParticleSpawner.cc
  objects/Skybox.cc
  objects/SpaceShip.cc
  objects/SpaceShipAI.cc
  objects/Tickable.cc
  objects/WorldEntity.cc

  objects/Projectile.cc
  objects/BillboardProjectile.cc
  objects/RotatingProjectile.cc
  objects/ParticleProjectile.cc
)

GET_TARGET_PROPERTY(TOLUA_EXE tolua LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tolua/tolua_bind.cc
  COMMAND ${TOLUA_EXE} -n orxonox -o ../../src/orxonox/tolua/tolua_bind.cc -H ../../src/orxonox/tolua/tolua_bind.h ../../src/orxonox/tolua/tolua.pkg
  DEPENDS tolua
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib
)
	
ADD_EXECUTABLE( orxonox ${ORXONOX_SRC_FILES} )

IF(NETWORKTRAFFIC_TESTING_ENABLED)

  SET( ORXONOXS_SRC_FILES
    GraphicsEngine.cc
    objects/Ambient.cc
    objects/Camera.cc
    objects/CameraHandler.cc
    objects/Explosion.cc
    objects/Model.cc
    objects/NPC.cc
    objects/Projectile.cc
    objects/Skybox.cc
    objects/SpaceShip.cc
    objects/WorldEntity.cc
  )

  ADD_LIBRARY(orxonoxs SHARED ${ORXONOX_SRC_FILES})
ENDIF(NETWORKTRAFFIC_TESTING_ENABLED)

TARGET_LINK_LIBRARIES( orxonox
  ${OGRE_LIBRARIES}
  ${Lua_LIBRARIES}
  ${CEGUI_LIBRARIES}
  tinyxml
  tolualib
  ois_orxonox
  util
  core
  audio
  network
)

