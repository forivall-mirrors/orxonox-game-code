PROJECT(Orxonox)

SET(SRC_FILES 
	AudioManager.cc
	AudioBuffer.cc
	AudioSource.cc
)
SET(INC_FILES 
	AudioManager.h
	AudioBuffer.h
	AudioSource.h
)


ADD_LIBRARY(audio ${SRC_FILES} ${INC_FILES})

TARGET_LINK_LIBRARIES(audio ${OPENAL_LIBRARY} ${ALUT_LIBRARY})


