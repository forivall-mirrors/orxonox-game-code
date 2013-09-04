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
 #

FUNCTION(GENERATE_BUILD_UNITS _target_name _all_files_var)
  SET(_source_files)
  SET(_total_file_count 0)

  # Count the number of actual C++ source files
  FOREACH(_file ${${_all_files_var}})
    # Only look at C++ source files
    IF(_file MATCHES "\\.(cpp|cc|cxx)$")
      # Some files might be marked as not to compile at all
      GET_SOURCE_FILE_PROPERTY(_skip1 ${_file} HEADER_FILE_ONLY)
      GET_SOURCE_FILE_PROPERTY(_skip2 ${_file} EXCLUDE_FROM_BUILD_UNITS)
      IF(NOT _skip1 AND NOT _skip2)
        GET_SOURCE_FILE_PROPERTY(_size ${_file} BUILD_UNIT_SIZE)
        IF(NOT _size)
          SET(_size 1)
        ENDIF()
        # Append file AND size to the list (like storing an std::pair)
        LIST(APPEND _source_files ${_file} ${_size})
        MATH(EXPR _total_file_count "${_total_file_count} + ${_size}")
        # Don't compile
        SET_SOURCE_FILES_PROPERTIES(${_file} PROPERTIES HEADER_FILE_ONLY TRUE)
      ENDIF()
    ENDIF()
  ENDFOREACH(_file)

  # Get number of build units we have to make. The default is NR_OF_BUILD_UNITS
  # However we can specify different values in a config file
  SET(_config ${BUILD_UNITS_CONFIG_${NR_OF_BUILD_UNITS}_THREADS})
  SET(_nr_of_units)
  IF(_config)
    LIST(FIND _config ${_target_name} _index)
    IF(NOT _index EQUAL -1)
      # Nr of build units is the next element in the list (we assume it exists)
      MATH(EXPR _index "${_index} + 1")
      LIST(GET _config ${_index} _nr_of_units)
    ENDIF()
  ENDIF()
  IF(NOT _nr_of_units)
    # Use default as specified (e.g. "full4" --> 4) or 1 for externals
    IF(_arg_ORXONOX_EXTERNAL)
      SET(_nr_of_units 1)
    ELSE()
      SET(_nr_of_units ${NR_OF_BUILD_UNITS})
    ENDIF()
  ENDIF()

  # Disable precompiled header files for targets with two or less build units
  IF(_nr_of_units LESS 3)
    SET(PCH_DISABLE_${_target_name} TRUE PARENT_SCOPE)
  ENDIF()

  SET(_remaining_files ${_total_file_count})
  SET(_remaining_units ${_nr_of_units})
  SET(_unit_nr 1)
  # Loop counts back from ${_nr_of_units} to 1
  FOREACH(_remaining_units RANGE ${_nr_of_units} 1 -1)
    # Use integer division to get the current build unit size
    MATH(EXPR _aimed_size "${_remaining_files} / ${_remaining_units}")

    SET(_current_size 0)
    SET(_current_unit)

    SET(_file_index 0)
    LIST(LENGTH _source_files _list_size)
    WHILE(${_file_index} LESS ${_list_size} AND NOT ${_current_size} EQUAL ${_aimed_size})
      # _source_files stores pairs of values (file followed by its size)
      MATH(EXPR _size_index "${_file_index} + 1")
      LIST(GET _source_files ${_file_index} _file)
      LIST(GET _source_files ${_size_index} _size)

      MATH(EXPR _new_size "${_current_size} + ${_size}")
      IF(${_new_size} GREATER ${_aimed_size})
        # Try next file in list (jump 2 because pairs are stored)
        MATH(EXPR _file_index "${_file_index} + 2")
      ELSE()
        SET(_current_size ${_new_size})
        LIST(APPEND _current_unit ${_file})
        # Remove from _source_files list
        LIST(REMOVE_AT _source_files ${_file_index} ${_size_index})
        MATH(EXPR _list_size "${_list_size} - 2")
      ENDIF()
    ENDWHILE()

    # Finalise
    LIST(LENGTH _current_unit _nr_of_included_files)
    IF(_nr_of_included_files EQUAL 1)
      # If unit consists of one file, we can compile it the old fashioned way
      SET_SOURCE_FILES_PROPERTIES(${_current_unit} PROPERTIES HEADER_FILE_ONLY FALSE)
    ELSEIF(_nr_of_included_files GREATER 1)
      # Assemble unit by writing some #include statements
      SET(_include_string)
      FOREACH(_file ${_current_unit})
        SET(_include_string "${_include_string}#include \"${_file}\"\n")
      ENDFOREACH(_file)

      # Generate the filename
      IF(NOT _nr_of_units EQUAL 1)
        SET(_suffix ${_unit_nr})
      ENDIF()
      SET(_unit_file ${CMAKE_CURRENT_BINARY_DIR}/${_target_name}BuildUnit${_suffix}.cc)
      # Only write if content has changed (avoids recompile)
      IF(EXISTS ${_unit_file})
        FILE(READ ${_unit_file} _file_include_string)
      ENDIF()
      IF(NOT _include_string STREQUAL "${_file_include_string}")
        FILE(WRITE ${_unit_file} "${_include_string}")
      ENDIF()

      LIST(APPEND _build_units ${_unit_file})

      # Increase file name counter
      MATH(EXPR _unit_nr "${_unit_nr} + 1")
    ENDIF()

    # Compute remaining files
    MATH(EXPR _remaining_files "${_remaining_files} - ${_current_size}")
  ENDFOREACH(_remaining_units)

  # Add units to list of source files (function, not macro --> parent scope)
  # Do this ONCE because parent scope changes will NOT be visible here
  SET(${_all_files_var} ${${_all_files_var}} ${_build_units} PARENT_SCOPE)

ENDFUNCTION(GENERATE_BUILD_UNITS)
