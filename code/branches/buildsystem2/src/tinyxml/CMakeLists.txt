SET( TINYXML_SRC_FILES
  ticpp.cpp
  tinystr.cpp
  tinyxml.cpp
  tinyxmlerror.cpp
  tinyxmlparser.cpp
)

IF(MSVC)
  ADD_LIBRARY(tinyxml_orxonox STATIC ${TINYXML_SRC_FILES})
ELSE(MSVC)
  ADD_LIBRARY(tinyxml_orxonox SHARED ${TINYXML_SRC_FILES})
ENDIF(MSVC)

IF (NOT WIN32)
  INSTALL(TARGETS tinyxml_orxonox LIBRARY DESTINATION lib)
ENDIF (NOT WIN32)
