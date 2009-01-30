 #
 #             ORXONOX - the hottest 3D action shooter ever to exist
 #                             > www.orxonox.net <
 #
 #        This program is free software; you can redistribute it and/or
 #         modify it under the terms of the GNU General Public License
 #        as published by the Free Software Foundation; either version 2
 #            of the License, or (at your option) any later version.
 #
 #       This program is distributed in the hope that it will be useful,
 #        but WITHOUT ANY WARRANTY; without even the implied warranty of
 #        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #                 GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License along
 #      with this program; if not, write to the Free Software Foundation,
 #     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #
 #
 #  Author:
 #    Reto Grieder
 #  Description:
 #    Sets the library directories when using precompiled dependency archives
 #    on Windows with MinGW.
 #
 

IF(MINGW AND LIBRARY_USE_PACKAGE_IF_SUPPORTED)
  MESSAGE(STATUS "Running on MinGW. Using customized paths and options.")

  # Determine library directory
  IF(EXISTS ${CMAKE_SOURCE_DIR}/libs)
    SET(MINGW_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/libs")
  ELSEIF(EXISTS ${CMAKE_SOURCE_DIR}/../lis)
    SET(MINGW_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/../libs")
  ELSE()
    MESSAGE(FATAL_ERROR "Could not find dependency directory for the MinGW libraries")
  ENDIF()
  SET(ORXONOX_RUNTIME_LIBRARY_DIRECTORY ${MINGW_LIBRARY_DIR})

  # Note: When setting ENV${} variables, make sure to use quotes when
  #       having multiple directories.
  # Using BOOST_ROOT only does not work with CMake 2.6.0 (2.6.2 works though)
  SET(BOOST_INCLUDEDIR      "${MINGW_LIBRARY_DIR}/boost_1_34_1")
  SET(BOOST_LIBRARYDIR      "${MINGW_LIBRARY_DIR}/boost_1_34_1/stage/lib")
  SET(ENV{CEGUIDIR}         "${MINGW_LIBRARY_DIR}/cegui-0.6.1")
  SET(ENV{ENETDIR}          "${MINGW_LIBRARY_DIR}/enet-1.1")
  SET(LINK_ENET_DYNAMIC     FALSE)
  SET(ENV{ALUTDIR}          "${MINGW_LIBRARY_DIR}/freealut-1.1.0")
  SET(ENV{OGGDIR}           "${MINGW_LIBRARY_DIR}/libogg-1.1.3;${MINGW_LIBRARY_DIR}/libogg-1.1.3/src/.libs")
  SET(ENV{VORBISDIR}        "${MINGW_LIBRARY_DIR}/libvorbis-1.2.0;${MINGW_LIBRARY_DIR}/libvorbis-1.2.0/lib/.libs")
  SET(ENV{OPENALDIR}        "${MINGW_LIBRARY_DIR}/openal-0.0.8/common;${MINGW_LIBRARY_DIR}/openal-0.0.8/src/.libs")
  SET(ENV{LUA_DIR}          "${MINGW_LIBRARY_DIR}/lua-5.1.3")
  SET(ENV{OGRE_HOME}        "${MINGW_LIBRARY_DIR}/ogre/OgreMain;${MINGW_LIBRARY_DIR}")
  SET(ENV{OGRE_PLUGIN_DIR}  "${MINGW_LIBRARY_DIR}/ogre/Samples/Common")
  SET(TCL_INCLUDE_PATH      "${MINGW_LIBRARY_DIR}/tcl8.5.2/generic")
  SET(TCL_LIBRARY           "${MINGW_LIBRARY_DIR}/tcl8.5.2/win/tcl85.dll")
  SET(ZLIB_INCLUDE_DIR      "${MINGW_LIBRARY_DIR}/zlib/include")
  SET(ZLIB_LIBRARY          "${MINGW_LIBRARY_DIR}/zlib/libzlib.a")
  SET(LINK_ZLIB_DYNAMIC    FALSE)
  IF($ENV{DXSDK_DIR} STREQUAL "")
    SET(ENV{DXSDK_DIR}      "${MINGW_LIBRARY_DIR}/DXSDK")
  ENDIF()
ENDIF(MINGW AND LIBRARY_USE_PACKAGE_IF_SUPPORTED)
