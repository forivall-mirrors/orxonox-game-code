# irkutil.tcl:
#
# Utility procedures used throughout the IRK package.

namespace eval ::irk {

    # The randselect randomly selects an element of a given list.

    proc randselect {l} {
	lindex $l [expr int(floor(rand() * [llength $l]))]
    }

    # Remove a user from the list of users on a channel

    proc removeFromChannel {token user chan} {
	variable state

	# If it's us that left the channel, forget the channel:

	if {![string compare $state($token,nick) $user]} {
	    removeChannel $token $chan
	    return
	}

	# Try to retrieve the channel's user list:

	if {[catch {set users $state($token,$chan,NAMES)}]} {
	    return
	}
	set idx [lsearch $users $user]
	if {$idx == -1} {
	    return
	}

	# Remove the user:

	set state($token,$chan,NAMES) [lreplace $users $idx $idx]
    }

    # Remove a user from all the channels I am on (for QUIT etc.)

    proc removeFromAllChannels {token user} {
	variable state

	foreach chan $state($token,channels) {
	    removeFromChannel $token $user $chan
	}
    }

    # Add a user to a channel I am on.

    proc addToChannel {token user chan} {
	variable state

	lappend state($token,$chan,NAMES) $user
    }

    # Add a channel to the list of channels we're on:

    proc addChannel {token chan} {
	variable state

	lappend state($token,channels) $chan
    }

    # Remove a channel from the list of channels we're on:

    proc removeChannel {token chan} {
	variable state

	if {[catch {set channels $state($token,channels)}]} {
	    return
	}
	set idx [lsearch $channels $chan]
	if {$idx == -1} {
	    return
	}

	# Remove this channel from the list of channels we're on:

	set state($token,channels) [lreplace $channels $idx $idx]

	# Forget all state for this channel:

	array unset state $token,$chan,*
    }

    # This procedure cleans up all state associated with a connection:

    proc forgetConnection {token} {
	variable state

	array unset state $token,*
    }

    # This procedure updates the channel names for all channels we're on
    # due to a nick change.

    proc replaceAllChannels {token nick newnick} {
	variable state

	# If we're not on any channels, then no need to replace.

	if {![info exists state($token,channels)]} {
	    return
	}

	# Replace the old nick with the new nick in all the channels I'm on.

	foreach chan $state($token,channels) {
	    if {[catch {set names $state($token,$chan,NAMES)}]} {
		continue
	    }
	    set idx [lsearch $names $nick]
	    if {$idx != -1} {
		set state($token,$chan,NAMES) \
			[lreplace $names $idx $idx $newnick]
	    }
	}
    }

    # This procedure determines whether a nick is our nick.
    
    proc isus {token nick} {
	variable state

	if {![string compare $nick $state($token,nick)]} {
	    return 1
	}
	return 0
    }

    # This procedure cleans up input received as part of a CTCP action.

    proc ctcpcleanup {l} {
	if {[llength $l] > 1} {
	    set h [string range [lindex $l 0] 1 end]
	    set t [string range [lindex $l end] 0 end-1]
	    return [lreplace [lreplace $l 0 0 $h] end end $t]
	}
	return [string range $l 1 end-1]
    }
	
}
