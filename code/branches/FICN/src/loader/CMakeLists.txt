PROJECT(Orxonox)

# TODO find a cleaner way to include xmlParser
SET( LOADER_SRC_FILES
  LevelLoader.cc
)

IF(WIN32)
  ADD_LIBRARY(loader ${LOADER_SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(loader SHARED ${LOADER_SRC_FILES})
ENDIF(WIN32)

TARGET_LINK_LIBRARIES( loader
  util
)