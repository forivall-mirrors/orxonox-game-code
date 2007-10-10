PROJECT (Tutorial)

SET (MAIN_SRC main.cpp ExampleApplication.h ExampleFrameListener.h ExampleLoadingBar.h )

ADD_EXECUTABLE (main ${MAIN_SRC})

# find OGRE
FIND_LIBRARY (OGRE_LIBS NAMES OGRE
  PATH /usr/lib /usr/local/lib /usr/pack/ogre-1.4.5-sd/i686-debian-linux3.1/lib/pkgconfig)
IF (OGRE_LIBS)
  TARGET_ADD_LIBRARY (main OGRE_LIBS)
ENDIF (OGRE_LIBS)

# find OIS
FIND_LIBRARY (OIS_LIBS NAMES OIS
  PATH /usr/lib /usr/local/lib /usr/pack/ois-1.0-sd/i686-debian-linux3.1/lib/pkgconfig)
IF (OIS_LIBS)
  ADD_LIBRARY (OIS_LIBS)
ENDIF (OIS_LIBS)

INCLUDE_DIRECTORIES (main . /usr/pack/ois-1.0-sd/i686-debian-linux3.1/include /usr/pack/ogre-1.4.5-sd/i686-debian-linux3.1/include)
