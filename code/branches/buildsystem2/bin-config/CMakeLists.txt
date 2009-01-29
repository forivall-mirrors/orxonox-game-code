SET(READ_ONLY_CONFIG_FILES
  def_keybindings.ini
  def_masterKeybindings.ini
  disco.txt
  irc.tcl
  remote.tcl
  telnet_server.tcl
)

SET(WRITABLE_CONFIG_FILES
  orxonox.ini
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
    ELSE(${_build_config} MATCHES "Rel")
      SET(_build_config_short "Debug")
    ENDIF(${_build_config} MATCHES "Rel")
    IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_build_config_short}/${_file_name})
      SET(_in_file ${CMAKE_CURRENT_SOURCE_DIR}/${_build_config_short}/${_file_name})
    ELSE(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_build_config_short}/${_file_name})
      SET(_in_file ${CMAKE_CURRENT_SOURCE_DIR}/${_file_name})
    ENDIF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_build_config_short}/${_file_name})

    # Copy to the folder named like the build config for Visual Studio
    IF(CMAKE_CONFIGURATION_TYPES)
      SET(_out_file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_build_config}/${_file_name})
    ELSE(CMAKE_CONFIGURATION_TYPES)
      SET(_out_file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_file_name})
    ENDIF(CMAKE_CONFIGURATION_TYPES)
    # Only copy if target file doesn't exist. This may result in problems but
    # otherwise we might delete a user's config
    IF (NOT EXISTS ${_out_file} OR _read_only)
      CONFIGURE_FILE(${_in_file} ${_out_file} @ONLY)
    ENDIF (NOT EXISTS ${_out_file} OR _read_only)
  ENDFOREACH(_build_config)
ENDMACRO(CONFIGURE_FILES)

# Copy config files to all Visual Studio output directories
IF(CMAKE_CONFIGURATION_TYPES)
  SET(BUILD_CONFIGS ${CMAKE_CONFIGURATION_TYPES})
ELSE(CMAKE_CONFIGURATION_TYPES)
  SET(CONFIG_OUT_PATHS_REL ".")
  SET(BUILD_CONFIGS ${CMAKE_BUILD_TYPE})
ENDIF(CMAKE_CONFIGURATION_TYPES)

FOREACH(_file_name ${READ_ONLY_CONFIG_FILES})
  CONFIGURE_FILES("${_file_name}" "${BUILD_CONFIGS}" TRUE)
ENDFOREACH(_file_name)
FOREACH(_file_name ${WRITABLE_CONFIG_FILES})
  CONFIGURE_FILES("${_file_name}" "${BUILD_CONFIGS}" FALSE)
ENDFOREACH(_file_name)

################ Run Scripts ##################

# Create a run script for both Windows and Linux in the source root path if
# CMake is not used to create multi-configuration project files
IF(NOT CMAKE_CONFIGURATION_TYPES)
  IF(WIN32)
    SET(RUN_SCRIPT run.bat)
    # Note: Do not use FILE(TO_NATIVE_PATH) because it doesn't work for MinGW
    STRING(REGEX REPLACE "^([A-Z]\\:)\\/.*$" "\\1" WINDOWS_DRIVE_CHANGE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    STRING(REPLACE "/" "\\" CMAKE_RUNTIME_OUTPUT_DIRECTORY_WINDOWS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  ELSE(UNIX)
    SET(RUN_SCRIPT run)
  ENDIF(WIN32)
  IF (NOT EXISTS ${CMAKE_SOURCE_DIR}/${RUN_SCRIPT})
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${RUN_SCRIPT} ${CMAKE_SOURCE_DIR}/${RUN_SCRIPT} @ONLY)
  ENDIF (NOT EXISTS ${CMAKE_SOURCE_DIR}/${RUN_SCRIPT})
ENDIF(NOT CMAKE_CONFIGURATION_TYPES)
