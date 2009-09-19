SET_SOURCE_FILES(PONG_SRC_FILES
  Pong.cc
  PongAI.cc
  PongBall.cc
  PongBat.cc
  PongBot.cc
  PongCenterpoint.cc
  PongScore.cc
)

ORXONOX_ADD_LIBRARY(pong
  MODULE
  FIND_HEADER_FILES
  PCH_FILE
    PongPrecompiledHeaders.h
  PCH_NO_DEFAULT
  DEFINE_SYMBOL
    "PONG_SHARED_BUILD"
  LINK_LIBRARIES
    orxonox
    overlays
  SOURCE_FILES ${PONG_SRC_FILES}
)
