SET_SOURCE_FILES(Mini4Dgame_SRC_FILES
  Mini4Dgame.cc
  #Mini4DgameAI.cc
  Mini4DgameBoard.cc
)

ORXONOX_ADD_LIBRARY(mini4Dgame
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
  SOURCE_FILES ${Mini4Dgame_SRC_FILES}
)
