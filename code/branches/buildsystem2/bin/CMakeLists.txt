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
 #    Configures the binary output directory with files required for Orxonox
 #    to run like orxonox.ini or the default keybindings.
 #    Also creates a run-script in the root directory of the build tree.
 #
 
SET(READ_ONLY_CONFIG_FILES
  def_keybindings.ini
  def_masterKeybindings.ini
  disco.txt
  irc.tcl
  remote.tcl
  telnet_server.tcl
)

SET(WRITABLE_CONFIG_FILES
)

# Not getting installed
SET(ORXONOX_INI orxonox.ini)

IF(TARDIS)
  # OGRE can't find fonts to display config screen on Tardis,
  # so providing default config file here (bug).
  LIST(APPEND WRITABLE_CONFIG_FILES ogre.cfg)
ENDIF(TARDIS)

# We need the same code for both READ_ONLY and WRITABLE config files
MACRO(CONFIGURE_FILES _file_name _build_configs _read_only_arg)
  SET(_read_only ${_read_only_arg})
  FOREACH(_build_config ${_build_configs})
    # Is there an extra file in bin/Debug or bin/Release?
    IF(${_build_config} MATCHES "Rel")
      SET(_build_config_short "Release")
    ELSE()
      SET(_build_config_short "Debug")
    ENDIF()
    IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_build_config_short}/${_file_name})
      SET(_in_file ${CMAKE_CURRENT_SOURCE_DIR}/${_build_config_short}/${_file_name})
    ELSE()
      SET(_in_file ${CMAKE_CURRENT_SOURCE_DIR}/${_file_name})
    ENDIF()

    # Copy to the folder named like the build config for Visual Studio
    IF(CMAKE_CONFIGURATION_TYPES)
      SET(_out_file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_build_config}/${_file_name})
    ELSE()
      SET(_out_file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_file_name})
    ENDIF()
    # Only copy if target file doesn't exist. This may result in problems but
    # otherwise we might delete a user's config
    IF(NOT EXISTS ${_out_file} OR _read_only)
      CONFIGURE_FILE(${_in_file} ${_out_file} @ONLY)
    ENDIF()
  ENDFOREACH(_build_config)
ENDMACRO(CONFIGURE_FILES)

# Copy config files to all Visual Studio output directories
IF(CMAKE_CONFIGURATION_TYPES)
  SET(BUILD_CONFIGS ${CMAKE_CONFIGURATION_TYPES})
ELSE()
  SET(CONFIG_OUT_PATHS_REL ".")
  SET(BUILD_CONFIGS ${CMAKE_BUILD_TYPE})
ENDIF()

FOREACH(_file_name ${READ_ONLY_CONFIG_FILES})
  CONFIGURE_FILES("${_file_name}" "${BUILD_CONFIGS}" TRUE)
ENDFOREACH(_file_name)
FOREACH(_file_name ${WRITABLE_CONFIG_FILES} ${ORXONOX_INI})
  CONFIGURE_FILES("${_file_name}" "${BUILD_CONFIGS}" FALSE)
ENDFOREACH(_file_name)


################ Installation #################

# Not using collective call to allow configuration with CMake.
FOREACH(_file ${READ_ONLY_CONFIG_FILES} ${WRITABLE_CONFIG_FILES})
  IF(CMAKE_CONFIGURATION_TYPES)
    FOREACH(_configuration ${CMAKE_CONFIGURATION_TYPES})
      INSTALL(FILES ${CMAKE_BINARY_DIR}/bin/${_configuration}/${_file}
              DESTINATION bin CONFIGURATIONS ${_configuration})
    ENDFOREACH(_configuration)
  ELSE()
    INSTALL(FILES ${CMAKE_BINARY_DIR}/bin/${_file} DESTINATION bin)
  ENDIF()
ENDFOREACH(_file)


################ Run Scripts ##################

# Create a run script for both Windows and Linux in the source root path if
# CMake is not used to create multi-configuration project files
IF(NOT CMAKE_CONFIGURATION_TYPES)
  IF(WIN32)
    SET(RUN_SCRIPT run.bat)
    # Note: Do not use FILE(TO_NATIVE_PATH) because it doesn't work for MinGW
    STRING(REGEX REPLACE "^([A-Z]\\:)\\/.*$" "\\1" WINDOWS_DRIVE_CHANGE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    STRING(REPLACE "/" "\\" CMAKE_RUNTIME_OUTPUT_DIRECTORY_WINDOWS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    STRING(REPLACE "/" "\\" ORXONOX_RUNTIME_LIBRARY_DIRECTORY_WINDOWS ${ORXONOX_RUNTIME_LIBRARY_DIRECTORY})
  ELSE(UNIX)
    SET(RUN_SCRIPT run)
  ENDIF(WIN32)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${RUN_SCRIPT} ${CMAKE_SOURCE_DIR}/${RUN_SCRIPT} @ONLY)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${RUN_SCRIPT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${RUN_SCRIPT} @ONLY)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES)
