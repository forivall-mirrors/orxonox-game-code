#    AddSourceFiles.cmake - CMake Module to include source files in subdirectories.
#    Author: Reto '1337' Grieder (2008)
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

# BIG FAT NOTE:
# There's possibly a bug in the CMake behaviour when using PARENT_SCOPE.
# It seems like the parent variable doesn't get updated locally but written
# correclty to the parent scope. So accessing a parent variable will always
# return its value BEFORE the local scope was created! Mind this when
# updating to a new CMake version.

# Adds source files to the internal handler
MACRO(ADD_SOURCE_FILES)
  
  # Write to parent scoped variables AND to our own scope
  # (Also see the big fat note at the beginning of the file)
  # --> _source_files_internal_parent stays constant here, not matter what!
  SET(_source_files_internal_local ${_source_files_internal_local} ${ARGN})
  SET(_source_files_internal_parent ${_source_files_internal_local} PARENT_SCOPE)

ENDMACRO(ADD_SOURCE_FILES)


# Adds a subdirectory to the internal souce file handler
MACRO(ADD_SOURCE_DIRECTORY _directory)

  # Save variable
  SET(_source_files_internal_temp ${_source_files_internal_local})
  # Clear the local variable because we use it in the child scope
  SET(_source_files_internal_local)

  # Subfolder puts source files into CMake variable _source_files_internal_parent
  ADD_SUBDIRECTORY(${_directory})
  # Recover our own local variable
  SET(_source_files_internal_local ${_source_files_internal_temp})

  # Put the directory name in front of each source file from the subfolder
  SET(_source_files_internal_temp)
  FOREACH(_source_file ${_source_files_internal_parent})
    LIST(APPEND _source_files_internal_temp "${_directory}/${_source_file}")
  ENDFOREACH(_source_file)

  # Add the content of the temporary list
  ADD_SOURCE_FILES(${_source_files_internal_temp})

ENDMACRO(ADD_SOURCE_DIRECTORY)

# Writes the content from the internal variables to a user specified one
MACRO(WRITE_SOURCE_FILES _variable_name)

  SET(${_variable_name} ${_source_files_internal_local})

ENDMACRO(WRITE_SOURCE_FILES)
