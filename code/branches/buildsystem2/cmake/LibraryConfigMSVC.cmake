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
 #    on Windows with Microsoft Visual Studio.
 #
 
###################### MSVC config ########################
# Set the library directories when using Visual Studio.   #

IF(MSVC AND LIBRARY_USE_PACKAGE_IF_SUPPORTED)
  MESSAGE(STATUS "Running on MSVC. Using customized paths and options.")

  # Determine library directory
  IF(EXISTS ${CMAKE_SOURCE_DIR}/dependencies/include)
    SET(DEPENDENCY_DIR "${CMAKE_SOURCE_DIR}/dependencies")
  ELSEIF(EXISTS ${CMAKE_SOURCE_DIR}/../dependencies/include)
    SET(DEPENDENCY_DIR "${CMAKE_SOURCE_DIR}/../dependencies/include")
  ELSEIF(EXISTS ${CMAKE_SOURCE_DIR}/../lib_dist/dependencies/include)
    SET(DEPENDENCY_DIR "${CMAKE_SOURCE_DIR}/../lib_dist/dependencies/include")
  ELSE()
    MESSAGE(FATAL_ERROR "Could not find dependency directory for the Visual Studio libraries")
  ENDIF()

  # 64 bit system?
  IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(MSVC_MACHINE_POSTFIX x64)
  ELSE()
    SET(MSVC_MACHINE_POSTFIX x86)
  ENDIF()
  SET(MSVC_INCLUDE_DIR   ${DEPENDENCY_DIR}/include)
  SET(MSVC_LIBRARY_DIR   ${DEPENDENCY_DIR}/lib/msvc8-${MSVC_MACHINE_POSTFIX})
  SET(MSVC_BINARY_DIR    ${DEPENDENCY_DIR}/bin/msvc8-${MSVC_MACHINE_POSTFIX})
  SET(CMAKE_LIBRARY_PATH ${MSVC_LIBRARY_DIR})
  SET(ORXONOX_RUNTIME_LIBRARY_DIRECTORY ${MSVC_BINARY_DIR})

  # Set variables for the include directories and the libraries
  # Note: When setting ENV${} variables, make sure to use quotes when
  #       having multiple directories.
  SET(ENV{ALUTDIR}               ${MSVC_INCLUDE_DIR}/freealut-1.1.0)
  SET(ENV{BOOST_ROOT}            ${MSVC_INCLUDE_DIR}/boost-1.35.0)
  SET(ENV{CEGUIDIR}              ${MSVC_INCLUDE_DIR}/cegui-0.6.1)
  IF($ENV{DXSDK_DIR} STREQUAL "")
    SET(ENV{DXSDK_DIR}           ${MSVC_INCLUDE_DIR}/directx-2007.aug)
  ENDIF()
  SET(ENV{ENETDIR}               ${MSVC_INCLUDE_DIR}/enet-1.2)
  SET(ENV{LUA_DIR}               ${MSVC_INCLUDE_DIR}/lua-5.1.3)
  SET(ENV{OGGDIR}                ${MSVC_INCLUDE_DIR}/libogg-1.1.3)
  SET(ENV{OGRE_HOME}             ${MSVC_INCLUDE_DIR}/ogre-1.4.9)
  SET(ENV{OGRE_PLUGIN_DIR}       ${MSVC_BINARY_DIR})
  SET(ENV{OPENALDIR}             ${MSVC_INCLUDE_DIR}/openal-1.1)
  LIST(APPEND CMAKE_INCLUDE_PATH ${MSVC_INCLUDE_DIR}/tcl-8.5.2/include)
  SET(ENV{VORBISDIR}             ${MSVC_INCLUDE_DIR}/libvorbis-1.2.0)
  LIST(APPEND CMAKE_INCLUDE_PATH ${MSVC_INCLUDE_DIR}/zlib-1.2.3/include)
  SET(ZLIB_LIBRARY     optimized ${MSVC_LIBRARY_DIR}/zlib.lib
                       debug     ${MSVC_LIBRARY_DIR}/zlib_d.lib CACHE STRING "")

ENDIF(MSVC AND LIBRARY_USE_PACKAGE_IF_SUPPORTED)
