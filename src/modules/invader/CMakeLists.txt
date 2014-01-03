SET_SOURCE_FILES(Invader_SRC_FILES
BUILD_UNIT InvaderBuildUnit.cc
  Invader.cc
  InvaderCenterPoint.cc
  InvaderShip.cc
  InvaderEnemy.cc
  InvaderEnemyShooter.cc
  InvaderWeapon.cc
  InvaderWeaponEnemy.cc
  InvaderHUDinfo.cc
END_BUILD_UNIT
)

ORXONOX_ADD_LIBRARY(invader
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
    weapons
  SOURCE_FILES ${Invader_SRC_FILES}
)
