PROJECT(Orxonox)

# TODO find a cleaner way to include xmlParser
SET( LOADER_SRC_FILES
  LevelLoader.cc
)

ADD_LIBRARY(loader SHARED ${LOADER_SRC_FILES})

TARGET_LINK_LIBRARIES( loader
  xml
)