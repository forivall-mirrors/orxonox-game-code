PROJECT(Orxonox)

# TODO find a cleaner way to include xmlParser
SET(SRC_FILES LevelLoader.cc ../xml/xmlParser.cc)
SET(INC_FILES LevelLoader.h ../xml/xmlParser.h)

ADD_LIBRARY(core ${SRC_FILES} ${INC_FILES})