# irkenqueue.tcl
#
# Simple flow control management so as to avoid flooding.

namespace eval ::irk {

    # For each combination of destination+connection, we keep three items:
    # 
    # flowctl($dest,$conn,after)	The "after" token for the next time
    #					we send anything to this destination.
    # flowctl($dest,$conn,queue)	A list of items to send to this
    #					destination on this connection.
    # flowctl($dest,$conn,lastsend)	The time we last sent to this
    #					destination on this connection.
    #
    # NOTE: We do not limit the length of each item to send. This
    # would lead to extremely hard to diagnose bugs due to commands
    # (e.g. ctcp's) getting cut up into chunks.

    variable flowctl

    # The following setting determines the number of seconds that must
    # pass between sends to any one destination+connection. If fewer
    # seconds have passed since the last send, then flow control is
    # activated for this destination+connection.

    set flowctl(sendlimit) 2

    # This procedure sends an item to a specific destination+connection.
    # If possible, the item is sent right away. Otherwise it is enqueued
    # for later sending.

    proc enqueue {dest conn item} {
	variable flowctl

	# If this destination has a backlog, append the new
	# items. Otherwise, check if the previous send was
	# less than 2 seconds ago. If so, enqueue it for
	# later sending. Otherwise send it now and record
	# the time we sent this item.

	if {[info exists flowctl($dest,$conn,after)]} {
	    lappend flowctl($dest,$conn,queue) $item
	} else {
	    if {[catch {set lastsend $flowctl($dest,$conn,lastsend)}]} {
		set lastsend 0
	    }
	    set now [clock seconds]
	    set lim $flowctl(sendlimit)
	    if {[expr $now - $lastsend] < $lim} {
		lappend flowctl($dest,$conn,queue) $item
		set wait [expr ($lim - ($now - $lastsend)) * 1000]
		set flowctl($dest,$conn,after) \
			[after $wait [list ::irk::sender $dest $conn]]
	    } else {
		set flowctl($dest,$conn,lastsend) $now
		puts $conn $item
	    }
	}

	return ""
    }

    # This procedure does the sending when flow control for a connection
    # is activated.

    proc sender {dest conn} {
	variable flowctl

	# First of all clear the after entry.

	unset flowctl($dest,$conn,after)

	# Grab the first item on the queue:

	if {[info exists flowctl($dest,$conn,queue)]} {
	    set items $flowctl($dest,$conn,queue)
	    if {[string compare $items ""]} {
		set item [lindex $items 0]
		set rest [lrange $items 1 end]
		if {[string compare $rest ""]} {
		    set lim [expr $flowctl(sendlimit) * 1000]
		    set flowctl($dest,$conn,queue) $rest
		    set flowctl($dest,$conn,after) \
			    [after $lim [list ::irk::sender $dest $conn]]
		} else {
		    unset flowctl($dest,$conn,queue)
		}

		# Record time we last sent to this destination.

		set flowctl($dest,$conn,lastsend) [clock seconds]

		# Send this item:

		puts $conn $item
	    } else {
		unset flowctl($dest,$conn,queue)
	    }
	}
    }
}

