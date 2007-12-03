PROJECT(Orxonox)

SET( HUD_SRC_FILES
	hud_overlay.cc
	test_overlay.cc
)

LINK_DIRECTORIES(.)

ADD_LIBRARY(hud SHARED ${HUD_SRC_FILES})
SET_TARGET_PROPERTIES(hud PROPERTIES LINK_FLAGS "--no-undefined" )
