# - Find Tcl includes and libraries.
# This module finds if TCL is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#  TCL_LIBRARY        = path to Tcl library (tcl tcl80)
#  TCL_INCLUDE_PATH   = path to where tcl.h can be found

GET_FILENAME_COMPONENT(TCL_TCLSH_PATH "${TCL_TCLSH}" PATH)

SET (TCLTK_POSSIBLE_LIB_PATHS
  /usr/lib 
  /usr/local/lib
  "$ENV{ProgramFiles}/Tcl/Lib"
  "C:/Program Files/Tcl/lib" 
  "C:/Tcl/lib" 
  ../libs/tcl8.5.2/unix
  ../libs/tcl8.5.2/win
)

FIND_LIBRARY(TCL_LIBRARY
  NAMES tcl tcl85 tcl8.5 tcl84 tcl8.4 tcl83 tcl8.3 tcl82 tcl8.2 tcl80 tcl8.0
  PATHS ${TCLTK_POSSIBLE_LIB_PATHS}
)

GET_FILENAME_COMPONENT(TCL_LIBRARY_PATH "${TCL_LIBRARY}" PATH)

SET (TCLTK_POSSIBLE_INCLUDE_PATHS
  /usr/include
  /usr/local/include
  /usr/include/tcl8.4
  /usr/include/tcl8.3
  /usr/include/tcl8.2
  /usr/include/tcl8.0
  "$ENV{ProgramFiles}/Tcl/include"
  "C:/Program Files/Tcl/include"
  C:/Tcl/include
  ../libs/tcl8.5.2/generic
)

FIND_PATH(TCL_INCLUDE_PATH tcl.h
  ${TCL_FRAMEWORK_INCLUDES} ${TCLTK_POSSIBLE_INCLUDE_PATHS}
)

IF(TCL_INCLUDE_PATH)
  IF(TK_INCLUDE_PATH)
    IF(TCL_LIBRARY)
      IF(TK_LIBRARY)
        SET(TCL_FOUND 1)
      ENDIF(TK_LIBRARY)
    ENDIF(TCL_LIBRARY)
  ENDIF(TK_INCLUDE_PATH)
ENDIF(TCL_INCLUDE_PATH)
