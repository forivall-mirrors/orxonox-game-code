PROJECT(Orxonox)

# TODO find a cleaner way to include xmlParser
SET(SRC_FILES LevelLoader.cc ../xml/xmlParser.cc)
SET(INC_FILES LevelLoader.h ../xml/xmlParser.h)

INCLUDE_DIRECTORIES(..)

ADD_LIBRARY(loader ${SRC_FILES} ${INC_FILES})