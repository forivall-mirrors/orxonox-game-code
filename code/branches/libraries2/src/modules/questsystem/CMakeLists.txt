SET_SOURCE_FILES(QUEST_SRC_FILES
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
  QuestHint.cc
  QuestItem.cc
  QuestListener.cc
  QuestManager.cc
  QuestNotification.cc
)

ADD_SUBDIRECTORY(notifications)

# add the parent directory for tolua (TODO: remove this if the quest module is moved somewhere else)
INCLUDE_DIRECTORIES(..)

ORXONOX_ADD_LIBRARY(quest
  MODULE
  TOLUA_FILES
    QuestDescription.h
    QuestManager.h
  DEFINE_SYMBOL
    "QUEST_SHARED_BUILD"
  LINK_LIBRARIES
    orxonox
    overlays
  SOURCE_FILES ${QUEST_SRC_FILES}
)
