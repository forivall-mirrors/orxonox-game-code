SET (SRC_FILES
  AudioIncludes.h
  AudioManager.cc
  AudioBuffer.cc
  AudioSource.cc
  AudioStream.cc
)

ADD_LIBRARY( audio SHARED ${SRC_FILES})
SET_TARGET_PROPERTIES( audio PROPERTIES LINK_FLAGS "--no-undefined" )

TARGET_LINK_LIBRARIES( audio
  ${OPENAL_LIBRARY}
  ${ALUT_LIBRARY}
  ${VORBISFILE_LIBRARY}
  ${VORBIS_LIBRARY}
  ${OGG_LIBRARY}
  core
)


