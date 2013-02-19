SET_SOURCE_FILES(GAMETYPES_SRC_FILES
  SpaceRace.cc
  RaceCheckPoint.cc
  SpaceRaceManager.cc
  OldSpaceRace.cc
  OldRaceCheckPoint.cc
  SpaceRaceBot.cc
  SpaceRaceController.cc
)

ORXONOX_ADD_LIBRARY(gametypes
  MODULE
  FIND_HEADER_FILES
  TOLUA_FILES
  PCH_FILE
    GametypesPrecompiledHeaders.h
  LINK_LIBRARIES
    orxonox
    objects
  SOURCE_FILES ${GAMETYPES_SRC_FILES}
)
