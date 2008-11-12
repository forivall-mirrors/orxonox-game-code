SET( ORXONOX_SRC_FILES
  CameraManager.cc
  GraphicsEngine.cc
  LevelManager.cc
  Main.cc
  Settings.cc

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
  overlays/hud/ChatOverlay.cc

  tools/BillboardSet.cc
  tools/Mesh.cc
  tools/ParticleInterface.cc
  tools/TextureGenerator.cc
  tools/Timer.cc
  tools/WindowEventListener.cc

  objects/EventListener.cc
  objects/EventDispatcher.cc
  objects/EventTarget.cc
  objects/Radar.cc
  objects/RadarListener.cc
  objects/RadarViewable.cc
  objects/Tickable.cc
  objects/Test.cc
  objects/Script.cc

  objects/Scene.cc
  objects/worldentities/WorldEntity.cc
  objects/worldentities/PositionableEntity.cc
  objects/worldentities/MovableEntity.cc
  objects/worldentities/ControllableEntity.cc
  objects/worldentities/Model.cc
  objects/worldentities/Billboard.cc
  objects/worldentities/BlinkingBillboard.cc
  objects/worldentities/Light.cc
  objects/worldentities/Camera.cc
  objects/worldentities/CameraPosition.cc
  objects/worldentities/SpawnPoint.cc
  objects/worldentities/ParticleEmitter.cc
  objects/worldentities/ParticleSpawner.cc
#  objects/worldentities/Backlight.cc

  objects/quest/AddQuest.cc
  objects/quest/AddQuestHint.cc
  objects/quest/AddReward.cc
  objects/quest/ChangeQuestStatus.cc
  objects/quest/CompleteQuest.cc
  objects/quest/FailQuest.cc
  objects/quest/GlobalQuest.cc
  objects/quest/LocalQuest.cc
  objects/quest/Quest.cc
  objects/quest/QuestDescription.cc
  objects/quest/QuestEffect.cc
  objects/quest/QuestHint.cc
  objects/quest/QuestItem.cc
  objects/quest/QuestManager.cc
  objects/quest/Rewardable.cc

  objects/weaponSystem/WeaponSystem.cc
  objects/weaponSystem/WeaponSet.cc
  objects/weaponSystem/WeaponSlot.cc
  objects/weaponSystem/Weapon.cc
  objects/weaponSystem/Munition.cc
  objects/weaponSystem/weapons/LaserGun.cc
  objects/weaponSystem/munitions/LaserGunMunition.cc
  objects/weaponSystem/projectiles/BillboardProjectile.cc
  objects/weaponSystem/projectiles/ParticleProjectile.cc
  objects/weaponSystem/projectiles/Projectile.cc

  objects/worldentities/triggers/Trigger.cc
  objects/worldentities/triggers/DistanceTrigger.cc
  objects/worldentities/triggers/EventTrigger.cc

  objects/worldentities/pawns/Spectator.cc
  objects/worldentities/pawns/Pawn.cc
  objects/worldentities/pawns/SpaceShip.cc

  objects/controllers/Controller.cc
  objects/controllers/HumanController.cc

  objects/infos/Info.cc
  objects/infos/Level.cc
  objects/infos/PlayerInfo.cc
  objects/infos/HumanPlayer.cc

  objects/gametypes/Gametype.cc

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
    objects/Camera.cc
    CameraManager.cc
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

