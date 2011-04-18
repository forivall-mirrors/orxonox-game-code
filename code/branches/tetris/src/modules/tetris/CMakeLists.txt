SET_SOURCE_FILES(PONG_SRC_FILES
COMPILATION_BEGIN PongCompilation.cc
  Pong.cc
  PongAI.cc
  PongBall.cc
  PongBat.cc
  PongBot.cc
  PongCenterpoint.cc
  PongScore.cc
COMPILATION_END
)

ORXONOX_ADD_LIBRARY(pong
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
  SOURCE_FILES ${PONG_SRC_FILES}
)
