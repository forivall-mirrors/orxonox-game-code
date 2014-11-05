SET_SOURCE_FILES(Dodgerace2_SRC_FILES
DodgeRace.cc
DodgeRaceCenterPoint.cc
DodgeRaceHUDinfo.cc
)

ORXONOX_ADD_LIBRARY(dodgerace2
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
    weapons
  SOURCE_FILES ${Dodgerace2_SRC_FILES}
)
