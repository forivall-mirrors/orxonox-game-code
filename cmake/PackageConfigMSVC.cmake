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
 #  Note:
 #    When setting ENV${} variables, make sure to use quotes for lists
 #

IF(MSVC)

  # MAJOR: Interface breaking change somewhere or added a new library
  # MINOR: Updated a library to a new version
  # PATCH: Bug fix or smaller things
  SET(DEPENDENCY_VERSION 0.0.1)

  MESSAGE(STATUS "Using library package for the dependencies.")

  # 64 bit system?
  IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(BINARY_POSTFIX x64)
  ELSE()
    SET(BINARY_POSTFIX x86)
  ENDIF()

  # Choose right MSVC version
  STRING(REGEX REPLACE "^Visual Studio ([0-9][0-9]?) .*$" "\\1"
         _msvc_version "${CMAKE_GENERATOR}")
  IF(MSVC71)
    SET(_msvc_version "7.1")
  ENDIF(MSVC71)

  SET(DEP_INCLUDE_DIR ${DEPENDENCY_PACKAGE_DIR}/include)
  SET(DEP_LIBRARY_DIR ${DEPENDENCY_PACKAGE_DIR}/lib/msvc${_msvc_version}-${BINARY_POSTFIX})
  SET(DEP_BINARY_DIR  ${DEPENDENCY_PACKAGE_DIR}/bin/msvc${_msvc_version}-${BINARY_POSTFIX})
  IF(NOT EXISTS ${DEP_LIBRARY_DIR})
    message(FATAL_ERROR "No binaries found for your MSVC Version (${CMAKE_GENERATOR})")
  ENDIF()

  # Also the directory with the runtime libraries
  SET(ORXONOX_RUNTIME_LIBRARY_DIRECTORY ${DEP_BINARY_DIR})

  # Sets the library path for the FIND_LIBRARY
  SET(CMAKE_LIBRARY_PATH ${DEP_LIBRARY_DIR})

  # Include paths and other special treatments
  SET(ENV{ALUTDIR}               ${DEP_INCLUDE_DIR}/freealut-1.1.0)
  SET(ENV{BOOST_ROOT}            ${DEP_INCLUDE_DIR}/boost-1.37.0)
  SET(ENV{CEGUIDIR}              ${DEP_INCLUDE_DIR}/cegui-0.6.2)
  SET(ENV{DXSDK_DIR}             ${DEP_INCLUDE_DIR}/directx-2007.aug)
  SET(ENV{ENETDIR}               ${DEP_INCLUDE_DIR}/enet-1.2)
  SET(ENV{LUA_DIR}               ${DEP_INCLUDE_DIR}/lua-5.1.4)
  SET(ENV{OGGDIR}                ${DEP_INCLUDE_DIR}/libogg-1.1.3)
  SET(ENV{VORBISDIR}             ${DEP_INCLUDE_DIR}/libvorbis-1.2.0)
  SET(ENV{OGRE_HOME}             ${DEP_INCLUDE_DIR}/ogre-1.4.9)
  SET(ENV{OGRE_PLUGIN_DIR}       ${DEP_BINARY_DIR})
  SET(ENV{OPENALDIR}             ${DEP_INCLUDE_DIR}/openal-1.1)
  LIST(APPEND CMAKE_INCLUDE_PATH ${DEP_INCLUDE_DIR}/tcl-8.5.2/include)
  SET(TCL_LIBRARY                ${DEP_LIBRARY_DIR}/tcl85.lib CACHE FILEPATH "")
  LIST(APPEND CMAKE_INCLUDE_PATH ${DEP_INCLUDE_DIR}/zlib-1.2.3/include)
  SET(ZLIB_LIBRARY               ${DEP_LIBRARY_DIR}/zdll.lib CACHE FILEPATH "")

ENDIF(MSVC)
