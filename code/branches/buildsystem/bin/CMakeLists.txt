SET(CONFIG_FILES
    def_keybindings.ini
    disco.txt
    irc.tcl
    ogre.cfg-init
    orxonox.bat
    Plugins.cfg
    plugins.cfg-init
    remote.tcl
    run-script
    telnet_server.tcl
)

FOREACH(_FILE ${CONFIG_FILES})
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${_FILE} ${EXECUTABLE_OUTPUT_PATH}/${_FILE} COPYONLY)
ENDFOREACH(_FILE)
