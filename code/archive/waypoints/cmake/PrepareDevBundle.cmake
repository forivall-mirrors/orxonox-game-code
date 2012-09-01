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
 #    Kevin Young
 #  Description:
 #    Creates the file structure for the development application bundle
 #

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
  SET(DEVBUNDLE_NIB_FILE "MainMenu.nib")

  # Create bundle folder structure
  FILE(MAKE_DIRECTORY "${DEVBUNDLE_CURRENT_DIR}")
  FILE(MAKE_DIRECTORY "${DEVBUNDLE_CURRENT_BIN}")
  FILE(MAKE_DIRECTORY "${DEVBUNDLE_CURRENT_RESOURCES}")
  FILE(MAKE_DIRECTORY "${DEVBUNDLE_CURRENT_RESOURCES}/${DEVBUNDLE_DEFAULT_LANGFILE}/${DEVBUNDLE_NIB_FILE}")
  
  # Populate with Info.plist, NIB-file and the Icon.icns
  CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/${DEFAULT_DATA_PATH}/mac/Orxonox-Info.plist" "${DEVBUNDLE_CURRENT_DIR}/Info.plist")
  CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/${DEFAULT_DATA_PATH}/mac/Icon.icns" "${DEVBUNDLE_CURRENT_RESOURCES}/Icon.icns" COPYONLY)
  CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/${DEFAULT_DATA_PATH}/mac/${DEVBUNDLE_DEFAULT_LANGFILE}/${DEVBUNDLE_NIB_FILE}/designable.nib" "${DEVBUNDLE_CURRENT_RESOURCES}/${DEVBUNDLE_DEFAULT_LANGFILE}/${DEVBUNDLE_NIB_FILE}/designable.nib" COPYONLY)
  CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/${DEFAULT_DATA_PATH}/mac/${DEVBUNDLE_DEFAULT_LANGFILE}/${DEVBUNDLE_NIB_FILE}/keyedobjects.nib" "${DEVBUNDLE_CURRENT_RESOURCES}/${DEVBUNDLE_DEFAULT_LANGFILE}/${DEVBUNDLE_NIB_FILE}/keyedobjects.nib" COPYONLY)
  CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/${DEFAULT_DATA_PATH}/mac/${DEVBUNDLE_DEFAULT_LANGFILE}/InfoPlist.strings" "${DEVBUNDLE_CURRENT_RESOURCES}/${DEVBUNDLE_DEFAULT_LANGFILE}/InfoPlist.strings")

ENDFOREACH(_subdir)
