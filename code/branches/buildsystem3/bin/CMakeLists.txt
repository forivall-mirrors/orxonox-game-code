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
 #    Creates a run-script in the root directory of the build tree.
 #

############ Configure Dev Build ##############


################ Run Scripts ##################

# Create a run script for Windows to manually add the DLL path when executing
IF(WIN32)
  SET(RUN_SCRIPT run.bat)
ELSEIF(UNIX)
  SET(RUN_SCRIPT run)
ENDIF()

IF(NOT CMAKE_CONFIGURATION_TYPES)
  SET(_subdirs .)
  SEt(_default_subdir .)
ELSE()
  SET(_subdirs ${CMAKE_CONFIGURATION_TYPES})
  SEt(_default_subdir Debug)
ENDIF()
STRING(REPLACE "/" "\\" ORXONOX_RUNTIME_LIBRARY_DIRECTORY_WINDOWS ${ORXONOX_RUNTIME_LIBRARY_DIRECTORY})
FOREACH(_subdir ${_subdirs})
  GET_FILENAME_COMPONENT(CURRENT_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_subdir} ABSOLUTE)

  # Note: Do not use FILE(TO_NATIVE_PATH) because it doesn't work for MinGW
  STRING(REGEX REPLACE "^([A-Z]\\:)\\/.*$" "\\1" WINDOWS_DRIVE_CHANGE ${CURRENT_RUNTIME_DIR})
  STRING(REPLACE "/" "\\" CURRENT_RUNTIME_DIR_WINDOWS ${CURRENT_RUNTIME_DIR})

  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${RUN_SCRIPT}.in ${CURRENT_RUNTIME_DIR}/${RUN_SCRIPT} @ONLY)
  IF(_subdir STREQUAL _default_subdir)
    # Convenience script to be used when sitting in the binary directory. Defaults to Debug for msvc.
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${RUN_SCRIPT}.in ${CMAKE_BINARY_DIR}/${RUN_SCRIPT} @ONLY)
  ENDIF()

  # In order to evaluate at run time whether it is a dev build in the build tree,
  # we plant a special file
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/orxonox_dev_build.keep_me ${CURRENT_RUNTIME_DIR}/orxonox_dev_build.keep_me)
ENDFOREACH(_subdir)
