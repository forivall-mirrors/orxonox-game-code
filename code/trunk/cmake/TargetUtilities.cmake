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
 #    Adds a library or an executable like ADD_LIBRARY/ADD_EXECUTABLE, but
 #    accepts a lot more input information. Simply supply the keywords
 #    described below in any order you wish.
 #
 #    Switches: (when given --> TRUE, FALSE otherwise)
 #      FIND_HEADER_FILES: Searches the current directory for all header files
 #                         and adds them to the target.
 #      EXCLUDE_FROM_ALL:  Inherited from ADD_LIBRARY/ADD_EXECUTABLE
 #      ORXONOX_EXTERNAL:  Specify this for third party libraries
 #      NO_DLL_INTERFACE:  Link statically with MSVC
 #      NO_SOURCE_GROUPS:  Don't create msvc source groups
 #      STATIC/SHARED:     Inherited from ADD_LIBRARY
 #      WIN32:             Inherited from ADD_EXECUTABLE
 #      PCH_NO_DEFAULT:    Do not make precompiled header files default if
 #                         specified with PCH_FILE
 #    Lists:
 #      LINK_LIBRARIES:    Redirects to TARGET_LINK_LIBRARIES
 #      VERSION:           Set version to the binary
 #      SOURCE_FILES:      Source files for the target
 #      DEFINE_SYMBOL:     Sets the DEFINE_SYMBOL target property
 #      TOLUA_FILES:       Files with tolua interface
 #      PCH_FILE:          Precompiled header file
 #      PCH_EXCLUDE:       Source files to be excluded from PCH support
 #  Note:
 #    This function also installs the target!
 #  Prerequisistes:
 #    ORXONOX_DEFAULT_LINK, ORXONOX_CONFIG_FILES
 #  Parameters:
 #    _target_name, ARGN for the macro arguments
 #

INCLUDE(CapitaliseName)
INCLUDE(GenerateToluaBindings)
INCLUDE(ParseMacroArguments)
INCLUDE(SourceFileUtilities)
IF(PCH_COMPILER_SUPPORT)
  INCLUDE(PrecompiledHeaderFiles)
ENDIF()

FUNCTION(ORXONOX_ADD_LIBRARY _target_name)
  TU_ADD_TARGET(${_target_name} LIBRARY "STATIC;SHARED" ${ARGN})
ENDFUNCTION(ORXONOX_ADD_LIBRARY)

FUNCTION(ORXONOX_ADD_EXECUTABLE _target_name)
  TU_ADD_TARGET(${_target_name} EXECUTABLE "WIN32" ${ARGN})
ENDFUNCTION(ORXONOX_ADD_EXECUTABLE)


FUNCTION(TU_ADD_TARGET _target_name _target_type _additional_switches)
  CAPITALISE_NAME(${_target_name} _target_name_capitalised)

  # Specify all possible options (either switch or with add. arguments)
  SET(_switches   FIND_HEADER_FILES  EXCLUDE_FROM_ALL  ORXONOX_EXTERNAL
                  NO_DLL_INTERFACE   NO_SOURCE_GROUPS  ${_additional_switches}
                  PCH_NO_DEFAULT)
  SET(_list_names LINK_LIBRARIES  VERSION   SOURCE_FILES  DEFINE_SYMBOL
                  TOLUA_FILES     PCH_FILE  PCH_EXCLUDE)
  PARSE_MACRO_ARGUMENTS("${_switches}" "${_list_names}" ${ARGN})


  # GET_HEADER_FILES
  IF(_arg_FIND_HEADER_FILES)
    GET_ALL_HEADER_FILES(_${target_name}_header_files)
  ENDIF()

  # Remove potential duplicates
  SET(_${_target_name}_files ${_${target_name}_header_files} ${_arg_SOURCE_FILES})
  LIST(REMOVE_DUPLICATES _${_target_name}_files)

  # Generate the source groups
  IF(NOT _arg_NO_SOURCE_GROUPS)
    GENERATE_SOURCE_GROUPS(${_${_target_name}_files})

    IF(NOT _arg_ORXONOX_EXTERNAL)
      # Move the prereqs.h file to the config section
      IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_target_name_capitalised}Prereqs.h)
        SOURCE_GROUP("Config" FILES ${_target_name_capitalised}Prereqs.h)
      ENDIF()
      # Add the config files in a special source group
      LIST(APPEND _${_target_name}_files ${ORXONOX_CONFIG_FILES})
      SOURCE_GROUP("Config" FILES ${ORXONOX_CONFIG_FILES})
    ENDIF()
  ENDIF(NOT _arg_NO_SOURCE_GROUPS)

  # TOLUA_FILES
  IF(_arg_TOLUA_FILES)
    GENERATE_TOLUA_BINDINGS(${_target_name_capitalised} _${_target_name}_files
                            INPUTFILES ${_arg_TOLUA_FILES})
  ENDIF()

  # First part (pre target) of precompiled header files
  IF(PCH_COMPILER_SUPPORT AND PCH_ENABLE AND _arg_PCH_FILE)
    # Provide convenient option to control PCH
    STRING(TOUPPER "${_target_name}" _target_name_upper)
    IF(_arg_PCH_NO_DEFAULT)
      SET(PCH_DEFAULT FALSE)
    ELSE()
      SET(PCH_DEFAULT TRUE)
    ENDIF()
    OPTION(PCH_ENABLE_${_target_name_upper} "Enable using precompiled header files for library ${_target_name}." ${PCH_DEFAULT})

    IF(PCH_ENABLE_${_target_name_upper})
      PRECOMPILED_HEADER_FILES_PRE_TARGET(${_target_name} ${_arg_PCH_FILE} _${_target_name}_files EXCLUDE ${_arg_PCH_EXCLUDE})
    ENDIF()
  ENDIF()

  # Certain libraries don't have dllexport/dllimport and can't be linked shared with msvc
  IF(MSVC AND _arg_NO_DLL_INTERFACE)
    SET(_arg_SHARED)
    SET(_arg_STATIC STATIC)
  ENDIF()

  # Set default linking if required
  IF(NOT _arg_SHARED AND NOT _arg_STATIC)
    IF("${ORXONOX_DEFAULT_LINK}" STREQUAL "STATIC")
      SET(_arg_STATIC STATIC)
    ELSEIF("${ORXONOX_DEFAULT_LINK}" STREQUAL "SHARED")
      SET(_arg_SHARED SHARED)
    ENDIF()
  ENDIF()

  # Add the library/executable
  IF("${_target_type}" STREQUAL "LIBRARY")
    ADD_LIBRARY(${_target_name} ${_arg_STATIC} ${_arg_SHARED}
                ${_arg_EXCLUDE_FROM_ALL} ${_${_target_name}_files})
  ELSE()
    ADD_EXECUTABLE(${_target_name} ${_arg_WIN32} ${_arg_EXCLUDE_FROM_ALL}
                   ${_${_target_name}_files})
  ENDIF()

  # LINK_LIBRARIES
  IF(_arg_LINK_LIBRARIES)
    TARGET_LINK_LIBRARIES(${_target_name} ${_arg_LINK_LIBRARIES})
  ENDIF()

  # DEFINE_SYMBOL
  IF(_arg_DEFINE_SYMBOL)
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES DEFINE_SYMBOL ${_arg_DEFINE_SYMBOL})
  ENDIF()

  # VERSION
  IF(_arg_VERSION)
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES VERSION ${_arg_VERSION})
  ELSEIF(NOT _arg_ORXONOX_EXTERNAL)
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES VERSION ${ORXONOX_VERSION})
  ENDIF()

  # Second part of precompiled header files
  IF(PCH_COMPILER_SUPPORT AND PCH_ENABLE_${_target_name_upper} AND _arg_PCH_FILE)
    PRECOMPILED_HEADER_FILES_POST_TARGET(${_target_name} ${_arg_PCH_FILE})
  ENDIF()

  IF(NOT _arg_STATIC AND NOT _arg_NO_INSTALL)
    INSTALL(TARGETS ${_target_name}
      RUNTIME DESTINATION ${ORXONOX_RUNTIME_INSTALL_PATH}
      LIBRARY DESTINATION ${ORXONOX_LIBRARY_INSTALL_PATH}
      #ARCHIVE DESTINATION ${ORXONOX_ARCHIVE_INSTALL_PATH}
    )
  ENDIF()

ENDFUNCTION(TU_ADD_TARGET)
