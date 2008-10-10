set telnetserverthreadid [TclThreadManager create]
TclThreadManager execute $telnetserverthreadid source telnet_server.tcl
