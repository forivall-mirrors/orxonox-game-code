SET_SOURCE_FILES(TETRIS_SRC_FILES
  Tetris.cc
  TetrisCenterpoint.cc
  TetrisStone.cc
  TetrisBrick.cc
  TetrisScore.cc
)

ORXONOX_ADD_LIBRARY(tetris
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
  SOURCE_FILES ${TETRIS_SRC_FILES}
)
