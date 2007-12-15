PROJECT(Orxonox)

SET(SRC_FILES
	AudioIncludes.h
	AudioManager.cc
	AudioBuffer.cc
	AudioSource.cc
	AudioStream.cc
)


INCLUDE_DIRECTORIES(..)

ADD_LIBRARY(audio ${SRC_FILES})

TARGET_LINK_LIBRARIES(audio ${OPENAL_LIBRARY} ${ALUT_LIBRARY} ${VORBISFILE_LIBRARY} ${VORBISENC_LIBRARY} ${VORBIS_LIBRARY} ${OGG_LIBRARY})


