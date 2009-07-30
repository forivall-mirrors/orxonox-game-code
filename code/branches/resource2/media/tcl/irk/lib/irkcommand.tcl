# irkcommand.tcl:
#
# Various commands that can be invoked by the user of the IRK library:

namespace eval irk {

    # Say something to a user or to a channel

    proc say {token chan saying} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	if {![string compare "" $saying]} {
	    return
	}
	sendit $sock "PRIVMSG $chan :[string trim $saying]"
    }

    # Send a NOTICE to a user or a channel

    proc notice {token chan args} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	sendit $sock "NOTICE $chan :$args"
    }

    # Send a raw command to the server:

    proc send {token args} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	puts $sock $args
    }

    # Change your NICK

    proc nick {token {newnick ""}} {
	variable state

	if {![string compare "" $newnick]} {
	    return $state($token,nick)
	}
	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	puts $sock "NICK $newnick"
    }

    # Join a channel

    proc join {token chans {keys ""}} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	puts $sock "JOIN $chans $keys"
    }

    # Leave a channel

    proc leave {token chans {partmsg ""}} {
	variable state

	if {![string compare $partmsg ""]} {
	    if {[info exists state(partmsg)]} {
		set partmsg $state(partmsg)
	    }
	}
	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	puts $sock "PART $chans $partmsg"
    }

    # Return a list of all connections that are open:

    proc connections {} {
	variable state

	if {![info exists state(connections)]} {
	    return {}
	}
	return $state(connections)
    }

    # Return a list of channels we're on in a give connection:

    proc onchannels {token} {
	variable state

	if {![info exists state($token,channels)]} {
	    return {}
	}
	return $state($token,channels)
    }

    # Returns 1 if we are on the given channel.

    proc onchannel {token channel} {
	variable state

	if {![info exists state($token,channels)]} {
	    return 0
	}

	set idx [lsearch $state($token,channels) $channel]
	if {$idx == -1} {
	    return 0
	}
	return 1
    }

    # Return a list of users on a given channel (we must be on that channel).

    proc whoison {token chan} {
	variable state

	if {![info exists state($token,$chan,NAMES)]} {
	    return {}
	}
	return $state($token,$chan,NAMES)
    }

    # Ping a server

    proc ping {token} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	set state($token,PINGSTART) [clock clicks -millis]
	puts $sock "PING $state($token,host)"
    }

    # Quit this connection

    proc quit {token} {
	disconnect $token
    }

    # Query information about someone

    proc whois {token nick} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	puts $sock "WHOIS $nick"
    }

    # Set an away message

    proc away {token args} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	puts $sock "AWAY :$args"
    }

    # This procedure lets the client do CTCP actions:

    proc ctcp {token target action args} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	sendit $sock "PRIVMSG $target :\001$action $args\001"
    }

    # This command implements the TCL CTCP protocol:

    proc tcl {token target args} {
	variable state

	# Send it to the channel:

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid irc connection"
	}
	sendit $sock "PRIVMSG $target :\001TCL $args\001"

	# Apply it locally:

	if {[info exists state($token,channel,ctcp,TCL,LOCAL)]} {
	    $state($token,channel,ctcp,TCL,LOCAL) \
		    $token $state($token,nick) $state($token,user) \
		    PRIVMSG $target TCL $args
	}
    }

    # These procedures add and remove action handlers:

    # These procedures add actions that will be called when the user
    # with the given nick causes the supplied comm(and) to be executed
    # on the given dest(ination). This is the most specific form of action.

    proc addaction3 {token nick comm dest cmd} {
	variable state

	if {![info exists state($token,$nick,$comm,$dest)]} {
	    set state($token,$nick,$comm,$dest) [list $cmd]
	} else {
	    lappend state($token,$nick,$comm,$dest) $cmd
	}
    }

    proc setaction3 {token nick comm dest cmd} {
	variable state

	set state($token,$nick,$comm,$dest) [list $cmd]
    }

    # This procedure removes an action set by either of the above two
    # procedures.

    proc remaction3 {token nick comm dest cmd} {
	variable state

	if {![info exists state($token,$nick,$comm,$dest)]} {
	    return
	}
	set cmds $state($token,$nick,$comm,$dest)
	set idx [lsearch $cmds $cmd]
	if {$idx == -1} {
	    return
	}
	set state($token,$nick,$comm,$dest) [lreplace $cmds $idx $idx]
	if {![string compare "" $state($token,$nick,$comm,$dest)]} {
	    unset state($token,$nick,$comm,$dest)
	}
    }

    # These procedures add actions that will be called when the
    # specific comm(and) is caused for the given dest(ination).

    proc addaction2 {token comm dest cmd} {
	variable state

	if {![info exists state($token,$comm,$dest)]} {
	    set state($token,$comm,$dest) [list $cmd]
	} else {
	    lappend state($token,$comm,$dest) $cmd
	}
    }

    proc setaction2 {token comm dest cmd} {
	variable state

	set state($token,$comm,$dest) [list $cmd]
    }

    # This procedure removes an action set by either of the above two
    # procedures.

    proc remaction2 {token comm dest cmd} {
	variable state

	if {![info exists state($token,$comm,$dest)]} {
	    return
	}
	set cmds $state($token,$comm,$dest)
	set idx [lsearch $cmds $cmd]
	if {$idx == -1} {
	    return
	}
	set state($token,$comm,$dest) [lreplace $cmds $idx $idx]
	if {![string compare "" $state($token,$comm,$dest)]} {
	    unset state($token,$comm,$dest)
	}
    }

    # These procedures add actions that will be called when the
    # specific comm(and) is caused any dest(ination).

    proc addaction1 {token comm cmd} {
	variable state

	if {![info exists state($token,$comm)]} {
	    set state($token,$comm) [list $cmd]
	} else {
	    lappend state($token,$comm) $cmd
	}
    }

    proc setaction1 {token comm cmd} {
	variable state

	set state($token,$comm) [list $cmd]
    }

    # This procedure removes an action set by either of the above two
    # procedures.

    proc remaction1 {token comm cmd} {
	variable state

	if {![info exists state($token,$comm)]} {
	    return
	}
	set cmds $state($token,$comm)
	set idx [lsearch $cmds $cmd]
	if {$idx == -1} {
	    return
	}
	set state($token,$comm) [lreplace $cmds $idx $idx]
	if {![string compare "" $state($token,$comm)]} {
	    unset state($token,$comm)
	}
    }

    # These procedures add global actions that will be called
    # when the specific comm(and) is caused on any dest(ination) and
    # any irc connection. These are the lowest priority commands.

    proc addactionglobal {comm cmd} {
	variable state

	if {![info exists state(cmd,$comm)]} {
	    set state(cmd,$comm) [list $cmd]
	} else {
	    lappend state(cmd,$comm) $cmd
	}
    }

    proc setactionglobal {comm cmd} {
	variable state

	set state(cmd,$comm) [list $cmd]
    }

    # This procedure removes an action set by either of the above two
    # procedures.

    proc remactionglobal {comm cmd} {
	variable state

	if {![info exists state(cmd,$comm)]} {
	    return
	}
	set cmds $state(cmd,$comm)
	set idx [lsearch $cmds $cmd]
	if {$idx == -1} {
	    return
	}
	set state(cmd,$comm) [lreplace $cmds $idx $idx]
	if {![string compare "" $state(cmd,$comm)]} {
	    unset state(cmd,$comm)
	}
    }

    # This procedure manages configuration information for IRC:

    proc config {args} {
	if {$args == {}} {
	    return [collectConfig]
	}

	if {[llength $args] == 1} {
	    return [queryConfig [lindex $args 0]]
	}

	if {[expr [llength $args] % 2] != 0} {
	    error "incorrect number of argument, must be multiple of 2"
	}

	setConfig $args
    }

    # Helper procedure to return a list with all important user settable
    # configuration information.

    proc collectConfig {} {
	variable state

	set config {}

	foreach name [array names state "-*"] {
	    lappend config [list $name $state($name)]
	}
	return $config
    }

    # Helper procedure to return the value of one option.

    proc queryConfig {option} {
	variable state

	if {![info exists state($option)]} {
	    return {}
	}
	return $state($option)
    }

    # Helper procedure to modify the configuration of a set of options.

    proc setConfig {theargs} {
	variable state

	foreach {opt val} $theargs {
	    if {![string match "-*" $opt]} {
		continue
	    }
	    set state($opt) $val
	}
    }
}
