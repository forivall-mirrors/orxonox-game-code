SET_SOURCE_FILES(TOWERDEFENSE_SRC_FILES
  TowerDefense.cc
  Tower.cc
  TowerDefenseCenterpoint.cc
  TowerDefenseHUDController.cc
  TowerDefensePlayerStats.cc
  TowerTurret.cc
)

ORXONOX_ADD_LIBRARY(towerdefense
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
  SOURCE_FILES ${TOWERDEFENSE_SRC_FILES}
)
