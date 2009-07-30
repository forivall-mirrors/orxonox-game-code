# fortunebot.tcl --
#
# Demo bot using the irc TCL client library. This is a VERY simple bot that
# demonstrates some simple uses of the irc TCL library.
#
# The bot sits on any number of channels and networks. It periodically grabs
# a fortune from a web site and sends the fortune with appropriate delays
# between each line to all channels on all networks it is on.

# Example use in Tcl:
#
# % source fortunebot.tcl
# % set token [irc::connect ....]
# % fortune::join $token #mychannel
# % fortune::start 120
#
# What this does:
# * Loads the bot, which in turn will load the IRC library and HTTP.
# * Connect to IRC
# * Send the bot to join #mychannel
# * Start the bot, with 120 seconds delay. Now it'll do its actions every
#   120 seconds.
#
# To stop the bot:
#
# % fortune::stop
#
# Make him leave a channel:
#
# % fortune::leave $token #mychannel

package require irk
package require http

namespace eval fortune {
    variable state

    array set state {
	linedelay		2000
	fortuneurl		http://www.earth.com/fortune
    }
}

# Bot control:

proc ::fortune::start {{delay 60}} {
    variable state

    # Compute the delay in milliseconds:

    set state(delay) [expr $delay * 1000]

    # Schedule the bot to run each $delay milliseconds:

    set state(after) [after $state(delay) [list ::fortune::doquote]]
}

proc ::fortune::stop {} {
    variable state

    # Stop the bot if its running:

    if {[info exists state(after)]} {
	after cancel $state(after)
	unset state(after)
    }
}


# This is the actual body of the bot:
#
# Grab a quote from a web page and post it to all channels we're on:

proc ::fortune::doquote {} {
    variable state

    # Grab the quote. The command callback does all the work:

    http::geturl $state(fortuneurl) -command ::fortune::httpdone

    # Finally reschedule ourselves, after events are one-shots

    set state(after) [after $state(delay) [list ::fortune::doquote]]
}

proc ::fortune::httpdone {http} {
    variable state
    upvar #0 $http response

    # Scrape the fortune off of the page:

    set fortune [grabfortune $response(body)]

    # Discard the HTTP array:

    unset response

    # Check if the quote is too long. If it is then punt.

    if {[llength $fortune] > 3} {
	return
    }

    # Say this quote on all channels on all connections we're on:

    foreach conn [irk::connections] {
	tell $fortune $conn
    }
}

# This procedure scrapes the quote off of an HTML page:

proc ::fortune::grabfortune {body} {
    set body [split $body "\n"]
    set beg [lsearch $body <PRE>]
    set end [lsearch $body </PRE>]
    return [lrange $body [expr $beg+1] [expr $end-1]]
}

# This procedure sends the quote to all channels we want the bot to be on:

proc ::fortune::tell {fort conn} {
    variable state

    # Send the fortune to each channel we're on:

    foreach chan [irk::onchannels $conn] {
	tellchan $fort $conn $chan
    }
}

# Asynchronously send lines to the channel:

proc ::fortune::tellchan {fort conn channel} {
    variable state

    # Check if we are still on the channel:

    if {![irk::onchannel $conn $channel]} {
	return
    }

    # OK we're still on this channel, so say the current line and schedule
    # the next line for later:

    if {[llength $fort] > 0} {
	irk::say $conn $channel [lindex $fort 0]
	after $state(linedelay) \
	    [list ::fortune::tellchan [lrange $fort 1 end] $conn $channel]
    }
}
