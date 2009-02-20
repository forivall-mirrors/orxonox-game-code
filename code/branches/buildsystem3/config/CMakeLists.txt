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
 
SET(ORXONOX_CONFIG_DEV_PATH ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

SET(READ_ONLY_CONFIG_FILES
  def_keybindings.ini
  def_masterKeybindings.ini
)

SET(WRITABLE_CONFIG_FILES
)

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
      SET(_out_file ${CMAKE_CURRENT_BINARY_DIR}/${_build_config}/${_file_name})
    ELSE()
      SET(_out_file ${CMAKE_CURRENT_BINARY_DIR}/${_file_name})
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
  SET(BUILD_CONFIGS ${CMAKE_BUILD_TYPE})
ENDIF()

FOREACH(_file_name ${READ_ONLY_CONFIG_FILES})
  CONFIGURE_FILES("${_file_name}" "${BUILD_CONFIGS}" TRUE)
ENDFOREACH(_file_name)
FOREACH(_file_name ${WRITABLE_CONFIG_FILES})
  CONFIGURE_FILES("${_file_name}" "${BUILD_CONFIGS}" FALSE)
ENDFOREACH(_file_name)


################ Installation #################

# Not using collective call to allow configuration with CMake.
FOREACH(_file ${READ_ONLY_CONFIG_FILES} ${WRITABLE_CONFIG_FILES})
  IF(CMAKE_CONFIGURATION_TYPES)
    FOREACH(_configuration ${CMAKE_CONFIGURATION_TYPES})
      INSTALL(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${_configuration}/${_file}
        DESTINATION ${ORXONOX_CONFIG_INSTALL_PATH}
        CONFIGURATIONS ${_configuration}
      )
    ENDFOREACH(_configuration)
  ELSE()
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_file} DESTINATION ${ORXONOX_CONFIG_INSTALL_PATH})
  ENDIF()
ENDFOREACH(_file)
