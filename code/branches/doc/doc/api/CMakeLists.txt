# Add a "doc" target with CMake build system.
# And configure doxy.config.in to doxy.config
#
# target "doc" allows building the documentation with doxygen/dot on WIN32 and Linux
# Creates .chm windows help file if MS HTML help workshop
# (available from http://msdn.microsoft.com/workshop/author/htmlhelp)
# is installed with its DLLs in PATH.
#
#
# Please note, that the tools, e.g.:
# doxygen, dot, latex, dvips, makeindex, gswin32, etc.
# must be in path.
#
# Note about Visual Studio Projects:
# MSVS hast its own path environment which may differ from the shell.
# See "Menu Tools/Options/Projects/VC++ Directories" in VS 7.1
#
# author Jan Woetzel 2004-2006
# www.mip.informatik.uni-kiel.de/~jw
# Adjustments for Orxonox by Reto Grieder

FIND_PACKAGE(Doxygen QUIET)

IF (DOXYGEN_FOUND)
  # click+jump in Emacs and Visual Studio (for doxy.config) (jw)
  IF(CMAKE_BUILD_TOOL MATCHES "msdev|devenv")
    SET(DOXY_WARN_FORMAT "\"$file($line) : $text \"")
  ELSE()
    SET(DOXY_WARN_FORMAT "\"$file:$line: $text \"")
  ENDIF()

  SET(DOXY_INPUT_DIR  ${CMAKE_CURRENT_SOURCE_DIR})
  SET(DOXY_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

  # we need latex for doxygen because of the formulas
  # Orxonox doesn't
  #FIND_PACKAGE(LATEX)
  #IF    (NOT LATEX_COMPILER)
  #  MESSAGE(STATUS "latex command LATEX_COMPILER not found but usually required. You will probably get warnings and user inetraction on doxy run.")
  #ENDIF (NOT LATEX_COMPILER)
  #IF    (NOT MAKEINDEX_COMPILER)
  #  MESSAGE(STATUS "makeindex command MAKEINDEX_COMPILER not found but usually required.")
  #ENDIF (NOT MAKEINDEX_COMPILER)
  #IF    (NOT DVIPS_CONVERTER)
  #  MESSAGE(STATUS "dvips command DVIPS_CONVERTER not found but usually required.")
  #ENDIF (NOT DVIPS_CONVERTER)

  SET(INTERNAL_DOCFILE ${CMAKE_CURRENT_BINARY_DIR}/Internal.dox)
  IF(WIN32)
    SET(_working_dir ${RUNTIME_LIBRARY_DIRECTORY})
  ELSE()
    SET(_working_dir ${CMAKE_CURRENT_BINARY_DIR})
  ENDIF()
  ADD_CUSTOM_COMMAND(
    OUTPUT ${INTERNAL_DOCFILE}
    COMMAND orxonox-main
    ARGS --noIOConsole --generateDoc ${INTERNAL_DOCFILE}
    WORKING_DIRECTORY ${_working_dir}
    COMMENT "Generating additional Doxygen documentation from Orxonox executable"
  )
  ADD_CUSTOM_TARGET(doc_internal DEPENDS ${INTERNAL_DOCFILE})
  IF (NOT EXISTS ${INTERNAL_DOCFILE})
    FILE(WRITE ${INTERNAL_DOCFILE} "// empty...")
  ENDIF()

  # Take care of most of the conditional compilation problems
  # (Doxygen requires separate input for that)
  IF(WIN32)
    SET(DOXY_PLATFORM_DEF "ORXONOX_PLATFORM_WINDOWS")
  ELSEIF(APPLE)
    SET(DOXY_PLATFORM_DEF "ORXONOX_PLATFORM_APPLE ORXONOX_PLATFORM_UNIX")
  ELSE()
    SET(DOXY_PLATFORM_DEF "ORXONOX_PLATFORM_LINUX ORXONOX_PLATFORM_UNIX")
  ENDIF()
  IF(MSVC)
    SET(DOXY_COMPILER_DEF "ORXONOX_COMPILER_MSVC")
  ELSE()
    SET(DOXY_COMPILER_DEF "ORXONOX_COMPILER_GCC")
  ENDIF()

  IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in)
    # use (configured) doxy.config from (out of place) BUILD tree:
    SET(DOXY_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/doxy.config)
    SET(DOXY_LOGFILE ${CMAKE_CURRENT_BINARY_DIR}/doxy.log)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in ${CMAKE_CURRENT_BINARY_DIR}/doxy.config @ONLY)
  ELSE()
    MESSAGE(FATAL_ERROR "Warning: Could not find dox.config.in in the root directory.")
  ENDIF()

  ADD_CUSTOM_TARGET(doc ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG})

  # create a windows help .chm file using hhc.exe
  # HTMLHelp DLL must be in path!
  # fallback: use hhw.exe interactively
  IF(WIN32)
    FIND_PACKAGE(HTMLHelp)
    IF(HTML_HELP_COMPILER)
      SET(TMP ${DOXY_OUTPUT_DIR}/html/index.hhp)
      STRING(REGEX REPLACE "/" "\\\\" HHP_FILE ${TMP})
      ADD_CUSTOM_TARGET(doc_chm ${HTML_HELP_COMPILER} ${HHP_FILE})
      ADD_DEPENDENCIES(doc_chm doc)
      # Adding a dependency somehow adds doc target as default build target
      SET_TARGET_PROPERTIES(doc doc_chm PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)
    ENDIF(HTML_HELP_COMPILER)
  ENDIF (WIN32)


  # Installation
  INSTALL(
    FILES ${DOXY_CONFIG} ${CMAKE_CURRENT_BINARY_DIR}/doxy.log
    DESTINATION ${DOC_INSTALL_DIRECTORY}/api OPTIONAL
  )
  # Install command always needs the directory
  IF(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/html)
    FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html)
  ENDIF()
  INSTALL(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
    DESTINATION ${DOC_INSTALL_DIRECTORY}/api
  )
ENDIF(DOXYGEN_FOUND)
