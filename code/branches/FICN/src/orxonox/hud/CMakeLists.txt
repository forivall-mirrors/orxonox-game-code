PROJECT(Orxonox)

SET( HUD_SRC_FILES
	hud_overlay.cc
	test_overlay.cc
)

LINK_DIRECTORIES(.)

ADD_LIBRARY(hud ${HUD_SRC_FILES})