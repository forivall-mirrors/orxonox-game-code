PROJECT(Orxonox)

SET( WEAPON_SRC_FILES
  ammunition_dump.cc
  barrel_gun.cc
  base_weapon.cc
  bullet.cc
  bullet_manager.cc
  weapon_station.cc
)

IF(WIN32)
  ADD_LIBRARY(weapon ${WEAPON_SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(weapon SHARED ${WEAPON_SRC_FILES})
ENDIF(WIN32)
