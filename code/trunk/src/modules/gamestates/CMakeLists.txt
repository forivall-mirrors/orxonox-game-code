SET_SOURCE_FILES(GAMESTATES_SRC_FILES
  GSClient.cc
  GSDedicated.cc
  GSGraphics.cc
  GSIOConsole.cc
  GSLevel.cc
  GSMainMenu.cc
  GSRoot.cc
  GSServer.cc
  GSStandalone.cc
)

ORXONOX_ADD_LIBRARY(gamestates
  MODULE
  FIND_HEADER_FILES
  PCH_FILE
    GameStatesPrecompiledHeaders.h
  DEFINE_SYMBOL
    "GAMESTATES_SHARED_BUILD"
  LINK_LIBRARIES
    orxonox
  SOURCE_FILES ${GAMESTATES_SRC_FILES}
)
