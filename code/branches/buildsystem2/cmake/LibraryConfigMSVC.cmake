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
 #      Reto Grieder
 #   Co-authors:
 #      ...
 #
 
###################### MSVC config ########################
# Set the library directories when using Visual Studio.   #

IF (MSVC AND LIBRARY_USE_PACKAGE_IF_SUPPORTED)
  MESSAGE(STATUS "Running on MSVC. Using customized paths and options.")

  # Determine library directory
  IF(EXISTS ${CMAKE_SOURCE_DIR}/dependencies/orxonox_vc8)
    SET(MSVC_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/dependencies/orxonox_vc8")
  ELSEIF(EXISTS ${CMAKE_SOURCE_DIR}/../lib_dist/vc8/dependencies/orxonox_vc8)
    SET(MSVC_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/../lib_dist/vc8/dependencies/orxonox_vc8")
  ELSE(EXISTS ${CMAKE_SOURCE_DIR}/dependencies/orxonox_vc8)
    MESSAGE(FATAL_ERROR "Could not find dependency directory for the Visual Studio libraries")
  ENDIF(EXISTS ${CMAKE_SOURCE_DIR}/dependencies/orxonox_vc8)
  SET(ORXONOX_LIBRARY_BIN_DIR ${MSVC_LIBRARY_DIR}/bin)

  # Set variables for the include directories and the libraries
  # Note: When setting ENV${} variables, make sure to use quotes when
  #       having multiple directories.
  SET(BOOST_ROOT             ${MSVC_LIBRARY_DIR}/boost-1.35.0)
  SET(BOOST_LIBRARYDIR       ${MSVC_LIBRARY_DIR}/boost-1.35.0/lib)
  SET(ENV{CEGUIDIR}          ${MSVC_LIBRARY_DIR}/cegui-0.6.1)
  SET(ENV{ENETDIR}           ${MSVC_LIBRARY_DIR}/enet-1.2)
  SET(ENV{ALUTDIR}           ${MSVC_LIBRARY_DIR}/freealut-1.1.0)
  SET(ENV{OGGDIR}            ${MSVC_LIBRARY_DIR}/libogg-1.1.3)
  SET(ENV{VORBISDIR}         ${MSVC_LIBRARY_DIR}/libvorbis-1.2.0)
  SET(ENV{OPENALDIR}         ${MSVC_LIBRARY_DIR}/openal-1.1)
  SET(ENV{LUA_DIR}           ${MSVC_LIBRARY_DIR}/lua-5.1.3)
  SET(ENV{OGRE_HOME}         ${MSVC_LIBRARY_DIR}/ogre-1.4.9)
  SET(ENV{OGRE_PLUGIN_DIR}   ${MSVC_LIBRARY_DIR}/bin)
  SET(TCL_INCLUDE_PATH       ${MSVC_LIBRARY_DIR}/tcl-8.5.5/include)
  SET(TCL_LIBRARY            ${MSVC_LIBRARY_DIR}/tcl-8.5.5/lib/tcl85.lib)
  SET(ZLIB_INCLUDE_DIR       ${MSVC_LIBRARY_DIR}/zlib-1.2.3/include)
  SET(ZLIB_LIBRARY optimized ${MSVC_LIBRARY_DIR}/zlib-1.2.3/lib/zlib.lib
                   debug     ${MSVC_LIBRARY_DIR}/zlib-1.2.3/lib/zlib_d.lib)
  IF ($ENV{DXSDK_DIR} STREQUAL "")
    SET(ENV{DXSDK_DIR}       ${MSVC_LIBRARY_DIR}/directx-sdk-aug-2007)
  ENDIF ($ENV{DXSDK_DIR} STREQUAL "")

ENDIF (MSVC AND LIBRARY_USE_PACKAGE_IF_SUPPORTED)
