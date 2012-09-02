# irkdispatch.tcl:
#
# Based on the input received from the server, dispatch control to various
# command procedures.

namespace eval ::irk {

    # This procedure sets up the default actions for a connection:

    proc setupDefaultActions {s nick} {
	variable state

	# Catch any unexpected PRIVMSG actions:

	set state($s,PRIVMSG,$nick)	$state(PRIVMSG,unsolicited)

	# Set up CTCP global actions:

	set state($s,cmd,ctcp,PING)	::irk::RECV,CTCP,PING
	set state($s,cmd,ctcp,TIME)	::irk::RECV,CTCP,TIME
	set state($s,cmd,ctcp,VERSION)	::irk::RECV,CTCP,VERSION
	set state($s,cmd,ctcp,USERINFO) \
					::irk::RECV,CTCP,USERINFO

	# Set up CTCP channel specific actions:

	set state($s,channel,ctcp,ACTION) \
					::irk::RECV,CTCP,ACTION

	# Set up the CTCP TCL protocol:

	set state($s,channel,ctcp,TCL)	::irk::RECV,CTCP,TCL
	set state($s,channel,ctcp,TCL,LOCAL) \
					::irk::RECV,CTCP,TCL,LOCAL

	# Deal with CTCP commands we do not understand:

	set state($s,cmd,ctcp,error) 	::irk::RECV,CTCP,ERROR
	set state($s,channel,ctcp,error) \
					::irk::RECV,CTCP,ERROR
    }

    # The consumer procedure consumes input received from
    # a connection. It calls the dispatcher for the given connection
    # with each input line.

    proc consumer {s} {
	variable state

	if {[eof $s]} {
	    disconnect $s
	} else {
	    set l [gets $s]
	    if {[info exists state($s,trace)]} {
		$state($s,trace) $l
	    }
	    if {[string compare $l ""]} {
		dissect $state($s,contok) $l
	    }
	}
    }

    # This procedure dissects the input into its parts:

    proc dissect {token line} {
	variable state

	# Make a list out of the line:

	set line [split $line " "]

	# Split first token into nickname and user mask:

	set nandu [split [lindex $line 0] \!]

	# Call dispatcher for this connection:

	$state($token,disp) \
		$token \
		[lindex $nandu 0] \
		[lindex $nandu 1] \
		[lindex $line 1] \
		[lindex $line 2] \
		[lrange $line 3 end]
    }

    # This procedure is the default command dispatcher:
    
    proc defaultDispatcher {token nick user comm dest rest} {
	variable state

	# Check if the nick starts with ':'. If not then this is
	# a special action, handled by the built in actions.

	if {![string match ":*" $nick]} {
	    return [specialAction $token $nick $user $comm $dest $rest]
	}

	# Trim off the leading ':' on the $nick, if present.

	if {[string match ":*" $nick]} {
	    set nick [string range $nick 1 end]
	}

	# If a ':' is present in the $dest, trim that off too.

	if {[string match ":*" $dest]} {
	    set dest [string range $dest 1 end]
	}

	# If a ':' is present in the $rest, trim that off too.

	set firstrest [lindex $rest 0]
	if {[string match ":*" $firstrest]} {
	    set firstrest [string range $firstrest 1 end]
	    set rest [lreplace $rest 0 0 $firstrest]
	}

	# Clean up the payload:

	set rest [split [string trim [eval concat $rest]] " "]

	# Now try to dispatch to specific handlers.
	#
	# First see if there is a handler for the specific combination
	# of $token, $nick, $comm and $dest. This is used for PRIVMSG.

	if {[info exists state($token,$nick,$comm,$dest)]} {
	    foreach cmd $state($token,$nick,$comm,$dest) {
		if {[catch {set res [$cmd $token $nick $user \
					  $comm $dest $rest]} err]} {
		    if {[info exists state(errorhandler)]} {
			$state(errorhandler) $err $token $comm $dest
		    }
		    return
		}

		# If this handler said to go on to a more general handler,
		# then don't return. Otherwise return.

		if {[string compare $res pass]} {
		    return
		}
	    }
	}

	# If there's a handler for $token, $comm and $dest, use that.

	if {[info exists state($token,$comm,$dest)]} {
	    foreach cmd $state($token,$comm,$dest) {
		if {[catch {set res [$cmd $token $nick $user \
					  $comm $dest $rest]} err]} {
		    if {[info exists state(errorhandler)]} {
			$state(errorhandler) $err $token $comm $dest
		    }
		    return
		}

		# If this handler said to go on to a more general handler,
		# then don't return. Otherwise return.

		if {[string compare $res pass]} {
		    return
		}
	    }
	}

	# See if there's a handler for $token and $comm. If so use that.

	if {[info exists state($token,$comm)]} {
	    foreach cmd $state($token,$comm) {
		if {[catch {set res [$cmd $token $nick $user \
					  $comm $dest $rest]} err]} {
		    if {[info exists state(errorhandler)]} {
			$state(errorhandler) $err $token $comm $dest
		    }
		    return
		}

		# If this handler said to go on to a more general handler,
		# then don't return. Otherwise return.

		if {[string compare $res pass]} {
		    return
		}
	    }
	}

	# See if there's a global handler for the command. All the
	# default handlers are defined here.

	if {[info exists state(cmd,$comm)]} {
	    foreach cmd $state(cmd,$comm) {
		if {[catch {set res [$cmd $token $nick $user \
					  $comm $dest $rest]} err]} {
		    if {[info exists state(errorhandler)]} {
			$state(errorhandler) $err $token $comm $dest
		    }
		    return
		}

		# If this handler said to go on to a more general handler,
		# then don't return. Otherwise return.

		if {[string compare $res pass]} {
		    return
		}
	    }
	}

	# If all of the above fail, send this input to the default
	# action handler:

	if {[catch {set res [$state(action) \
				$token $nick $user \
				$comm $dest $rest]} err]} {
	    if {[info exists state(errorhandler)]} {
		$state(errorhandler) $err $token $comm $dest
	    }
	    return
	}
    }

    # This procedure deals with special actions (built in, cannot
    # easily be modified by users). I use this to e.g deal with
    # PING, NOTICE, ERROR etc., automatically.

    proc specialAction {token nick user comm dest rest} {
	variable state

	# The nick is the special action selector:

	$state(special,$nick) $token $nick $user $comm $dest $rest
    }

    # This is the default error handler:

    proc echoerror {args} {
	puts stderr $args
    }

    # This procedure provides a default tracing facility (it just prints
    # the lines received to stderr):

    proc trace {args} {
	puts stderr "Received: $args"
    }
}
