PROJECT(Orxonox)

SET(SRC_FILES
  BillboardSet.cc
  Light.cc
  Mesh.cc
)

IF(WIN32)
  ADD_LIBRARY(tools ${SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(tools SHARED ${SRC_FILES})
ENDIF(WIN32)
