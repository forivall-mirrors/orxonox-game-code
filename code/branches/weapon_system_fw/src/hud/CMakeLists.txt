PROJECT(Orxonox)

# create a few variables to simplify life
SET( HUD_SRC
	hud_overlay.cc
	test_overlay.cc
   )

ADD_LIBRARY(HUD ${HUD_SRC} )
