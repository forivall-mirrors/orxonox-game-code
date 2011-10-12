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
 #    General package configuration. Merely sets the include paths.
 #    Library files are treated separately.
 #

IF(NOT _INTERNAL_PACKAGE_MESSAGE)
  MESSAGE(STATUS "Using library package for the dependencies.")
  SET(_INTERNAL_PACKAGE_MESSAGE 1 CACHE INTERNAL "Do not edit!" FORCE)
ENDIF()

# Ogre versions >= 1.7 require the POCO library on Windows with MSVC for threading
COMPARE_VERSION_STRINGS(${DEPENDENCY_VERSION} 5 _result TRUE)
IF(NOT _result EQUAL -1 AND NOT APPLE)
  SET(OGRE_NEEDS_POCO TRUE)
ENDIF()

# Include paths and other special treatments
SET(ENV{ALUTDIR}               ${DEP_INCLUDE_DIR}/freealut)
SET(ENV{BOOST_ROOT}            ${DEP_INCLUDE_DIR}/boost)
SET(ENV{CEGUIDIR}              ${DEP_INCLUDE_DIR}/cegui)
SET(ENV{DBGHELP_DIR}           ${DEP_INCLUDE_DIR}/dbghelp)
SET(ENV{DXSDK_DIR}             ${DEP_INCLUDE_DIR}/directx)
#SET(ENV{ENETDIR}               ${DEP_INCLUDE_DIR}/enet)
SET(ENV{LUA5.1_DIR}            ${DEP_INCLUDE_DIR}/lua)
SET(ENV{OGGDIR}                ${DEP_INCLUDE_DIR}/libogg)
SET(ENV{VORBISDIR}             ${DEP_INCLUDE_DIR}/libvorbis)
SET(ENV{OGRE_HOME}             ${DEP_INCLUDE_DIR}/ogre)
SET(ENV{OGRE_PLUGIN_DIR}       ${DEP_BINARY_DIR})
SET(ENV{OPENALDIR}             ${DEP_INCLUDE_DIR}/openal)
SET(ENV{POCODIR}               ${DEP_INCLUDE_DIR}/poco)
LIST(APPEND CMAKE_INCLUDE_PATH ${DEP_INCLUDE_DIR}/tcl/include)
LIST(APPEND CMAKE_INCLUDE_PATH ${DEP_INCLUDE_DIR}/zlib/include)

### INSTALL ###

# Tcl script library
INSTALL(
  DIRECTORY ${DEP_LIBRARY_DIR}/tcl/
  DESTINATION lib/tcl
)

# On Windows, DLLs have to be in the executable folder, install them
IF(WIN32 AND DEP_BINARY_DIR)
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
  IF(EXISTS ${DEP_BINARY_DIR}/install_manifest.txt)
    FILE(STRINGS ${DEP_BINARY_DIR}/install_manifest.txt _files)
    FOREACH(_file ${_files})
      INSTALL(
        FILES ${DEP_BINARY_DIR}/${_file}
        DESTINATION bin
        CONFIGURATIONS RelForDevs Release RelWithDebInfo MinSizeRel
      )
      IF(INSTALL_PDB_FILES) # MSVC specific: install debug symbols files
        GET_FILENAME_COMPONENT(_base_name ${_file} NAME_WE)
        IF(EXISTS ${DEP_BINARY_DIR}/${_base_name}.pdb)
          INSTALL(
            FILES ${DEP_BINARY_DIR}/${_base_name}.pdb
            DESTINATION bin
            CONFIGURATIONS RelForDevs RelWithDebInfo
          )
        ENDIF()
      ENDIF()
    ENDFOREACH(_file)
  ELSE()
    # Try to filter out all the debug libraries. If the regex doesn't do the
    # job anymore, simply adjust it.
    INSTALL(
      DIRECTORY ${DEP_BINARY_DIR}/
      DESTINATION bin
      CONFIGURATIONS RelForDevs Release RelWithDebInfo MinSizeRel
      REGEX "_[Dd]\\.[a-zA-Z0-9+-]+$|-mt-gd-|^.*\\.pdb$" EXCLUDE
    )
  ENDIF()
ENDIF()
