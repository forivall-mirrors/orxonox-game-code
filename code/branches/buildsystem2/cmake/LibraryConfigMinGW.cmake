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
  IF(EXISTS ${CMAKE_SOURCE_DIR}/dependencies/include)
    SET(DEPENDENCY_DIR "${CMAKE_SOURCE_DIR}/dependencies")
  ELSEIF(EXISTS ${CMAKE_SOURCE_DIR}/../dependencies/include)
    SET(DEPENDENCY_DIR "${CMAKE_SOURCE_DIR}/../dependencies/include")
  ELSE()
    MESSAGE(FATAL_ERROR "Could not find dependency directory for the MinGW libraries")
  ENDIF()

  # 64 bit system?
  IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(MINGW_MACHINE_POSTFIX x64)
  ELSE()
    SET(MINGW_MACHINE_POSTFIX x86)
  ENDIF()
  SET(MINGW_INCLUDE_DIR  ${DEPENDENCY_DIR}/include)
  SET(MINGW_LIBRARY_DIR  ${DEPENDENCY_DIR}/lib/mingw-${MINGW_MACHINE_POSTFIX})
  SET(MINGW_BINARY_DIR   ${DEPENDENCY_DIR}/bin/mingw-${MINGW_MACHINE_POSTFIX})
  SET(CMAKE_LIBRARY_PATH ${MINGW_LIBRARY_DIR} ${MINGW_BINARY_DIR})
  SET(ORXONOX_RUNTIME_LIBRARY_DIRECTORY ${MINGW_BINARY_DIR})

  # Note: When setting ENV${} variables, make sure to use quotes when
  #       having multiple directories.
  SET(ENV{ALUTDIR}               ${MINGW_INCLUDE_DIR}/freealut-1.1.0)
  SET(ENV{BOOST_ROOT}            ${MINGW_INCLUDE_DIR}/boost-1.34.1)
  SET(ENV{CEGUIDIR}              ${MINGW_INCLUDE_DIR}/cegui-0.6.1)
  IF($ENV{DXSDK_DIR} STREQUAL "")
    SET(ENV{DXSDK_DIR}           ${MINGW_INCLUDE_DIR}/directx-2007.aug)
  ENDIF()
  SET(ENV{ENETDIR}               ${MINGW_INCLUDE_DIR}/enet-1.1)
  SET(LINK_ENET_DYNAMIC FALSE)
  SET(ENV{LUA_DIR}               ${MINGW_INCLUDE_DIR}/lua-5.1.3)
  SET(ENV{OGGDIR}                ${MINGW_INCLUDE_DIR}/libogg-1.1.3)
  SET(ENV{VORBISDIR}             ${MINGW_INCLUDE_DIR}/libvorbis-1.2.0)
  SET(ENV{OGRE_HOME}             ${MINGW_INCLUDE_DIR}/ogre-1.4.9)
  SET(ENV{OGRE_PLUGIN_DIR}       ${MINGW_BINARY_DIR})
  SET(ENV{OPENALDIR}             ${MINGW_INCLUDE_DIR}/openal-1.1)
  LIST(APPEND CMAKE_INCLUDE_PATH ${MINGW_INCLUDE_DIR}/tcl-8.5.2/include)
  SET(TCL_LIBRARY                ${MINGW_BINARY_DIR}/tcl85.dll CACHE FILEPATH "")
  LIST(APPEND CMAKE_INCLUDE_PATH ${MINGW_INCLUDE_DIR}/zlib-1.2.3/include)
  SET(ZLIB_LIBRARY               ${MINGW_LIBRARY_DIR}/libzlib.a CACHE FILEPATH "")
  SET(LINK_ZLIB_DYNAMIC FALSE)

ENDIF(MINGW AND LIBRARY_USE_PACKAGE_IF_SUPPORTED)
