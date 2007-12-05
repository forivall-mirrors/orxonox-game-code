PROJECT(Orxonox)

SET(SRC_FILES 
	AudioIncludes.h
	AudioManager.cc
	AudioBuffer.cc
	AudioSource.cc
	AudioStream.cc
)
SET(INC_FILES 
	AudioManager.h
	AudioBuffer.h
	AudioSource.h
	AudioStream.h
)


ADD_LIBRARY(audio ${SRC_FILES} ${INC_FILES})

TARGET_LINK_LIBRARIES(audio ${OPENAL_LIBRARY} ${ALUT_LIBRARY} ${OGG_LIBRARY} ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY} ${VORBISENC_LIBRARY})


