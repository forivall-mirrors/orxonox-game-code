MACRO(ADD_SOURCE_FILES _directory _source_list _target_list)

  # Set the variable (_source_list) first
  INCLUDE(${_directory}/CMakeLists.txt)
#MESSAGE(STATUS ${_directory})
#MESSAGE(STATUS "${_source_list}")
#MESSAGE(STATUS "${${_source_list}}")

  FOREACH(_source_file ${${_source_list}})
	LIST(APPEND ${_target_list} "${_directory}/${_source_file}")
#MESSAGE(STATUS ${${_target_list}})
  ENDFOREACH(_source_file)
ENDMACRO(ADD_SOURCE_FILES)
