ADD_SOURCE_FILES(ORXONOX_SRC_FILES
  Munition.cc
  Weapon.cc
  WeaponMode.cc
  WeaponPack.cc
  WeaponSet.cc
  WeaponSlot.cc
  WeaponSystem.cc
  DefaultWeaponmodeLink.cc
  MuzzleFlash.cc
)

SET_SOURCE_FILES(WEAPONSYSTEM_SRC_FILES
  MuzzleFlash.cc
)

ADD_SUBDIRECTORY(munitions)
ADD_SUBDIRECTORY(projectiles)
ADD_SUBDIRECTORY(weaponmodes)

ORXONOX_ADD_LIBRARY(weaponsystem
  MODULE
  LINK_LIBRARIES
    orxonox
  SOURCE_FILES ${WEAPONSYSTEM_SRC_FILES}
)
