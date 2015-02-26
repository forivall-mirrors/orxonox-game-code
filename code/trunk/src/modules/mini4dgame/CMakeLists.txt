SET_SOURCE_FILES(Mini4Dgame_SRC_FILES
  Mini4Dgame.cc
  #Mini4DgameAI.cc
  Mini4DgameBoard.cc
)

ORXONOX_ADD_LIBRARY(mini4dgame
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
  SOURCE_FILES ${Mini4Dgame_SRC_FILES}
)
