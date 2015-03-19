SET_SOURCE_FILES(TOWERDEFENSE_SRC_FILES
  TowerDefense.cc
  TowerDefenseTower.cc
  TowerDefenseCenterpoint.cc
  TowerDefenseHUDController.cc
  TowerDefensePlayerStats.cc
  TDCoordinate.cc
  TowerDefenseEnemy.cc

)

ORXONOX_ADD_LIBRARY(towerdefense
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
    objects
  SOURCE_FILES ${TOWERDEFENSE_SRC_FILES}
)
