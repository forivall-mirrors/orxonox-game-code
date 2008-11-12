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
  SET(CMAKE_C_COMPILER "gcc-4.1")
  SET(CMAKE_CXX_COMPILER "g++-4.1")
  #SET(Boost_ADDITIONAL_VERSIONS "1.36")
  SET(BOOST_INCLUDEDIR "/usr/pack/boost-1.34.1-sd/include")
  SET(BOOST_LIBRARYDIR "/usr/pack/boost-1.34.1-sd/i686-debian-linux3.1/lib")
  SET(ENV{LUA_DIR} "/usr/pack/lua-5.0.3-sd;/usr/pack/lua-5.0.3-sd/i686-debian-linux3.1")
  SET(ENV{ALUTDIR} "/usr/pack/openal-0.0.8-cl;/usr/pack/openal-0.0.8-cl/i686-debian-linux3.1")
  SET(ENV{OPENALDIR} "/usr/pack/openal-0.0.8-cl;/usr/pack/openal-0.0.8-cl/i686-debian-linux3.1")
  SET(ENV{ENETDIR} "/usr/pack/enet-2007-sd;/usr/pack/enet-2007-sd/i686-debian-linux3.1")
  SET(ENV{OGGVORBISDIR} "/usr/pack/oggvorbis-1.0-ds;/usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.0")
  #SET(TCL_INCLUDE_PATH "/usr/pack/tcltk-8.4.9.linux-mo/include")
  SET(ENV{CEGUIDIR} "/usr/pack/cegui-0.5.0-sd;/usr/pack/cegui-0.5.0-sd/i686-debian-linux3.1")
  SET(ENV{OGREDIR} "/usr/pack/ogre-1.4.5-sd;/usr/pack/ogre-1.4.5-sd/i686-debian-linux3.1")
ENDIF (IS_TARDIS)

MARK_AS_ADVANCED(UNAME_CMD IS_TARDIS)
