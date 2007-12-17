PROJECT(Orxonox)

SET(SRC_FILES
  ParticleInterface.cc
)

IF(WIN32)
  ADD_LIBRARY(particle ${SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(particle SHARED ${SRC_FILES})
ENDIF(WIN32)
