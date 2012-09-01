SET_SOURCE_FILES(PONG_SRC_FILES
BUILD_UNIT PongBuildUnit.cc
  Pong.cc
  PongAI.cc
  PongBall.cc
  PongBat.cc
  PongBot.cc
  PongCenterpoint.cc
  PongScore.cc
END_BUILD_UNIT
)

ORXONOX_ADD_LIBRARY(pong
  MODULE
  FIND_HEADER_FILES
  LINK_LIBRARIES
    orxonox
    overlays
  SOURCE_FILES ${PONG_SRC_FILES}
)
