SET( TINYXML_SRC_FILES
  ticpp.cc
  tinystr.cc
  tinyxml.cc
  tinyxmlerror.cc
  tinyxmlparser.cc
)

ADD_LIBRARY(tinyxml_orxonox SHARED ${TINYXML_SRC_FILES})
