# - Find Tcl includes and libraries.
# This module finds if TCL is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#  TCL_LIBRARY        = path to Tcl library (tcl tcl80)
#  TCL_LIBRARY_DEBUG  = path to Tcl library (debug)
#  TCL_INCLUDE_PATH   = path to where tcl.h can be found
#
# Several changes and additions by Fabian 'x3n' Landau
#                 > www.orxonox.net <

IF (TCL_LIBRARY AND TCL_INCLUDE_PATH)
  SET(TCL_FIND_QUIETLY TRUE)
ENDIF (TCL_LIBRARY AND TCL_INCLUDE_PATH)

SET (TCLTK_POSSIBLE_LIB_PATHS
  "${TCL_TCLSH_PATH}/../lib"
  "${TK_WISH_PATH}/../lib"
  "${TCL_INCLUDE_PATH}/../lib"
  "${TK_INCLUDE_PATH}/../lib"
  "$ENV{ProgramFiles}/Tcl/Lib"
  "C:/Program Files/Tcl/lib" 
  "C:/Tcl/lib" 
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Scriptics\\Tcl\\8.5;Root]/lib
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Scriptics\\Tcl\\8.4;Root]/lib
  /usr/lib 
  /usr/local/lib
  /usr/pack/tcltk-8.4.9.linux-mo/lib
  ../libs/tcl8.5.2/win
  ../libs/tcl8.5.2/unix
  ../libs/tcl8.5.2/macosx
  ${DEPENDENCY_DIR}/tcl-8.5.2/lib
)

FIND_LIBRARY(TCL_LIBRARY
  NAMES tcl tcl85 tcl8.5 tcl84 tcl8.4 tcl85t
  PATHS ${TCLTK_POSSIBLE_LIB_PATHS}
)

FIND_LIBRARY(TCL_LIBRARY_DEBUG
  NAMES tcld tcl85d tcl8.5d tcl84d tcl8.4d
  PATHS ${TCLTK_POSSIBLE_LIB_PATHS}
)

SET (TCLTK_POSSIBLE_INCLUDE_PATHS
  ${TCL_TCLSH_PATH}/../include
  ${TK_WISH_PATH}/../include
  "${TCL_LIBRARY_PATH}/../include"
  "${TK_LIBRARY_PATH}/../include"
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Scriptics\\Tcl\\8.5;Root]/include
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Scriptics\\Tcl\\8.4;Root]/include
  "$ENV{ProgramFiles}/Tcl/include"
  "C:/Program Files/Tcl/include"
  C:/Tcl/include
  /usr/include
  /usr/local/include
  /usr/include/tcl8.5
  /usr/include/tcl8.4
  ../libs/tcl8.5.2/generic
  ${DEPENDENCY_DIR}/tcl-8.5.2/include
)

FIND_PATH(TCL_INCLUDE_PATH tcl.h
  ${TCL_FRAMEWORK_INCLUDES} ${TCLTK_POSSIBLE_INCLUDE_PATHS}
)


IF (TCL_LIBRARY AND TCL_INCLUDE_PATH)
  IF (NOT TCL_FIND_QUIETLY)
    MESSAGE(STATUS "Tcl was found")
    IF (VERBOSE_FIND)
      MESSAGE (STATUS "  include path: ${TCL_INCLUDE_PATH}")
      MESSAGE (STATUS "  library path: ${TCL_LIBRARY}")
      MESSAGE (STATUS "  libraries:    tcld tcl85d tcl8.5d tcl84d tcl8.4d")
    ENDIF (VERBOSE_FIND)
  ENDIF (NOT TCL_FIND_QUIETLY)
ELSE (TCL_LIBRARY AND TCL_INCLUDE_PATH)
  IF (NOT TCL_INCLUDE_PATH)
    MESSAGE(SEND_ERROR "Tcl include path was not found.")
  ENDIF (NOT TCL_INCLUDE_PATH)
  IF (NOT TCL_LIBRARY )
    MESSAGE(SEND_ERROR "Tcl library was not found.")
  ENDIF (NOT TCL_LIBRARY )
ENDIF (TCL_LIBRARY AND TCL_INCLUDE_PATH)

IF(TCL_INCLUDE_PATH)
  IF(TCL_LIBRARY)
    SET(TCL_FOUND 1)
  ENDIF(TCL_LIBRARY)
ENDIF(TCL_INCLUDE_PATH)
