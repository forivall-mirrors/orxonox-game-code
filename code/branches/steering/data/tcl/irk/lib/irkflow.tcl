# irkflow.tcl
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

    # The following setting controls how often (in msecs) we can send
    # to a given connection. It should not be modified by user code or
    # flooding may occur.

    set flowctl(floodlimit) 1500

    # This procedure sends an item to a specific destination+connection.
    # If possible, the item is sent right away. Otherwise it is enqueued
    # for later sending.

    proc sendit {conn item} {
	variable flowctl

	# If this connection has a backlog, append the new
	# items. Otherwise, check if the previous send was
	# less than flowctl(floodlimit) msecs ago. If so, enqueue
	# it for later sending. Otherwise send it now and record
	# the time we sent this item.

	if {[info exists flowctl($conn,after)]} {
	    lappend flowctl($conn,queue) $item
	} else {
	    if {[catch {set lastsend $flowctl($conn,lastsend)}]} {
		set lastsend 0
	    }
	    set now [clock clicks -milliseconds]
	    set lim $flowctl(floodlimit)
	    if {[expr $now - $lastsend] < $lim} {
		lappend flowctl($conn,queue) $item
		set wait [expr ($lim - ($now - $lastsend))]
		set flowctl($conn,after) \
			[after $wait [list ::irk::sendlater $conn]]
	    } else {
		set flowctl($conn,lastsend) $now
		puts $conn $item
	    }
	}

	return ""
    }

    # This procedure does the sending when flow control for a connection
    # is activated.

    proc sendlater {conn} {
	variable flowctl

	# First of all clear the after entry.

	unset flowctl($conn,after)

	# Grab the first item on the queue:

	if {[info exists flowctl($conn,queue)]} {
	    set items $flowctl($conn,queue)
	    if {[string compare $items ""]} {
		set item [lindex $items 0]
		set rest [lrange $items 1 end]
		if {[string compare $rest ""]} {
		    set flowctl($conn,queue) $rest
		    set flowctl($conn,after) \
			    [after $flowctl(floodlimit) \
				[list ::irk::sendlater $conn]]
		} else {
		    unset flowctl($conn,queue)
		}

		# Record time we last sent to this destination.

		set flowctl($conn,lastsend) [clock clicks -milliseconds]

		# Send this item:

		puts $conn $item
	    } else {
		unset flowctl($conn,queue)
	    }
	}
    }
}

