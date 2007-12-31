PROJECT(Orxonox)

PROJECT(Orxonox)

INCLUDE_DIRECTORIES(tinyxml)

SET (SRC_FILES
  tinyxml/tinystr.cc
  tinyxml/tinyxml.cc
  tinyxml/tinyxmlerror.cc
  tinyxml/tinyxmlparser.cc
  tinyxml/ticpp.cc
  substring.cc
)

IF(WIN32)
  ADD_LIBRARY(util ${SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(util SHARED ${SRC_FILES})
ENDIF(WIN32)



IF(TESTING_ENABLED)
ADD_SUBDIRECTORY(testing)
ENDIF(TESTING_ENABLED)
