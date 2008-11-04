MACRO(ADD_SOURCE_DIRECTORY _target_list _directory)

  # Subfolder puts source files into CMake Cache variable _CACHED_SOURCE_FILES
  ADD_SUBDIRECTORY(${_directory})

  # Put the directory name in front of each source file from the subfolder
  # and add it to the source list in the current directory
  FOREACH(_source_file ${_CACHED_SOURCE_FILES})
    LIST(APPEND ${_target_list} "${_directory}/${_source_file}")
  ENDFOREACH(_source_file)

ENDMACRO(ADD_SOURCE_DIRECTORY)


MACRO(ADD_SOURCE_FILES _source_list)
  
  # Put the source file into a variable that still exists in this_folder/../
  # Use FORCE to always overwrite the cache variable
  SET(_CACHED_SOURCE_FILES ${${_source_list}} CACHE STRING "" FORCE)
  MARK_AS_ADVANCED(_CACHED_SOURCE_FILES FORCE)

ENDMACRO(ADD_SOURCE_FILES)
