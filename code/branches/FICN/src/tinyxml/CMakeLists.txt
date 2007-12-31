PROJECT(Orxonox)

INCLUDE_DIRECTORIES(.)

SET (SRC_FILES
  tinystr.cc
  tinyxml.cc
  tinyxmlerror.cc
  tinyxmlparser.cc
  ticpp.cc
)

IF(WIN32)
  ADD_LIBRARY(xml ${SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(xml SHARED ${SRC_FILES})
ENDIF(WIN32)

