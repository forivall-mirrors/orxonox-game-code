PROJECT(Orxonox)

SET(SRC_FILES 
	Ambient.cc
	AudioObject.cc
)
SET(INC_FILES 
	Ambient.h
	AudioObject.h
)


ADD_LIBRARY(audio ${SRC_FILES} ${INC_FILES})

TARGET_LINK_LIBRARIES(audio alut)
TARGET_LINK_LIBRARIES(audio openal)


