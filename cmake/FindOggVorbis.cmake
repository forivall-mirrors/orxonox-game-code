
#SET(OGGVORBIS_LIBRARY_DIR "/usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.0/lib")
#SET(OGGVORBIS_LIBRARY "-L ${OGGVORBIS_LIBRARY_DIR} -lvorbisenc -lvorbisfile -logg -lvorbis ")
#SET(OGGVORBIS_INCLUDE_DIR "/usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.0/include")

#
# Includes
#

FIND_PATH(OGG_INCLUDE_DIR ogg/ogg.h
  /usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.0/include 		# Tardis specific hack
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/csw/include
  /opt/include
  ../libs/libogg-1.1.3/include
  )

FIND_PATH(VORBIS_INCLUDE_DIR vorbis/codec.h
  /usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.0/include 		# Tardis specific hack
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/csw/include
  /opt/include
  ../libs/libvorbis-1.2.0/include
  )

#
# Libs
#

FIND_LIBRARY(OGG_LIBRARY
  NAMES ogg
  PATHS
  /usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.1/lib
  /usr/local/lib
  /usr/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  ../libs/libogg-1.1.3/src/.libs
  )

FIND_LIBRARY(VORBIS_LIBRARY
  NAMES vorbis
  PATHS
  /usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.1/lib
  /usr/local/lib
  /usr/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  ../libs/libvorbis-1.2.0/lib/.libs
  )

FIND_LIBRARY(VORBISENC_LIBRARY
  NAMES vorbisenc
  PATHS
  /usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.1/lib
  /usr/local/lib
  /usr/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  ../libs/libvorbis-1.2.0/lib/.libs
  )

FIND_LIBRARY(VORBISFILE_LIBRARY
  NAMES vorbisfile
  PATHS
  /usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.1/lib
  /usr/local/lib
  /usr/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  ../libs/libvorbis-1.2.0/lib/.libs
  )

SET(OGG_FOUND "NO")
IF(OGG_LIBRARY)
  SET(OGG_FOUND "YES")
	MESSAGE(STATUS "Ogg was found.")
ENDIF(OGG_LIBRARY)

SET(VORBIS_FOUND "NO")
IF(VORBIS_LIBRARY)
  SET(VORBIS_FOUND "YES")
	MESSAGE(STATUS "Vorbis was found.")
ENDIF(VORBIS_LIBRARY)

