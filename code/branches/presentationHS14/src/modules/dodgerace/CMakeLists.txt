SET_SOURCE_FILES(Dodgerace_SRC_FILES
DodgeRace.cc
DodgeRaceCenterPoint.cc
DodgeRaceShip.cc
DodgeRaceCube.cc
DodgeRaceHUDinfo.cc
)

ORXONOX_ADD_LIBRARY(dodgerace
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
    weapons
  SOURCE_FILES ${Dodgerace_SRC_FILES}
)
