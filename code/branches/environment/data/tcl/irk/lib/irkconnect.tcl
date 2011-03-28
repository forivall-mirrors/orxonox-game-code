# irkconnect.tcl:
#
# This file provides the IRK commands to connect to remote servers
# as well as some attendant utility procedures:

namespace eval ::irk {

    # The "connect" procedure returns a token
    # for the connection to this server.
    #
    # symsrv		The symbolic name of the server to connect to.
    # nick		The nick name to use.
    # user		The user name to use.
    # pass		The password to use. 
    # disp		The command dispatcher expression to use.

    proc connect {{symsrv ""} {nick ""} {user ""} {pass ""} {disp ""}} {
	variable state

	# Auto initialize the list of connections. We do this so that
	# the list is not reset each time the irk.tcl file is sourced.

	if {![info exists state(connections)]} {
	    set state(connections) {}
	}

	# Try to convert the symbolic server name to a
	# server description. This may throw an error,
	# we propagate it.

	set servdesc [locate $symsrv]

	# If the nickname is not specified, there must be
	# a nick setting in the state array; use that

	if {![string compare $nick ""]} {
	    if {![info exists state(-$symsrv,nick)]} {
		error "No nick given or set in global state"
	    }
	    set nick $state(-$symsrv,nick)
	}

	# If a global nick is not set, save this nick for use
	# as a global nick.

	if {![info exists state(-$symsrv,nick)]} {
	    set state(-$symsrv,nick) $nick
	}

	# If user is not supplied, see if there is a global
	# user registered in the IRK package state. If not,
	# invent a user.

	if {![string compare $user ""]} {
	    if {[info exists state(-$symsrv,user)]} {
		set user $state(-$symsrv,user)
	    } else {
		set user $nick
		set state(-$symsrv,user) $user
	    }
	}

	# If a password is not supplied, see if there is a global
	# one registered. If so, use that. Otherwise, do not
	# use a password.

	if {![string compare $pass ""]} {
	    if {[info exists state(-$symsrv,pass)]} {
		set pass $state(-$symsrv,pass)
	    }
	}

	# If a dispatcher expression is not supplied, there must be a
	# global dispatcher expression in the state array, and use that.

	if {![string compare $disp ""]} {
	    if {![info exists state(dispatcher)]} {
		error "ERROR: o dispatch given or found in global state"
	    }
	    set disp $state(dispatcher)
	}

	# Try to connect to the given server.

	set h [lindex $servdesc 0]
	set p [randselect [lindex $servdesc 1]]

	set s [opensocket $h $p]

	# The socket is line buffered and consumed by the
	# supplied consumer

	fconfigure $s -translation auto -buffering line
	fileevent $s readable [list ::irk::consumer $s]


	# Identify ourselves to the IRK server: If a password is given
	# send that first. Then send the nick name and user name.

	if {[string compare $pass ""]} {
	    puts $s "PASS $pass"
	}

	puts $s "NICK $nick"
	puts $s "USER $user $h $h :$user"

	# Make a connection token:

	set contok [contok $s $symsrv]

	# Save the state for this new connection

	lappend state(connections) $contok

	set state($contok,port) $p
	set state($contok,host) $h
	set state($contok,symsrv) $symsrv
	set state($contok,nick) $nick
	set state($contok,user) $user
	set state($contok,pass) $pass
	set state($contok,disp) $disp

	# Set up some default behavior for the connection:

	setupDefaultActions $contok $nick

	# Collect information about who the server thinks we are

	puts $s "WHOIS $nick"

	# Finally return the token for this connection:

	return $contok
    }

    # This procedure makes an easy to remember connection token. It takes
    # the symbolic server's name and appends _<n> to it, where n is an
    # integer starting at 0 and monotonically increasing for every new
    # connection to that server.
    #
    # Once it figures out what the connection token is going to be,
    # it associates it with the given socket so it can be used.

    proc contok {sock symsrv} {
	variable state

	# Compute the symbolic name for this connection:

	if {![info exists state($symsrv,counter)]} {
	    set state($symsrv,counter) 0
	}
	set contok ${symsrv}_$state($symsrv,counter)
	incr state($symsrv,counter)

	# Associate the symbolic name with the socket:

	set state($contok,socket) $sock
	set state($sock,contok) $contok

	return $contok
    }

    # The locate procedure tries to convert the symbolic name for
    # a connection to a server/port specification.

    proc locate {s} {
	variable symsrv
	variable state

	# If the caller specified "" as the name of the server, select
	# a random one from the list of known servers.

	if {![string compare $s ""]} {
	    set s [randselect $state(servers)]
	}

	# Now see if the requested server exists:

	if {![info exists symsrv($s)]} {
	    error \
		"Could not find a match for symbolic IRK server name \"$s\""
	}

	# It does, return the server specification:

	return $symsrv($s)
    }

    # opensocket connects to the requested server and port, either
    # directly or through a SOCKS5 proxy.

    proc opensocket {server port} {
	variable state

	if {[info exists state(-socksproxy)] \
		&& [info exists state(-socksport)]} {
	    set sock [socket $state(-socksproxy) $state(-socksport)]
	    return [::socks::init $sock $server $port]
	}

	return [socket $server $port]
    }

    # The disconnect procedure disconnects from a given connection
    # identified by its symbolic name, and cleans up state associated
    # with the connection.

    proc disconnect {contok} {
	variable state

	if {[catch {set s $state($contok,socket)} err]} {
	    error "ERROR: $contok: No such IRC connection"
	}

	# Send a QUIT message.

	if {[info exists state(quitmsg)]} {
	    set q ":$state($contok,nick) $state(quitmsg)"
	} else {
	    set q ":$state($contok,nick) quit"
	}
	puts $s "QUIT $q"

	# Try to close the connection with the server.

	catch {close $s}

	# And clean up all state associated with this connection:

	array unset state $contok,*
	
	# Remove this connection from the list of active connections:

	set i [lsearch $state(connections) $s]
	set state(connections) [lreplace $state(connections) $i $i]

	return ""
    }
}
