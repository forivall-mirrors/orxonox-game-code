PROJECT(Orxonox)

SET( HUD_SRC_FILES
  hud_overlay.cc
  test_overlay.cc
)

IF(WIN32)
  ADD_LIBRARY(hud ${HUD_SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(hud SHARED ${HUD_SRC_FILES})
ENDIF(WIN32)
