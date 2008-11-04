SET( SRC_FILES
  EventListener.cc
  EventDispatcher.cc
  EventTarget.cc
  Radar.cc
  RadarListener.cc
  RadarViewable.cc
  Tickable.cc
  Test.cc
  Script.cc

  Scene.cc
  worldentities/WorldEntity.cc
  worldentities/PositionableEntity.cc
  worldentities/MovableEntity.cc
  worldentities/ControllableEntity.cc
  worldentities/Model.cc
  worldentities/Billboard.cc
  worldentities/BlinkingBillboard.cc
  worldentities/Light.cc
  worldentities/Camera.cc
  worldentities/CameraPosition.cc
  worldentities/SpawnPoint.cc
  worldentities/ParticleEmitter.cc
  worldentities/ParticleSpawner.cc
#  worldentities/Backlight.cc

  quest/AddQuest.cc
  quest/AddQuestHint.cc
  quest/AddReward.cc
  quest/ChangeQuestStatus.cc
  quest/CompleteQuest.cc
  quest/FailQuest.cc
  quest/GlobalQuest.cc
  quest/LocalQuest.cc
  quest/Quest.cc
  quest/QuestDescription.cc
  quest/QuestEffect.cc
  quest/QuestHint.cc
  quest/QuestItem.cc
  quest/QuestManager.cc
  quest/Rewardable.cc

  weaponSystem/WeaponSystem.cc
  weaponSystem/WeaponSet.cc
  weaponSystem/WeaponSlot.cc
  weaponSystem/Weapon.cc
  weaponSystem/Munition.cc

  worldentities/triggers/Trigger.cc
  worldentities/triggers/DistanceTrigger.cc
  worldentities/triggers/EventTrigger.cc

  worldentities/pawns/Spectator.cc
  worldentities/pawns/Pawn.cc
  worldentities/pawns/SpaceShip.cc

  infos/Info.cc
  infos/Level.cc
  infos/PlayerInfo.cc
  infos/HumanPlayer.cc

  gametypes/Gametype.cc
)

ADD_SOURCE_DIRECTORY(SRC_FILES controllers)

ADD_SOURCE_FILES(SRC_FILES)

