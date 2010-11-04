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
 #    OS X package configuration
 #

INCLUDE(CheckPackageVersion)

CHECK_PACKAGE_VERSION(1.0)

IF(NOT _INTERNAL_PACKAGE_MESSAGE)
  MESSAGE(STATUS "Using library package for the dependencies.")
  SET(_INTERNAL_PACKAGE_MESSAGE 1 CACHE INTERNAL "Do not edit!" FORCE)
ENDIF()

SET(DEP_INCLUDE_DIR ${DEPENDENCY_PACKAGE_DIR}/include)
SET(DEP_LIBRARY_DIR ${DEPENDENCY_PACKAGE_DIR}/lib)
SET(DEP_BINARY_DIR  ${DEPENDENCY_PACKAGE_DIR}/bin)
SET(DEP_FRAMEWORK_DIR ${DEPENDENCY_PACKAGE_DIR}/Library/Frameworks)

# Sets the library path for the FIND_LIBRARY
SET(CMAKE_LIBRARY_PATH ${DEP_LIBRARY_DIR})

# Certain find scripts don't behave as ecpected so we have
# to specify the libraries ourselves.
#SET(TCL_LIBRARY  ${DEP_BINARY_DIR}/tcl85.dll CACHE FILEPATH "")
#SET(ZLIB_LIBRARY ${DEP_BINARY_DIR}/zlib1.dll CACHE FILEPATH "")

# Include paths and other special treatments
SET(ENV{ALUTDIR}               ${DEP_FRAMEWORK_DIR})
SET(ENV{BOOST_ROOT}            ${DEPENDENCY_PACKAGE_DIR})
SET(ENV{CEGUIDIR}              ${DEP_FRAMEWORK_DIR})
#SET(ENV{DBGHELP_DIR}           ${DEP_INCLUDE_DIR}/dbghelp)
#SET(ENV{DXSDK_DIR}             ${DEP_INCLUDE_DIR}/directx)
#SET(ENV{ENETDIR}               ${DEP_INCLUDE_DIR}/enet)
SET(ENV{LUA_DIR}               ${DEP_INCLUDE_DIR}/lua)
SET(ENV{OGGDIR}                ${DEP_INCLUDE_DIR})
SET(ENV{VORBISDIR}             ${DEP_INCLUDE_DIR})
SET(ENV{OGRE_HOME}             ${DEP_FRAMEWORK_DIR})
SET(ENV{OGRE_PLUGIN_DIR}       ${DEP_BINARY_DIR})
#SET(ENV{OPENALDIR}             ${DEP_INCLUDE_DIR}/openal)
#SET(ENV{POCODIR}               ${DEP_INCLUDE_DIR}/poco)
#LIST(APPEND CMAKE_INCLUDE_PATH ${DEP_INCLUDE_DIR}/tcl/include)
#LIST(APPEND CMAKE_INCLUDE_PATH ${DEP_INCLUDE_DIR}/zlib/include)

### INSTALL ###

# Tcl script library
# TODO: How does this work on OS X?
#INSTALL(
#  DIRECTORY ${DEP_LIBRARY_DIR}/tcl/
#  DESTINATION lib/tcl
#)

# TODO: Install on OSX
IF(FALSE)
  ## DEBUG
  # When installing a debug version, we really can't know which libraries
  # are used in released mode because there might be deps of deps.
  # --> Copy all of them, except the debug databases
  INSTALL(
    DIRECTORY ${DEP_BINARY_DIR}/
    DESTINATION bin
    CONFIGURATIONS Debug
    REGEX "^.*\\.pdb$" EXCLUDE
  )

  ## RELEASE
  # Try to filter out all the debug libraries. If the regex doesn't do the
  # job anymore, simply adjust it.
  INSTALL(
    DIRECTORY ${DEP_BINARY_DIR}/
    DESTINATION bin
    CONFIGURATIONS Release RelWithDebInfo MinSizeRel
    REGEX "_[Dd]\\.[a-zA-Z0-9+-]+$|-mt-gd-|^.*\\.pdb$" EXCLUDE
  )
ENDIF()
