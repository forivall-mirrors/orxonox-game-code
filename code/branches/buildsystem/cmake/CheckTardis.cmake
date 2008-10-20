################# Tardis check ##################
# This code checks whether the current machine  #
# is an ETH tardis box.                         #
#################################################

# only run this test on a lunix/unix machine
IF (UNIX)

  FIND_PROGRAM(UNAME_CMD "uname"
  PATHS "/usr/bin /bin")
  IF(NOT UNAME_CMD)
    MESSAGE(ERROR "Unable to find uname. Tardis-Check cannot be done.")
  ENDIF(NOT UNAME_CMD)

  # run uname -n to get nodename
  EXECUTE_PROCESS(
  COMMAND "${UNAME_CMD}" "-n"
  RESULT_VARIABLE UNAME_RV
  ERROR_VARIABLE UNAME_EV
  OUTPUT_VARIABLE UNAME_OV)

  IF (NOT "${UNAME_RV}" STREQUAL "0")
    MESSAGE(ERROR "ERROR: uname terminated unclean.")
  ENDIF (NOT "${UNAME_RV}" STREQUAL "0")

  # check wheter we are on a tardis machine
  IF ("${UNAME_OV}" MATCHES "tardis")
    SET (IS_TARDIS ON)
  ENDIF ("${UNAME_OV}" MATCHES "tardis")

ENDIF (UNIX)

MARK_AS_ADVANCED(IS_TARDIS)

IF (IS_TARDIS)
  MESSAGE(STATUS "Running on D-ITET isg.ee Tardis Computer. Using customized paths.")
  SET($ENV{LUA_DIR} "/usr/pack/lua-5.0.3-sd/i686-debian-linux3.1/lib")
ENDIF (IS_TARDIS)
