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
 
################# Win check ##################
# Set the library directory when using precompiled tarballs for
# the dependencies under windows
###########################################

IF (MINGW)
  MESSAGE(STATUS "Running on MinGW. Using customized paths.")
  SET(MINGW_LIBRARY_DIR "../libs")
  #SET(Boost_ADDITIONAL_VERSIONS "1.36")
  SET(BOOST_INCLUDEDIR ".${MINGW_LIBRARY_DIR}/boost_1_34_1")
  SET(BOOST_LIBRARYDIR ".${MINGW_LIBRARY_DIR}/boost_1_34_1/stage/lib")
  SET(ENV{LUA_DIR} ".${MINGW_LIBRARY_DIR}/enet-1.1")
  SET(ENV{ALUTDIR} ".${MINGW_LIBRARY_DIR}/freealut-1.1.0")
  SET(ENV{OPENALDIR} ".${MINGW_LIBRARY_DIR}/openal-0.0.8/common")
  SET(ENV{ENETDIR} ".${MINGW_LIBRARY_DIR}/lua-5.1.3")
  SET(ENV{OGGVORBISDIR} ".${MINGW_LIBRARY_DIR}/libogg-1.1.3")
  #SET(TCL_INCLUDE_PATH "/usr/pack/tcltk-8.4.9.linux-mo/include")
  SET(ENV{CEGUIDIR} ".${MINGW_LIBRARY_DIR}/cegui-0.6.1")
  SET(ENV{OGREDIR} ".${MINGW_LIBRARY_DIR}/ogre/OgreMain;.${MINGW_LIBRARY_DIR}/ogre/Samples/Common/bin/Release")
ENDIF (MINGW)