SET(CONFIG_FILES
  def_keybindings.ini
  def_masterKeybindings.ini
  disco.txt
  irc.tcl
  remote.tcl
  telnet_server.tcl
)

IF(TARDIS)
  # OGRE can't find fonts to display config screen on Tardis,
  # so providing default config file here (bug).
  SET(CONFIG_FILES ${CONFIG_FILES}
    linux/run-script
    tardis/orxonox.ini
    tardis/ogre.cfg
  )

ELSEIF(UNIX)
  SET(CONFIG_FILES ${CONFIG_FILES}
    linux/run-script
    linux/orxonox.ini
  )
ENDIF(TARDIS)

IF(MINGW)
  SET(CONFIG_FILES ${CONFIG_FILES}
    "mingw/orxonox.ini"
    "mingw/orxonox.bat"
  )
ENDIF(MINGW)

FOREACH(_FILE ${CONFIG_FILES})
  GET_FILENAME_COMPONENT(_filename ${_FILE} NAME)
  IF (NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_filename})
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${_FILE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_filename} COPYONLY)
  ENDIF (NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_filename})
ENDFOREACH(_FILE)
