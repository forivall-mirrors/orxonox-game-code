set telnetserverthreadid [TclThreadManager create]
TclThreadManager execute $telnetserverthreadid source $pld/telnet_server.tcl
