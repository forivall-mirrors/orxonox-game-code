PROJECT(Orxonox)

# TODO find a cleaner way to include xmlParser
SET( LOADER_SRC_FILES
	LevelLoader.cc
	../xml/xmlParser.cc
)

ADD_LIBRARY(loader ${LOADER_SRC_FILES})