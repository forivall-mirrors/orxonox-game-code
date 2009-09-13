SET_SOURCE_FILES(QUESTSYSTEM_SRC_FILES
  AddQuest.cc
  AddQuestHint.cc
  AddReward.cc
  ChangeQuestStatus.cc
  CompleteQuest.cc
  FailQuest.cc
  GlobalQuest.cc
  LocalQuest.cc
  Quest.cc
  QuestDescription.cc
  QuestEffect.cc
  QuestEffectBeacon.cc
  QuestGUINode.cc
  QuestGUI.cc
  QuestHint.cc
  QuestItem.cc
  QuestListener.cc
  QuestManager.cc
  QuestNotification.cc
)

ADD_SUBDIRECTORY(notifications)

ORXONOX_ADD_LIBRARY(questsystem
  MODULE
  TOLUA_FILES
    QuestDescription.h
    QuestManager.h
  DEFINE_SYMBOL
    "QUESTSYSTEM_SHARED_BUILD"
  LINK_LIBRARIES
    orxonox
    overlays
  SOURCE_FILES ${QUESTSYSTEM_SRC_FILES}
)
