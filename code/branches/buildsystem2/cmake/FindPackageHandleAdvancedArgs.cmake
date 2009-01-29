INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(CompareVersionStrings)

FUNCTION(FIND_PACKAGE_HANDLE_ADVANCED_ARGS _name _fail_msg _version)

  # Modify the message to include version requirements
  IF("${_fail_msg}" STREQUAL "DEFAULT_MSG")
    SET(_fail_msg_adv "Could NOT find ${_name}")
  ELSE("${_fail_msg}" STREQUAL "DEFAULT_MSG")
    SET(_fail_msg_adv "${_fail_msg}")
  ENDIF("${_fail_msg}" STREQUAL "DEFAULT_MSG")
  IF(${_name}_FIND_VERSION_EXACT)
    SET(_fail_msg_adv "${_fail_msg_adv} (version requirements: exactly ${${_name}_FIND_VERSION}")
  ELSE(${_name}_FIND_VERSION_EXACT)
    SET(_fail_msg_adv "${_fail_msg_adv} (version requirements: at least ${${_name}_FIND_VERSION}")
  ENDIF(${_name}_FIND_VERSION_EXACT)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS("${_name}" "${_fail_msg_adv}" ${ARGN})
  STRING(TOUPPER ${_name} _NAME_UPPER)

  # Also check version requirements if given to FindPackage(...)
  IF(${_name}_FIND_VERSION)
    # Note: the last argument of the function tells it to cut the first
    #       version string instead of padding it with zeros if necessary
    COMPARE_VERSION_STRINGS("${_version}" "${${_name}_FIND_VERSION}" _compatible TRUE)
    IF(${_name}_FIND_VERSION_EXACT AND NOT _compatible EQUAL 0)
      MESSAGE(FATAL_ERROR "Exact ${_name} version required is ${${_name}_FIND_VERSION}\n"
                          "Your version is ${_version}")
      SET(${_NAME_UPPER}_FOUND FALSE)
    ELSEIF(_compatible LESS 0)
      MESSAGE(FATAL_ERROR "Minimum ${_name} version required is ${${_name}_FIND_VERSION}\n"
                          "Your version is ${_version}")
      SET(${_NAME_UPPER}_FOUND FALSE)
    ENDIF(${_name}_FIND_VERSION_EXACT AND NOT _compatible EQUAL 0)
  ENDIF(${_name}_FIND_VERSION)

  # Raise scope (FindPackageHandleStandardArgs uses PARENT_SCOPE)
  SET(${_NAME_UPPER}_FOUND ${${_NAME_UPPER}_FOUND} PARENT_SCOPE)

ENDFUNCTION(FIND_PACKAGE_HANDLE_ADVANCED_ARGS)
