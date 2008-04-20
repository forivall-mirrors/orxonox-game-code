AUX_SOURCE_DIRECTORY(tinyxml TINYXML_SRC_FILES)

SET(UTIL_SRC_FILES
  ArgReader.cc
  Math.cc
  String.cc
  Clipboard.cc
  SubString.cc
  MultiTypePrimitive.cc
  MultiTypeString.cc
  MultiTypeMath.cc
  ${TINYXML_SRC_FILES}
)

ADD_LIBRARY(util SHARED ${UTIL_SRC_FILES})

IF(TESTING_ENABLED)
  ADD_SUBDIRECTORY(testing)
ENDIF(TESTING_ENABLED)

TARGET_LINK_LIBRARIES(util
  ${OGRE_LIBRARIES}
) 

