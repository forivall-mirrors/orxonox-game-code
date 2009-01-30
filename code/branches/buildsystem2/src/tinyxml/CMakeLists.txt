SET(TINYXML++_FILES
  ticpp.h
  ticpprc.h
  tinystr.h
  tinyxml.h

  ticpp.cpp
  tinystr.cpp
  tinyxml.cpp
  tinyxmlerror.cpp
  tinyxmlparser.cpp
)
GENERATE_SOURCE_GROUPS(${TINYXML++_FILES})

IF(MSVC)
  ADD_LIBRARY(tinyxml++_orxonox STATIC ${TINYXML++_FILES})
ELSE()
  ADD_LIBRARY(tinyxml++_orxonox SHARED ${TINYXML++_FILES})
ENDIF()

IF(NOT WIN32)
  INSTALL(TARGETS tinyxml++_orxonox LIBRARY DESTINATION lib)
ENDIF()
