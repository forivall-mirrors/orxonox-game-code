PROJECT(Orxonox)

# create a few variables to simplify life
SET( WEAPON_SRC
	ammunition_dump.cc
	barrel_gun.cc
	base_weapon.cc
	bullet.cc
	bullet_manager.cc
	weapon_station.cc
)

INCLUDE_DIRECTORIES(..)

ADD_LIBRARY(weapon ${WEAPON_SRC})
