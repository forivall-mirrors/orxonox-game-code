SET_SOURCE_FILES(WEAPONS_SRC_FILES
  MuzzleFlash.cc
)

ADD_SUBDIRECTORY(munitions)
ADD_SUBDIRECTORY(projectiles)
ADD_SUBDIRECTORY(weaponmodes)

ORXONOX_ADD_LIBRARY(weapons
  MODULE
  FIND_HEADER_FILES
  PCH_FILE
    WeaponsPrecompiledHeaders.h
  DEFINE_SYMBOL
    "WEAPONS_SHARED_BUILD"
  LINK_LIBRARIES
    orxonox
    # TODO: Remove this as soon as projectiles aren't hardcoded anymore buth rather defined in XML
    objects
  SOURCE_FILES ${WEAPONS_SRC_FILES}
)
