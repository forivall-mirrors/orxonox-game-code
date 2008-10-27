SET( ORXONOX_SRC_FILES
  GraphicsEngine.cc
  Main.cc
  Settings.cc
  SignalHandler.cc

  gamestates/GSClient.cc
  gamestates/GSDedicated.cc
  gamestates/GSGraphics.cc
  gamestates/GSGUI.cc
  gamestates/GSIOConsole.cc
  gamestates/GSLevel.cc
  gamestates/GSRoot.cc
  gamestates/GSServer.cc
  gamestates/GSStandalone.cc

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
  objects/Radar.cc
  objects/RadarListener.cc
  objects/RadarViewable.cc
  objects/Skybox.cc
  objects/SpaceShip.cc
  objects/SpaceShipAI.cc
  objects/Tickable.cc
  objects/WorldEntity.cc

  objects/Projectile.cc
  objects/BillboardProjectile.cc
  objects/RotatingProjectile.cc
  objects/ParticleProjectile.cc
  
  objects/AddQuest.cc
  objects/AddQuestHint.cc
  objects/AddReward.cc
  objects/ChangeQuestStatus.cc
  objects/CompleteQuest.cc
  objects/FailQuest.cc
  objects/GlobalQuest.cc
  objects/LocalQuest.cc
  objects/Quest.cc
  objects/QuestDescription.cc
  objects/QuestEffect.cc
  objects/QuestHint.cc
  objects/QuestItem.cc

  tolua/tolua_bind.cc
)

GET_TARGET_PROPERTY(TOLUA_EXE tolua_orxonox LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tolua/tolua_bind.cc ${CMAKE_CURRENT_SOURCE_DIR}/tolua/tolua_bind.h
  COMMAND ${TOLUA_EXE} -n Orxonox -o ../../src/orxonox/tolua/tolua_bind.cc -H ../../src/orxonox/tolua/tolua_bind.h ../../src/orxonox/tolua/tolua.pkg
  DEPENDS
    tolua_orxonox
    tolua/tolua.pkg
    gui/GUIManager.h
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
  ${CEGUI_LIBRARIES}
  lua_orxonox
  ceguilua_orxonox
  tinyxml_orxonox
  tolualib_orxonox
  util
  core
  audio
  network
)

