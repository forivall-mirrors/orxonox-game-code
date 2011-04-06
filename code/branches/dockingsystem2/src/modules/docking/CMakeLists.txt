SET_SOURCE_FILES(DOCKING_SRC_FILES
  DockingEffect.cc
  DockToShip.cc
  Dock.cc
)

ORXONOX_ADD_LIBRARY(docking
  MODULE
  FIND_HEADER_FILES
  TOLUA_FILES
  PCH_FILE
    DockingPrecompiledHeaders.h
  LINK_LIBRARIES
    orxonox
  SOURCE_FILES ${DOCKING_SRC_FILES}
)
