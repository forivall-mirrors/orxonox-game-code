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

  INCLUDE(CheckPackageVersion)
  CHECK_PACKAGE_VERSION(6.0 7.0)

  # 64 bit system?
  IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(BINARY_POSTFIX x64)
  ELSE()
    SET(BINARY_POSTFIX x86)
  ENDIF()

  # Choose right MSVC version
  STRING(REGEX REPLACE "^Visual Studio ([0-9][0-9]?).*$" "\\1"
         _msvc_version "${CMAKE_GENERATOR}")

  SET(DEP_INCLUDE_DIR ${DEPENDENCY_PACKAGE_DIR}/include)
  SET(DEP_LIBRARY_DIR ${DEPENDENCY_PACKAGE_DIR}/lib/msvc${_msvc_version}-${BINARY_POSTFIX})
  SET(DEP_BINARY_DIR  ${DEPENDENCY_PACKAGE_DIR}/bin/msvc${_msvc_version}-${BINARY_POSTFIX})
  IF(NOT EXISTS ${DEP_LIBRARY_DIR})
    message(FATAL_ERROR "No binaries found for your MSVC Version (${CMAKE_GENERATOR})")
  ENDIF()

  # Also the directory with the runtime libraries
  SET(RUNTIME_LIBRARY_DIRECTORY ${DEP_BINARY_DIR})

  # Sets the library path for the FIND_LIBRARY
  SET(CMAKE_LIBRARY_PATH ${DEP_LIBRARY_DIR})
  
  # Part of Platform SDK and usually gets linked automatically
  SET(WMI_LIBRARY  wbemuuid.lib)

ENDIF(MSVC)
