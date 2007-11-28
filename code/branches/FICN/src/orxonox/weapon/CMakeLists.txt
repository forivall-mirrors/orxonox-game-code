PROJECT(Orxonox)

SET( WEAPON_SRC_FILES
	ammunition_dump.cc
	barrel_gun.cc
	base_weapon.cc
	bullet.cc
	bullet_manager.cc
	weapon_station.cc
)

ADD_LIBRARY(weapon ${WEAPON_SRC_FILES})
