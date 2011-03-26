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

SET(DEP_INCLUDE_DIR   ${DEPENDENCY_PACKAGE_DIR}/include)
SET(DEP_LIBRARY_DIR   ${DEPENDENCY_PACKAGE_DIR}/lib)
SET(DEP_BINARY_DIR    ${DEPENDENCY_PACKAGE_DIR}/bin)
SET(DEP_FRAMEWORK_DIR ${DEPENDENCY_PACKAGE_DIR}/Library/Frameworks)

# Sets the library and framwork paths for the FIND_LIBRARY commands
SET(CMAKE_LIBRARY_PATH   ${DEP_LIBRARY_DIR})
SET(CMAKE_FRAMEWORK_PATH ${DEP_FRAMEWORK_DIR})

# Include paths and other special treatments
SET(ENV{BOOST_ROOT}      ${DEPENDENCY_PACKAGE_DIR})
SET(ENV{LUA5.1_DIR}      ${DEP_FRAMEWORK_DIR}/CEGUILuaScriptModule.framework/Versions/A/Resources)
SET(ENV{OGGDIR}          ${DEP_INCLUDE_DIR})
SET(ENV{VORBISDIR}       ${DEP_INCLUDE_DIR})
SET(ENV{OGRE_PLUGIN_DIR} ${DEP_BINARY_DIR})

# For OS X 10.5 we have to ship modified headers to make it compile
# on gcc >= 4.2 (binaries stay the same)
# Sets the library path for the FIND_LIBRARY
IF(CMAKE_SYSTEM_VERSION STREQUAL "10.5")
  SET(ENV{OPENALDIR} ${DEP_INCLUDE_DIR}/openal)
ENDIF()

# Xcode won't be able to run the toluabind code generation if we're using the dependency package
#IF(DEPENDENCY_PACKAGE_ENABLE)
#  IF(${CMAKE_GENERATOR} STREQUAL "Xcode")
#    SET(ENV{DYLD_LIBRARY_PATH}               ${DEPENDENCY_PACKAGE_DIR}/lib)
#    SET(ENV{DYLD_FRAMEWORK_PATH}             ${DEPENDENCY_PACKAGE_DIR}/Library/Frameworks)
#  ENDIF(${CMAKE_GENERATOR} STREQUAL "Xcode")
#ENDIF(DEPENDENCY_PACKAGE_ENABLE)

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
