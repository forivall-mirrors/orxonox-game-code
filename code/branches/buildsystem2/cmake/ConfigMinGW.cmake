 #
 #   ORXONOX - the hottest 3D action shooter ever to exist
 #                    > www.orxonox.net <
 #
 #
 #   License notice:
 #
 #   This program is free software; you can redistribute it and/or
 #   modify it under the terms of the GNU General Public License
 #   as published by the Free Software Foundation; either version 2
 #   of the License, or (at your option) any later version.
 #
 #   This program is distributed in the hope that it will be useful,
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #   GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License
 #   along with this program; if not, write to the Free Software
 #   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #
 #   Author:
 #      ...
 #   Co-authors:
 #      ...
 #
 
################# MinGW config ####################
# Set the library directory when using precompiled
# tarballs for the dependencies under windows
###################################################

IF (MINGW)
  MESSAGE(STATUS "Running on MinGW. Using customized paths.")
  SET(MINGW_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/../libs")
  MARK_AS_ADVANCED(MING_LIBRARY_DIR)

  #SET(Boost_ADDITIONAL_VERSIONS "1.36")
  SET(BOOST_INCLUDEDIR ${MINGW_LIBRARY_DIR}/boost_1_34_1)
  SET(BOOST_LIBRARYDIR ${MINGW_LIBRARY_DIR}/boost_1_34_1/stage/lib)
  SET(ENV{CEGUIDIR}    ${MINGW_LIBRARY_DIR}/cegui-0.6.1)
  SET(ENV{ENETDIR}     ${MINGW_LIBRARY_DIR}/enet-1.1)
  # ENet is linked statically, hence we need to add some windows dependencies
  SET(ENet_ADDITIONAL_LIBRARIES ws2_32 winmm)
  SET(ENV{ALUTDIR}     ${MINGW_LIBRARY_DIR}/freealut-1.1.0)
  SET(ENV{OGGSDIR}     ${MINGW_LIBRARY_DIR}/libogg-1.1.3
                       ${MINGW_LIBRARY_DIR}/libogg-1.1.3/src/.libs)
  SET(ENV{VORBISDIR}   ${MINGW_LIBRARY_DIR}/libogg-1.1.3
                       ${MINGW_LIBRARY_DIR}/libvorbis-1.2.0/src/.libs)
  SET(ENV{OPENALDIR}   ${MINGW_LIBRARY_DIR}/openal-0.0.8/common
                       ${MINGW_LIBRARY_DIR}/openal-0.0.8/src/.libs)
  SET(ENV{LUA_DIR}     ${MINGW_LIBRARY_DIR}/lua-5.1.3)
  SET(ENV{OGRE_HOME}   ${MINGW_LIBRARY_DIR}/ogre/OgreMain
                       ${MINGW_LIBRARY_DIR}/ogre/Samples/Common/bin/Release)
  SET(TCL_INCLUDE_PATH ${MINGW_LIBRARY_DIR}/tcl8.5.2/generic)
  SET(TCL_LIBRARY_PATH ${MINGW_LIBRARY_DIR}/tcl8.5.2/win/tcl85.dll)
  SET(ZLIB_INCLUDE_DIR ${MINGW_LIBRARY_DIR}/zlib/include)
  SET(ZLIB_LIBRARY     ${MINGW_LIBRARY_DIR}/zlib/libzlib.a)
  IF (NOT ENV{DXSDK_DIR})
    SET(ENV{DXSDK_DIR} ${MINGW_LIBRARY_DIR}/DXSDK)
  ENDIF (NOT ENV{DXSDK_DIR})

  # MINGW doesn't like the -fPIC flag very much
  SET(ORXONOX_FPIC_FLAG "")
ENDIF (MINGW)

#MARK_AS_ADVANCED(MINGW_LIBRARY_DIR)
