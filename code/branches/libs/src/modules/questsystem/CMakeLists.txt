SET_SOURCE_FILES(QUESTSYSTEM_SRC_FILES
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
)

ADD_SUBDIRECTORY(effects)

ORXONOX_ADD_LIBRARY(questsystem
  MODULE
  FIND_HEADER_FILES
  TOLUA_FILES
    QuestManager.h
    QuestDescription.h
    Quest.h
    QuestHint.h
  PCH_FILE
    QuestsystemPrecompiledHeaders.h
  LINK_LIBRARIES
    orxonox
    notifications
    objects
  SOURCE_FILES ${QUESTSYSTEM_SRC_FILES}
)
