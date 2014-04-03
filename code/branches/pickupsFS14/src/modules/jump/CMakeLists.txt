SET_SOURCE_FILES(Jump_SRC_FILES
BUILD_UNIT JumpBuildUnit.cc
  Jump.cc
  JumpCenterPoint.cc
  JumpShip.cc
  JumpPlatform.cc
END_BUILD_UNIT
)

ORXONOX_ADD_LIBRARY(jump
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
    weapons
  SOURCE_FILES ${Jump_SRC_FILES}
)
