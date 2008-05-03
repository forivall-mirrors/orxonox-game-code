SET ( AUDIO_SRC_FILES
  AudioBuffer.cc
  AudioManager.cc
  AudioSource.cc
  AudioStream.cc
)

IF (WIN32)
  ADD_LIBRARY( audio ${AUDIO_SRC_FILES} )
ELSE (WIN32)
  ADD_LIBRARY( audio SHARED ${AUDIO_SRC_FILES} )
ENDIF (WIN32)

TARGET_LINK_LIBRARIES( audio
  ${OPENAL_LIBRARY}
  ${ALUT_LIBRARY}
  ${VORBISFILE_LIBRARY}
  ${VORBIS_LIBRARY}
  ${OGG_LIBRARY}
  core
)


