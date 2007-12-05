
SET(OGGVORBIS_LIBRARY_DIR "/usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.0/lib")
SET(OGGVORBIS_LIBRARY "${OGGVORBIS_LIBRARY_DIR}/libvorbisenc.so ${OGGVORBIS_LIBRARY_DIR}/libvorbisfile.so ${OGGVORBIS_LIBRARY_DIR}/libogg.so ${OGGVORBIS_LIBRARY_DIR}/libvorbis.so ")
SET(OGGVORBIS_INCLUDE_DIR "/usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.0/include")

MESSAGE(STATUS "OggVorbis was found. Libdir ${OGGVORBIS_LIBRARY_DIR} Includedir ${OGGVORBIS_INCLUDE_DIR}")
