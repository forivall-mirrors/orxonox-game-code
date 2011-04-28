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
IF(APPLE)

SET_SOURCE_FILES(CCR_FILES
  CMCrashReporter.m
  CMCrashReporterGlobal.m
  CMCrashReporterWindow.m
)

ORXONOX_ADD_LIBRARY(cmcrashreporter_orxonox
  ORXONOX_EXTERNAL
  LINK_LIBRARIES
    "-framework CoreFoundation"
    "-framework Cocoa"
    "-framework AddressBook"
  SOURCE_FILES
    ${CCR_FILES}
)

IF(NOT CMAKE_CONFIGURATION_TYPES)
  SET(_subdirs .)
  SET(_default_subdir .)
ELSE()
  SET(_subdirs ${CMAKE_CONFIGURATION_TYPES})
  SET(_default_subdir) # No default subdir
ENDIF()

FOREACH(_subdir ${_subdirs})
  GET_FILENAME_COMPONENT(CURRENT_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_subdir} ABSOLUTE)

  # Convenience variables
  # Note that this is just an empty bundle! The executable itself will
  # be added at build time
  SET(DEVBUNDLE_CURRENT_DIR "${CURRENT_RUNTIME_DIR}/${PROJECT_NAME}.app/Contents")
  SET(DEVBUNDLE_CURRENT_BIN "${DEVBUNDLE_CURRENT_DIR}/MacOS")
  SET(DEVBUNDLE_CURRENT_RESOURCES "${DEVBUNDLE_CURRENT_DIR}/Resources")
  SET(DEVBUNDLE_DEFAULT_LANGFILE "English.lproj")
  SET(DEVBUNDLE_NIB_FILE "CMCrashReporterWindow.nib")

  # Create bundle folder structure
  FILE(MAKE_DIRECTORY "${DEVBUNDLE_CURRENT_DIR}")
  FILE(MAKE_DIRECTORY "${DEVBUNDLE_CURRENT_BIN}")
  FILE(MAKE_DIRECTORY "${DEVBUNDLE_CURRENT_RESOURCES}")
  FILE(MAKE_DIRECTORY "${DEVBUNDLE_CURRENT_RESOURCES}/${DEVBUNDLE_DEFAULT_LANGFILE}/${DEVBUNDLE_NIB_FILE}")
  
  # Populate app with additional NIB file.
  CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/${DEVBUNDLE_NIB_FILE}/keyedobjects.nib" "${DEVBUNDLE_CURRENT_RESOURCES}/${DEVBUNDLE_DEFAULT_LANGFILE}/${DEVBUNDLE_NIB_FILE}/keyedobjects.nib" COPYONLY)
  CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/${DEVBUNDLE_NIB_FILE}/designable.nib" "${DEVBUNDLE_CURRENT_RESOURCES}/${DEVBUNDLE_DEFAULT_LANGFILE}/${DEVBUNDLE_NIB_FILE}/designable.nib" COPYONLY)


ENDFOREACH(_subdir)

ENDIF(APPLE)
