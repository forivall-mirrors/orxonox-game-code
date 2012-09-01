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
 #    The output is then stored in "_arg_ARGNAME" where ARGNAME is the the
 #    name of the switch or list.
 #
 #    Switches: (when given --> TRUE, FALSE otherwise)
 #      FIND_HEADER_FILES: Searches the current directory for all header files
 #                         and adds them to the target.
 #      EXCLUDE_FROM_ALL:  Inherited from ADD_LIBRARY/ADD_EXECUTABLE
 #      ORXONOX_EXTERNAL:  Specify this for third party libraries
 #      NO_DLL_INTERFACE:  Link statically with MSVC
 #      NO_SOURCE_GROUPS:  Don't create msvc source groups
 #      MODULE:            For dynamic module libraries (libraries only)
 #      WIN32:             Inherited from ADD_EXECUTABLE (executables only)
 #      PCH_NO_DEFAULT:    Do not make precompiled header files default if
 #                         specified with PCH_FILE
 #      NO_INSTALL:        Do not install the target at all
 #      NO_VERSION:        Prevents adding any version to a target
 #      NO_BUILD_UNITS:    Disables automatic (full) build units
 #
 #    Lists:
 #      LINK_LIBRARIES:    Redirects to TARGET_LINK_LIBRARIES
 #      LINK_LIBS_LINUX:   Redirects to TARGET_LINK_LIBRARIES only on Linux
 #      LINK_LIBS_WIN32:   Redirects to TARGET_LINK_LIBRARIES only on Windows
 #      LINK_LIBS_APPLE:   Redirects to TARGET_LINK_LIBRARIES only on Apple
 #      LINK_LIBS_UNIX:    Redirects to TARGET_LINK_LIBRARIES only on UNIX
 #      VERSION:           Set version to the binary
 #      SOURCE_FILES:      Source files for the target
 #      DEFINE_SYMBOL:     Sets the DEFINE_SYMBOL target property
 #      TOLUA_FILES:       Files with tolua interface
 #      PCH_FILE:          Precompiled header file
 #      PCH_EXCLUDE:       Source files to be excluded from PCH support
 #      OUTPUT_NAME:       If you want a different name than the target name
 #      EXCLUDE_FROM_BUILD_UNITS: Specifies files that are not put into
 #                         automatic (full) build units. They can still
 #                         explicitely be included in a BUILD_UNIT (partial)
 #  Note:
 #    This function also installs the target!
 #  Prerequisistes:
 #    ORXONOX_DEFAULT_LINK
 #  Parameters:
 #    _target_name, ARGN for the macro arguments
 #

INCLUDE(BuildUnits)
INCLUDE(CMakeDependentOption)
INCLUDE(CapitaliseName)
INCLUDE(GenerateToluaBindings)
INCLUDE(ParseMacroArguments)
INCLUDE(SourceFileUtilities)
IF(PCH_COMPILER_SUPPORT)
  INCLUDE(PrecompiledHeaderFiles)
ENDIF()

MACRO(ORXONOX_ADD_LIBRARY _target_name)
  TU_ADD_TARGET(${_target_name} LIBRARY "MODULE" ${ARGN})
ENDMACRO(ORXONOX_ADD_LIBRARY)

MACRO(ORXONOX_ADD_EXECUTABLE _target_name)
  TU_ADD_TARGET(${_target_name} EXECUTABLE "WIN32" ${ARGN})
ENDMACRO(ORXONOX_ADD_EXECUTABLE)


MACRO(TU_ADD_TARGET _target_name _target_type _additional_switches)
  CAPITALISE_NAME(${_target_name} _target_name_capitalised)
  STRING(TOUPPER "${_target_name}" _target_name_upper)

  # Specify all possible options (either switch or with add. arguments)
  SET(_switches   FIND_HEADER_FILES  EXCLUDE_FROM_ALL  ORXONOX_EXTERNAL
                  NO_DLL_INTERFACE   NO_SOURCE_GROUPS  PCH_NO_DEFAULT 
                  NO_INSTALL         NO_VERSION        NO_BUILD_UNITS
                  ${_additional_switches})
  SET(_list_names LINK_LIBRARIES     VERSION           SOURCE_FILES
                  DEFINE_SYMBOL      TOLUA_FILES       PCH_FILE
                  PCH_EXCLUDE        OUTPUT_NAME       LINK_LIBS_LINUX
                  LINK_LIBS_WIN32    LINK_LIBS_APPLE   LINK_LIBS_UNIX
                  EXCLUDE_FROM_BUILD_UNITS)

  PARSE_MACRO_ARGUMENTS("${_switches}" "${_list_names}" ${ARGN})

  # Process source files with support for build units
  # Note: All file paths are relative to the root source directory, even the
  #       name of the build unit.
  SET(_${_target_name}_source_files)
  SET(_get_build_unit_file FALSE)
  SET(_add_to_build_unit FALSE)
  FOREACH(_file ${_arg_SOURCE_FILES})
    IF(_file STREQUAL "BUILD_UNIT")
      # Next file is the name of the build unit
      SET(_get_build_unit_file TRUE)
    ELSEIF(_file STREQUAL "END_BUILD_UNIT")
      IF(NOT _build_unit_file)
        MESSAGE(FATAL_ERROR "No name provided for build unit")
      ENDIF()
      IF(ENABLE_BUILD_UNITS)
        IF(NOT _build_unit_include_string)
          MESSAGE(STATUS "Warning: Empty build unit!")
        ENDIF()
        IF(EXISTS ${_build_unit_file})
          FILE(READ ${_build_unit_file} _include_string_file)
        ENDIF()
        IF(NOT _build_unit_include_string STREQUAL "${_include_string_file}")
          FILE(WRITE ${_build_unit_file} "${_build_unit_include_string}")
        ENDIF()
        LIST(APPEND _${_target_name}_source_files ${_build_unit_file})
        LIST(APPEND _${_target_name}_build_units ${_build_unit_file})
        # Store the number of files included. May be used for full build units.
        SET_SOURCE_FILES_PROPERTIES(${_build_unit_file}
          PROPERTIES BUILD_UNIT_SIZE "${_build_unit_count}")
      ENDIF()
      SET(_add_to_build_unit FALSE)
    ELSEIF(_get_build_unit_file)
      # Note: ${_file} is relative to the binary directory
      SET(_build_unit_file ${CMAKE_BINARY_DIR}/${_file})
      SET(_get_build_unit_file FALSE)
      SET(_add_to_build_unit TRUE)
      SET(_build_unit_include_string)
      SET(_build_unit_count "0")
    ELSE()
      # Default, add source file

      # Prepare relative paths
      IF(NOT _file MATCHES "^(.\\:|\\/)")
        # Path can be relative to the current source directory if the file was
        # not added with the source file macros. Otherwise there is a "./" at
        # the beginning of each file and the filename is relative
        # to the CMAKE_SOURCE_DIR
        STRING(REGEX REPLACE "^\\.\\/(.+)$" "\\1" _temp ${_file})
        IF(NOT ${_temp} STREQUAL ${_file})
          SET(_file ${CMAKE_SOURCE_DIR}/${_temp})
        ELSE()
          SET(_file ${CMAKE_CURRENT_SOURCE_DIR}/${_file})
        ENDIF()
      ENDIF()

      LIST(APPEND _${_target_name}_source_files ${_file})

      # Handle build units
      IF(_add_to_build_unit AND ENABLE_BUILD_UNITS)
        IF(_file MATCHES "\\.(c|cc|cpp|cxx)$")
          SET(_build_unit_include_string "${_build_unit_include_string}#include \"${_file}\"\n")
          MATH(EXPR _build_unit_count "1 + ${_build_unit_count}")
        ENDIF()
        # Don't compile these files, even if they are source files
        SET_SOURCE_FILES_PROPERTIES(${_file} PROPERTIES HEADER_FILE_ONLY TRUE)
      ENDIF()
    ENDIF()
  ENDFOREACH(_file)

  # Assemble all header files of the library
  IF(_arg_FIND_HEADER_FILES)
    GET_ALL_HEADER_FILES(_${_target_name}_header_files)
  ENDIF()

  # Combine source and header files
  SET(_${_target_name}_files
    ${_${_target_name}_header_files}
    ${_${_target_name}_source_files}
  )
  # Remove potential duplicates
  LIST(REMOVE_DUPLICATES _${_target_name}_files)

  # TOLUA_FILES
  IF(_arg_TOLUA_FILES)
    GENERATE_TOLUA_BINDINGS(${_target_name_capitalised} _${_target_name}_files
                            INPUTFILES ${_arg_TOLUA_FILES})
    # Workaround for XCode: The folder where the bind files are written to has
    # to be present beforehand.
    IF(CMAKE_CONFIGURATION_TYPES)
      FOREACH(_dir ${CMAKE_CONFIGURATION_TYPES})
        FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${_dir})
      ENDFOREACH(_dir)
    ENDIF()
  ENDIF()

  # Mark files to be excluded from build units
  IF(_arg_EXCLUDE_FROM_BUILD_UNITS)
    SET_SOURCE_FILES_PROPERTIES(${_arg_EXCLUDE_FROM_BUILD_UNITS}
      PROPERTIES EXCLUDE_FROM_BUILD_UNITS TRUE)
  ENDIF()

  # Full build units
  IF(ENABLE_BUILD_UNITS AND NOT _arg_NO_BUILD_UNITS)
    # Use full build units even in partial mode for externals
    IF(ENABLE_BUILD_UNITS MATCHES "full" OR _arg_ORXONOX_EXTERNAL)
      GENERATE_BUILD_UNITS(${_target_name} _${_target_name}_files)
    ENDIF()
  ENDIF()

  # First part (pre target) of precompiled header files
  IF(PCH_COMPILER_SUPPORT AND _arg_PCH_FILE)
    # Provide convenient option to control PCH
    IF(_arg_PCH_NO_DEFAULT)
      SET(PCH_DEFAULT FALSE)
    ELSE()
      SET(PCH_DEFAULT TRUE)
    ENDIF()
    CMAKE_DEPENDENT_OPTION(PCH_ENABLE_${_target_name_upper}
      "Enable using precompiled header files for library ${_target_name}." ${PCH_DEFAULT} PCH_ENABLE OFF)
    # Almost never used individually, but produces a lot of options --> hide
    MARK_AS_ADVANCED(PCH_ENABLE_${_target_name_upper})

    IF(PCH_ENABLE_${_target_name_upper} AND NOT PCH_DISABLE_${_target_name})
      PRECOMPILED_HEADER_FILES_PRE_TARGET(${_target_name} ${_arg_PCH_FILE} _${_target_name}_files EXCLUDE ${_arg_PCH_EXCLUDE})
    ENDIF()
  ENDIF()

  # Generate the source groups
  IF(MSVC AND NOT _arg_NO_SOURCE_GROUPS)
    GENERATE_SOURCE_GROUPS(${_${_target_name}_files})

    IF(NOT _arg_ORXONOX_EXTERNAL)
      # Move the ...Prereqs.h and the PCH files to the 'Config' section
      IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_target_name_capitalised}Prereqs.h)
        SOURCE_GROUP("Config" FILES ${_target_name_capitalised}Prereqs.h)
      ENDIF()
      IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_arg_PCH_FILE})
        SOURCE_GROUP("Config" FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_arg_PCH_FILE})
      ENDIF()
    ENDIF()
  ENDIF()

  # Set link mode (SHARED/STATIC)
  IF(MSVC AND _arg_NO_DLL_INTERFACE)
    # Certain libraries don't have dllexport/dllimport and can't be linked shared with MSVC
    SET(_link_mode STATIC)
  ELSEIF(_arg_ORXONOX_EXTERNAL)
    # Externals can be linked shared or statically
    SET(_link_mode ${ORXONOX_EXTERNAL_LINK_MODE})
  ELSE()
    # All our own libraries are linked dynamically because of static symbols
    SET(_link_mode SHARED)
  ENDIF()

  # No warnings needed from third party libraries
  IF(_arg_ORXONOX_EXTERNAL)
    REMOVE_COMPILER_FLAGS("-W3 -W4" MSVC)
    ADD_COMPILER_FLAGS("-w" NOT MSVC)
    ADD_COMPILER_FLAGS("-W0" MSVC)
  ENDIF()

  # Don't compile header files
  FOREACH(_file ${_${_target_name}_files})
    IF(NOT _file MATCHES "\\.(c|cc|cpp|cxx|mm)$")
      SET_SOURCE_FILES_PROPERTIES(${_file} PROPERTIES HEADER_FILE_ONLY TRUE)
    ENDIF()
  ENDFOREACH(_file)



  # Add the library/executable
  IF("${_target_type}" STREQUAL "LIBRARY")
    ADD_LIBRARY(${_target_name} ${_link_mode}
                ${_arg_EXCLUDE_FROM_ALL} ${_${_target_name}_files})
  ELSE()
    ADD_EXECUTABLE(${_target_name} ${_arg_WIN32} ${_arg_EXCLUDE_FROM_ALL}
                   ${_${_target_name}_files})
  ENDIF()



  # Change library prefix to "lib"
  IF(MSVC AND ${_target_type} STREQUAL "LIBRARY")
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES
      PREFIX "lib"
    )
  ENDIF()

  # MSVC hack to exclude external library sources from the intellisense database
  # (IntelliSense stops working when adding "-Zm1000" as compile flag. "/Zm1000"
  # would not work because of the slash)
  IF(_arg_ORXONOX_EXTERNAL AND MSVC)
    GET_TARGET_PROPERTY(_compile_flags ${_target_name} COMPILE_FLAGS)
    IF(NOT _compile_flags)
      SET(_compile_flags)
    ENDIF()
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES COMPILE_FLAGS "${_compile_flags} -Zm1000")
  ENDIF()

  # Configure modules
  IF (_arg_MODULE)
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_MODULE_OUTPUT_DIRECTORY} # Windows
      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_MODULE_OUTPUT_DIRECTORY} # Unix
    )
    ADD_MODULE(${_target_name})
    # Ensure that the main program depends on the module
    SET(ORXONOX_MODULES ${ORXONOX_MODULES} ${_target_name} CACHE INTERNAL "")
  ENDIF()

  # Static library flags are not globally available
  IF(ORXONOX_STATIC_LINKER_FLAGS)
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES STATIC_LIBRARY_FLAGS ${ORXONOX_STATIC_LINKER_FLAGS})
  ENDIF()

  # LINK_LIBRARIES
  IF(_arg_LINK_LIBRARIES)
    TARGET_LINK_LIBRARIES(${_target_name} ${_arg_LINK_LIBRARIES})
  ENDIF()
  IF(_arg_LINK_LIBS_LINUX AND LINUX)
    TARGET_LINK_LIBRARIES(${_target_name} ${_arg_LINK_LIBS_LINUX})
  ENDIF()
  IF(_arg_LINK_LIBS_WIN32 AND WIN32)
    TARGET_LINK_LIBRARIES(${_target_name} ${_arg_LINK_LIBS_WIN32})
  ENDIF()
  IF(_arg_LINK_LIBS_APPLE AND APPLE)
    TARGET_LINK_LIBRARIES(${_target_name} ${_arg_LINK_LIBS_APPLE})
  ENDIF()
  IF(_arg_LINK_LIBS_UNIX AND UNIX)
    TARGET_LINK_LIBRARIES(${_target_name} ${_arg_LINK_LIBS_UNIX})
  ENDIF()

  # Visual Leak Detector specific stuff (avoids the include)
  IF(VISUAL_LEAK_DETECTOR_ENABLE)
    TARGET_LINK_LIBRARIES(${_target_name} debug ${VLD_LIBRARY})
  ENDIF()

  # RPATH settings for the installation
  IF("${_target_type}" STREQUAL "LIBRARY")
    IF(_arg_MODULE)
      SET(_rpath "${MODULE_RPATH}")
    ELSE()
      SET(_rpath "${LIBRARY_RPATH}")
    ENDIF()
  ELSE()
    SET(_rpath "${RUNTIME_RPATH}")
  ENDIF()
  SET_TARGET_PROPERTIES(${_target_name} PROPERTIES INSTALL_RPATH "${_rpath}")

  # DEFINE_SYMBOL
  IF(_arg_DEFINE_SYMBOL)
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES DEFINE_SYMBOL ${_arg_DEFINE_SYMBOL})
  ELSEIF(NOT _arg_ORXONOX_EXTERNAL)
    # Automatically add the macro definitions for our own libraries
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES DEFINE_SYMBOL "${_target_name_upper}_SHARED_BUILD")
  ENDIF()

  # VERSION
  IF(_arg_VERSION)
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES VERSION ${_arg_VERSION})
  ELSEIF(NOT _arg_ORXONOX_EXTERNAL AND NOT _arg_NO_VERSION AND NOT ${_target_type} STREQUAL "EXECUTABLE")
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES VERSION ${ORXONOX_VERSION})
  ENDIF()

  # OUTPUT_NAME
  IF(_arg_OUTPUT_NAME)
    SET_TARGET_PROPERTIES(${_target_name} PROPERTIES OUTPUT_NAME  ${_arg_OUTPUT_NAME})
  ENDIF()

  # Second part of precompiled header files
  IF(PCH_COMPILER_SUPPORT AND PCH_ENABLE_${_target_name_upper} AND _arg_PCH_FILE AND NOT PCH_DISABLE_${_target_name})
    PRECOMPILED_HEADER_FILES_POST_TARGET(${_target_name} ${_arg_PCH_FILE})
  ENDIF()

  # Install all targets except for static ones (executables also have SHARED in _link_mode)
  IF(${_link_mode} STREQUAL "SHARED" AND NOT _arg_NO_INSTALL)
    IF(_arg_MODULE)
      INSTALL(TARGETS ${_target_name}
        RUNTIME DESTINATION ${MODULE_INSTALL_DIRECTORY}
        LIBRARY DESTINATION ${MODULE_INSTALL_DIRECTORY}
      )
    ELSE()
      INSTALL(TARGETS ${_target_name}
        RUNTIME DESTINATION ${RUNTIME_INSTALL_DIRECTORY}
        LIBRARY DESTINATION ${LIBRARY_INSTALL_DIRECTORY}
      )
    ENDIF()
    IF(INSTALL_PDB_FILES) # MSVC specific: install debug symbols files
      FOREACH(_config RelForDevs RelWithDebInfo)
        GET_TARGET_PROPERTY(_location ${_target_name} LOCATION_${_config})
        # Get absolute location without dll/exe extension
        STRING(REGEX REPLACE "^(.+)\\.(dll|exe)$" "\\1" _location_we ${_location})
        IF(_arg_MODULE)
          INSTALL(FILES ${_location_we}.pdb
            DESTINATION ${MODULE_INSTALL_DIRECTORY}
            CONFIGURATIONS ${_config}
          )
        ELSE()
          INSTALL(FILES ${_location_we}.pdb
            DESTINATION ${RUNTIME_INSTALL_DIRECTORY}
            CONFIGURATIONS ${_config}
          )
        ENDIF()
      ENDFOREACH(_config)
    ENDIF()
  ENDIF()

ENDMACRO(TU_ADD_TARGET)


# Creates a helper file with name <name_of_the_library>${ORXONOX_MODULE_EXTENSION}
# This helps finding dynamically loadable modules at runtime

FUNCTION(ADD_MODULE _target)
  # We use the properties to get the name because the librarys name may differ from
  # the target name (for example orxonox <-> liborxonox)

  GET_TARGET_PROPERTY(_target_loc ${_target} LOCATION)
  GET_FILENAME_COMPONENT(_target_name ${_target_loc} NAME_WE)

  IF(CMAKE_CONFIGURATION_TYPES)
    FOREACH(_config ${CMAKE_CONFIGURATION_TYPES})
      SET(_module_filename ${CMAKE_MODULE_OUTPUT_DIRECTORY}/${_config}/${_target_name}${ORXONOX_MODULE_EXTENSION})

      FILE(WRITE ${_module_filename})

      INSTALL(
        FILES ${_module_filename}
        DESTINATION ${MODULE_INSTALL_DIRECTORY}
        CONFIGURATIONS ${_config}
      )
    ENDFOREACH()
  ELSE()
    SET(_module_filename ${CMAKE_MODULE_OUTPUT_DIRECTORY}/${_target_name}${ORXONOX_MODULE_EXTENSION})

    FILE(WRITE ${_module_filename})

    INSTALL(
      FILES ${_module_filename}
      DESTINATION ${MODULE_INSTALL_DIRECTORY}
    )
  ENDIF()
ENDFUNCTION(ADD_MODULE)
