# mysaint.tcl
#
# I got the inspiration to write this bot from the SmallSaints
# bot on DalNet and the request of some Malay friends to create a clone.
#
# The bot sits on a channel and simply counts words that each user says.

package require irk

namespace eval ::mysaint {

    variable state

    set state(thisdir) [file dir [info script]]
    set state(words,save) [file join $state(thisdir) savedwords.dat]
    set state(save,interval) 30000

    # For each identified user, there are several entries in the state array:
    #
    # state($user,words)	How many words they have total.
    # state($user,session)	How many words they said this time.
    # state($user,password)	What's their password
    # state($user,ident)	What's their ident
    #
    # We also keep a mapping from nicknames to users:
    #
    # state($nick,user)		What's the user mask associated with this
    #				nick name

    proc initialize {} {
	variable state
	variable thisdir

	# Set up a timer to save words:

	set state(saver) [after $state(save,interval) ::mysaint::save]

	# See if we have a saved words file. If so, read it in.

	if {[file exists $state(words,save)]} {
	    uplevel #0 source $state(words,save)
	}
    }

    proc userSays {token nick user comm dest rest} {
	if {[string match "!*" [lindex $rest 0]]} {
	    userSaysCommand $token $dest $nick $user \
		    [lindex $rest 0] [lrange $rest 1 end]
	} else {
	    userSaysWords $nick $user [llength $rest]
	}
    }

    proc userSaysWords {nick user n} {
	variable state

	set seconds [clock seconds]
	if {![info exists state($user,session,time)]} {
	    set state($user,session,time) [clock seconds]
	}
	set state($user,time) $seconds

	if {![info exists state($user,session)]} {
	    set state($user,session) $n
	} else {
	    incr state($user,session) $n
	}
	if {![info exists state($user,words)]} {
	    set state($user,words) $n
	} else {
	    incr state($user,words) $n
	}

	set state($nick,user) $user
    }

    proc userSaysCommand {token dest nick user cmd rest} {
	variable state

	# Determine destination of any reply we're going to send:

	if {![string compare $dest $state(nick)]} {
	    set dest $nick
	}

	switch -exact $cmd {
	    "!help"	{userAsksHelp $token $nick}
	    "!time"	{userAsksTime $token $nick $user $rest $dest}
	    "!heard"	{userAsksHeard $token $nick $user $rest $dest}
	    "!words"	{userAsksWords $token $nick $user $rest $dest}
	    "!donate"	{userDonates $token $nick $user $rest $dest}
	    "!pass"	{::irk::pass $token mysaint $nick $user $rest}
	    "!ident"	{::irk::id $token mysaint $nick $user $rest}
	    "!newpass"	{::irk::np $token mysaint $nick $user $rest}
	}
    }

    proc userAsksHelp {token nick} {
	::irk::say $token $nick \
		[list Hello $nick, the following commands are supported: ]
	::irk::say $token $nick "!help ---- prints this message"
	::irk::say $token $nick "!words --- how many words you have"
	::irk::say $token $nick "!donate -- to donate words to someone"
	after 2000 [list ::irk::say $token $nick \
				"!heard --- when did some speak last time"]
	after 2500 [list ::irk::say $token $nick \
				"!time ---- when someone connected"]
	after 5000 [list ::irk::say $token $nick \
		    "Note: To donate you have to have a password"]
	after 7500 [list ::irk::say $token $nick \
		    "Note: You can get a password with !pass"]
    }

    proc userAsksWords {token nick user rest dest} {
	variable state

	set target [lindex $rest 0]
	if {[string compare $target ""]} {
	    if {[catch {set user $state($target,user)}]} {
		set who "The unknown user $target"
		set target "The unknown user $target"
	    } else {
		set who $user
	    }
	} else {
	    set who ""
	}
	if {[string compare $who ""]} {
	    if {[catch {set words $state($who,words)}]} {
		set words 0
	    }
	    if {[catch {set session $state($who,session)}]} {
		set session 0
	    }
	    set reply \
		    "$nick, $target has $session words now, total $words words"
	} else {
	    if {[catch {set words $state($user,words)}]} {
		set words 0
	    }
	    if {[catch {set session $state($user,session)}]} {
		set session 0
	    }
	    set reply \
		    "$nick, you have $session words now, $words total words"
	}
	::irk::say $token $dest $reply
    }

    proc userAsksHeard {token nick user rest dest} {
	variable state

	set target [lindex $rest 0]
	if {[string compare $target ""]} {
	    if {[catch {set user $state($target,user)}]} {
		set answer "I haven't heard $target speak recently."
	    } else {
		set time $state($user,time)
		set time [clock format $time]
		set answer "I heard $target speak last at $time"
	    }
	} else {
	    set answer "I'm talking to you just now, $nick"
	}

	::irk::say $token $dest $answer
    }

    proc userAsksTime {token nick user rest dest} {
	variable state

	set target [lindex $rest 0]
	if {[string compare $target ""]} {
	    if {[catch {set user $state($target,user)}]} {
		set answer "I don't know when $target connected"
	    } else {
		if {[catch {set time $state($user,session,time)}]} {
		    set answer "I don't know when $target connected"
		} else {
		    set time [clock format $time]
		    set answer \
			    "I first noticed $target at $time"
		}
	    }
	} else {
	    if {[catch {set user $state($nick,user)}]} {
		set answer "$nick, I don't know when you connected"
	    } else {
		if {[catch {set time $state($user,session,time)}]} {
		    set answer "$nick, I don't know when you connected"
		} else {
		    set time [clock format $time]
		    set answer \
			    "$nick, I first noticed you at $time"
		}
	    }
	}

	::irk::say $token $dest $answer
    }

    # Start the bot:

    proc start {token} {
	variable state

	set state($token,token) $token

	set symsrv $::irk::state($token,symsrv)
	set state(nick) $::irk::state(-$symsrv,nick)
	set nick $state(nick)

	foreach chan [::irk::onchannels $token] {
	    ::irk::setaction2 $token PRIVMSG $chan ::mysaint::userSays
	}
	::irk::setaction2 $token PRIVMSG $nick ::mysaint::userSays

	# Set up a timer to save the words info.

	if {![info exists state(saver)]} {
	    initialize
	}

	return ""
    }

    # Stop the bot:

    proc stop {} {
	variable state

	foreach conn [array names state *,token] {
	    stopcon $state($conn)
	}

	after cancel $state(saver)
	catch {unset state(saver)}

	return ""
    }
    proc stopcon {token} {
	variable state

	if {[catch {set symsrv $::irk::state($token,symsrv)}]} {
	    return
	}
	set nick $state(nick)

	foreach chan [::irk::onchannels $token] {
	    ::irk::remaction2 $token PRIVMSG $chan ::mysaint::userSays
	}
	::irk::remaction2 $token PRIVMSG $nick ::mysaint::userSays

	return ""
    }

    # Save words to a disk file:

    proc save {} {
	variable state

	puts "Saving words in $state(words,save)"

	# First of all reschedule ourselves:

	set state(saver) [after $state(save,interval) ::mysaint::save]

	# Save the words:

	set pt "*,words"
	if {![catch {set fd [open $state(words,save) w]}]} {
	    puts $fd "array set ::mysaint::state [list [array get state $pt]]"

	    catch {close $fd}
	}
    }	
}
