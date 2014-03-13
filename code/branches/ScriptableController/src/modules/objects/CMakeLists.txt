SET_SOURCE_FILES(OBJECTS_SRC_FILES
  Attacher.cc
  ForceField.cc
  Planet.cc
  Script.cc
  SpaceBoundaries.cc
  Turret.cc
)

ADD_SUBDIRECTORY(collisionshapes)
ADD_SUBDIRECTORY(eventsystem)
ADD_SUBDIRECTORY(triggers)

ORXONOX_ADD_LIBRARY(objects
  MODULE
  FIND_HEADER_FILES
  PCH_FILE
    ObjectsPrecompiledHeaders.h
  LINK_LIBRARIES
    orxonox
  SOURCE_FILES ${OBJECTS_SRC_FILES}
)
