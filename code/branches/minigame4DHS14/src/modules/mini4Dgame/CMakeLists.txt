SET_SOURCE_FILES(Mini4Dgame_SRC_FILES
BUILD_UNIT mini4DgameBuildUnit.cc
  Mini4Dgame.cc
  Mini4DgameCenterpoint.cc
END_BUILD_UNIT
)

ORXONOX_ADD_LIBRARY(mini4Dgame
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
  SOURCE_FILES ${Mini4Dgame_SRC_FILES}
)
