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
 #    Adrian Friedli
 #  Description:
 #    Sets the CMake options that will generate Tolua++ bindings.
 #  Parameters:
 #    _tolua_package - Name of the package, e.g. "Core"
 #    _target_source_files - Variable name of the target source files, the
 #                           script will add the generated files to this list.
 #    ARGN - The header files in the style "INTPUTFILES Foo.h Bar.h"
 #  Global Variables: (need to be set before)
 #    TOLUA_PARSER_SOURCE - Lua file with the parser source code
 #    TOLUA_PARSER_DEPENDENCIES - All the dependent lua files
 #    RUNTIME_LIBRARY_DIRECTORY - Working directory
 #

# Workaround for XCode: The folder where the bind files are written to has
# to be present beforehand.
# We have to do this here because the header files are all stored in a single
# location per configuration.
IF(CMAKE_CONFIGURATION_TYPES)
  FOREACH(_dir ${CMAKE_CONFIGURATION_TYPES})
    FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/src/toluabind/${_dir}")
  ENDFOREACH(_dir)
ENDIF()

FUNCTION(GENERATE_TOLUA_BINDINGS _tolua_package _target_source_files)
  SET(_tolua_inputfiles ${ARGN})
  LIST(REMOVE_ITEM _tolua_inputfiles "INPUTFILES")

  SET(_tolua_pkgfile "${CMAKE_CURRENT_BINARY_DIR}/tolua.pkg")
  SET(_tolua_cxxfile "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/ToluaBind${_tolua_package}.cc")
  #SET(_tolua_hfile   "${CMAKE_BINARY_DIR}/src/toluabind/${CMAKE_CFG_INTDIR}/ToluaBind${_tolua_package}.h")

  SET(${_target_source_files}
    ${${_target_source_files}}
    ${_tolua_cxxfile}
    PARENT_SCOPE
  )

  IF(CMAKE_COMPILER_IS_GNU)
    # Disable annoying GCC warnings
    SET_SOURCE_FILES_PROPERTIES(${_tolua_cxxfile} PROPERTIES COMPILE_FLAGS "-w")
  ENDIF()

  IF(MSVC)
    # Including the file in a build unit is impossible because CMAKE_CFG_INTDIR
    # exands to an expression that the compiler doesn't understand
    SET_SOURCE_FILES_PROPERTIES(${_tolua_cxxfile} PROPERTIES EXCLUDE_FROM_BUILD_UNITS TRUE)
  ENDIF()

  # Create temporary package file and implicit dependencies
  FILE(REMOVE ${_tolua_pkgfile})
  FOREACH(_tolua_inputfile ${_tolua_inputfiles})
    FILE(APPEND ${_tolua_pkgfile} "\$cfile \"${_tolua_inputfile}\"\n")
    LIST(APPEND _implicit_dependencies CXX ${_tolua_inputfile})
  ENDFOREACH(_tolua_inputfile)

  IF(TOLUA_PARSER_HOOK_SCRIPT)
    # Hook scripts may contain functions that act as Tolua hooks
    SET(_hook_script -L "${TOLUA_PARSER_HOOK_SCRIPT}")
  ENDIF()

  ADD_CUSTOM_COMMAND(
    OUTPUT ${_tolua_cxxfile}
    COMMAND toluaapp_orxonox -n ${_tolua_package}
                             -w ${CMAKE_CURRENT_SOURCE_DIR}
                             -o ${_tolua_cxxfile}
                             -s ${TOLUA_PARSER_SOURCE}
                                ${_hook_script}
                                ${_tolua_pkgfile}
    DEPENDS           ${TOLUA_PARSER_DEPENDENCIES}
    IMPLICIT_DEPENDS  ${_implicit_dependencies}
    WORKING_DIRECTORY ${RUNTIME_LIBRARY_DIRECTORY}
    COMMENT "Generating tolua bind files for package ${_tolua_package}"
  )
ENDFUNCTION(GENERATE_TOLUA_BINDINGS)
