SET_SOURCE_FILES(JUMP_SRC_FILES
BUILD_UNIT JumpBuildUnit.cc
  Jump.cc
  JumpPlatform.cc
  JumpPlatformStatic.cc
  JumpPlatformHMove.cc
  JumpPlatformVMove.cc
  JumpPlatformDisappear.cc
  JumpPlatformTimer.cc
  JumpPlatformFake.cc
  JumpProjectile.cc
  JumpEnemy.cc
  JumpFigure.cc
  JumpItem.cc
  JumpSpring.cc
  JumpRocket.cc
  JumpShield.cc
  JumpPropeller.cc
  JumpBoots.cc
  JumpCenterpoint.cc
  JumpScore.cc
END_BUILD_UNIT
)

ORXONOX_ADD_LIBRARY(jump
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
    weapons
  SOURCE_FILES ${JUMP_SRC_FILES}
)
