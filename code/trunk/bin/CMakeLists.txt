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
 #    Creates run scripts in the root directory of the build tree and
 #    the runtime output directory.
 #

# Create run scripts for Windows to manually add the DLL path when executing
SET(RUN_SCRIPTS run standalone client1 client2 server dedicated dedicatedClient)
IF(WIN32)
  FOREACH(_script ${RUN_SCRIPTS})
    LIST(REMOVE_ITEM RUN_SCRIPTS ${_script})
    LIST(APPEND RUN_SCRIPTS ${_script}.bat)
  ENDFOREACH(_script)
ENDIF()
# Also copy vld.ini (visual leak detector config) for MSVC
IF(MSVC)
  LIST(APPEND RUN_SCRIPTS vld.ini)
ENDIF()

IF(NOT CMAKE_CONFIGURATION_TYPES)
  SET(_subdirs .)
  SEt(_default_subdir .)
ELSE()
  SET(_subdirs ${CMAKE_CONFIGURATION_TYPES})
  SEt(_default_subdir) # No default subdir
ENDIF()
STRING(REPLACE "/" "\\" RUNTIME_LIBRARY_DIRECTORY_WINDOWS ${RUNTIME_LIBRARY_DIRECTORY})
FOREACH(_subdir ${_subdirs})
  GET_FILENAME_COMPONENT(CURRENT_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_subdir} ABSOLUTE)

  # Note: Do not use FILE(TO_NATIVE_PATH) because it doesn't work for MinGW
  STRING(REGEX REPLACE "^([A-Z]\\:)\\/.*$" "\\1" WINDOWS_DRIVE_CHANGE ${CURRENT_RUNTIME_DIR})
  STRING(REPLACE "/" "\\" CURRENT_RUNTIME_DIR_WINDOWS ${CURRENT_RUNTIME_DIR})

  FOREACH(_script ${RUN_SCRIPTS})
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${_script}.in ${CURRENT_RUNTIME_DIR}/${_script} @ONLY)
    IF(_subdir STREQUAL _default_subdir)
      # Convenience script to be used when sitting in the binary directory. Does nothing for MSVC
      CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${_script}.in ${CMAKE_BINARY_DIR}/${_script} @ONLY)
    ENDIF()
  ENDFOREACH(_script)

  # In order to evaluate at run time whether it is a dev build in the build tree,
  # we plant a special file
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/orxonox_dev_build.keep_me ${CURRENT_RUNTIME_DIR}/orxonox_dev_build.keep_me)
ENDFOREACH(_subdir)


# Make a symlink to be able to run from the root directory when installing copyable on Unix
IF(UNIX AND INSTALL_COPYABLE)
  INSTAll(CODE "EXECUTE_PROCESS(COMMAND ln -sf ${RUNTIME_INSTALL_DIRECTORY}/${ORXONOX_EXECUTABLE_NAME} ${CMAKE_INSTALL_PREFIX}/orxonox)")
ENDIF()
