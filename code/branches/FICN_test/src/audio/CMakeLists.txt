PROJECT(Orxonox)

SET(SRC_FILES
  AudioIncludes.h
  AudioManager.cc
  AudioBuffer.cc
  AudioSource.cc
  AudioStream.cc
)


INCLUDE_DIRECTORIES(..)

IF(WIN32)
  ADD_LIBRARY(audio ${SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(audio SHARED ${SRC_FILES})
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(
  audio
  ${OPENAL_LIBRARY}
  ${ALUT_LIBRARY}
  ${VORBISFILE_LIBRARY}
  ${VORBISENC_LIBRARY}
  ${VORBIS_LIBRARY}
  ${OGG_LIBRARY}
)


