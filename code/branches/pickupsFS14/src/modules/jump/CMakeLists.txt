SET_SOURCE_FILES(JUMP_SRC_FILES
BUILD_UNIT JumpBuildUnit.cc
  Jump.cc
  JumpPlatform.cc
  JumpPlatformStatic.cc
  JumpPlatformHMove.cc
  JumpPlatformVMove.cc
  JumpFigure.cc
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
  SOURCE_FILES ${JUMP_SRC_FILES}
)
