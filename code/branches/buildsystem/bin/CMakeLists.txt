SET(CONFIG_FILES
  def_keybindings.ini
  disco.txt
  irc.tcl
  remote.tcl
  telnet_server.tcl
)

IF(IS_TARDIS)
  SET(CONFIG_FILES ${CONFIG_FILES}
    linux/run-script
	tardis/orxonox.ini
  )

ELSEIF(UNIX)
  SET(CONFIG_FILES ${CONFIG_FILES}
    linux/run-script
	linux/orxonox.ini
  )
ENDIF(IS_TARDIS)

IF(MINGW)
  SET(CONFIG_FILES ${CONFIG_FILES}
    "mingw/orxonox.ini"
    "mingw/orxonox.bat"
  )
ENDIF(MINGW)

FOREACH(_FILE ${CONFIG_FILES})
  GET_FILENAME_COMPONENT(_filename ${_FILE} NAME)
  IF (NOT EXISTS ${EXECUTABLE_OUTPUT_PATH}/${_filename})
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${_FILE} ${EXECUTABLE_OUTPUT_PATH}/${_filename} COPYONLY)
  ENDIF (NOT EXISTS ${EXECUTABLE_OUTPUT_PATH}/${_filename})
ENDFOREACH(_FILE)
