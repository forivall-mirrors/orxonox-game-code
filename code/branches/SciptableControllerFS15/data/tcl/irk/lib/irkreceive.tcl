# irkreceive.tcl:
#
# Various commands invoked in response to input received from the server:

namespace eval irk {

    # This procedure deals with the PING special action:

    proc RECV,PING {token nick user comm dest rest} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid IRK connection"
	}
	puts $sock "PONG :$comm $rest"

	return ""
    }

    # This procedure deals with the NOTICE special action:

    proc RECV,NOTICE {token nick user comm dest rest} {
	variable state

	append state($token,GLOBALNOTICES) "$rest\n"

	return ""
    }


    # This procedure deals with the ERROR special action:

    proc RECV,ERROR {token nick user comm dest rest} {
	variable state

	set rest [eval concat $rest]
	append state($token,ERRORS) "$nick $dest $rest\n"
#	puts "Got ERROR: $token $nick ---> $dest: $rest"
	::orxonox::execute error "Got ERROR: $token $nick ---> $dest: $rest"

	return ""
    }

    # This procedure deals with the AWAY response:

    proc RECV,AWAY {token nick user comm dest rest} {
	set id [lindex $rest 0]
	set rest [lrange $rest 1 end]
	set rest [lreplace $rest 0 0 [string range [lindex $rest 0] 1 end]]
	set rest [eval concat $rest]
	puts "$id is away: $rest"

	return ""
    }

    # This procedure deals with the WHOIS USER message:

    proc RECV,WHOIS,NICK,USER {token nick user comm dest rest} {
	variable state

	# Split the rest of the line on space:

	foreach {unick uuser uhost ustar urnm} [split $rest " "] break

	# If the WHOIS information is about this user, save it specially:

	if {[isus $token $unick]} {
	    set state($token,uuser)	$uuser
	    set state($token,uhost)	$uhost
	    set state($token,urnm)	$urnm
	}

	# Save the information for a regular user:

	set state($token,ident,$unick,uuser)	$uuser
	set state($token,ident,$unick,uhost)	$uhost
	set state($token,ident,$unick,urnm)	$urnm
    }

    # This procedure deals with the WHOIS SERVER message:

    proc RECV,WHOIS,NICK,SERVER {token nick user comm dest rest} {
	variable state

	# Split the rest of the line on space:

	foreach {unick userv} [split $rest " "] break

	# If the WHOIS information is about this user, save it specially:

	if {[isus $token $unick]} {
	    set state($token,userv) $userv
	}

	# Save the information for a regular user:

	set state($token,ident,$unick,userv) $userv
    }

    # This procedure deals with the WHOIS IDENT message

    proc RECV,WHOIS,NICK,IDENT {token nick user comm dest rest} {
	variable state

	# Extract the nick of the user who has identified

	set unick [lindex [split $rest " "] 0]

	# If the WHOIS information is about this user, save it specially:

	if {[isus $token $unick]} {
	    set state($token,ident) 1
	}

	# Save the information for a regular user:

	set state($token,ident,$unick,ident) 1
    }

    # This procedure deals with the WHOIS CONNECTTIME message:

    proc RECV,WHOIS,NICK,CONNECTTIME {token nick user comm dest rest} {
	variable state

	# Split the rest of the input on space:

	foreach {unick idle connecttime} [split $rest " "] break

	# Format the connect time for this user:

	set connecttime [clock format $connecttime]

	# If the WHOIS information is about this user, save it specially:

	if {[isus $token $unick]} {
	    set state($token,connecttime) $connecttime
	}

	# Save the information for a regular user:

	set state($token,ident,$unick,connecttime) $connecttime
    }

    # Handle the WHOIS CHANNELS message:

    proc RECV,WHOIS,NICK,CHANNELS {token nick user comm dest rest} {
	variable state

	# Split the rest on space.

	set rest [split $rest " "]

	# Get the nick for which this is the channel list:

	set unick [lindex $rest 0]
	set rest [lrange $rest 1 end]

	# The first channel may have an extra ":", if it does get rid of it.

	set firstchan [lindex $rest 0]
	if {[string match ":*" $firstchan]} {
	    set rest [lreplace $rest 0 0 [string range $firstchan 1 end]]
	}

	# If the WHOIS information is about this user, save it specially:

	if {[isus $token $unick]} {
	    set state($token,channels) $channels
	}

	# Save the information for a regular user:

	set state($token,ident,$unick,channels) $rest
    }

    # This procedure deals with the WHOIS END message:

    proc RECV,WHOIS,NICK,END {token nick user comm dest rest} {
	variable state

	set state($token,whois,done) 1
    }

    # This procedure deals with various MOTD actions:

    proc RECV,MOTD {token nick user comm dest rest} {
	variable state

	append state($token,MOTD) "${rest}\n"

	return ""
    }

    # This procedure deals with PONG actions:

    proc RECV,PONG {token nick user comm dest rest} {
	variable state

	if {[info exists state($token,PINGSTART)]} {
	    set elapsed \
		[expr [clock clicks -millis] - $state($token,PINGSTART)]
	    puts "$nick: $elapsed msecs"
	    unset state($token,PINGSTART)
	}

	return ""
    }

    # This procedure deals with NOTICE received from a regular user:

    proc RECV,NOTICE,USER {token nick user comm dest rest} {
	if {[string match "\001*" [lindex $rest 0]]} {
	    set rest [ctcpcleanup $rest]
	    RECV,NOTICE,CTCP,USER $token $nick $user $comm $dest \
		    [lindex $rest 0] [lrange $rest 1 end]
	} else {
	    set rest [eval concat $rest]
	    puts "$nick sends $dest a notice: $rest"
	}

	return ""
    }

    # This procedure helps with CTCP notice actions:

    proc RECV,NOTICE,CTCP,USER {token nick user comm dest action rest} {
	variable state

	if {[info exists state($token,response,ctcp,$action)]} {
	    $state($token,response,ctcp,$action) \
					$token $nick $user $comm $dest \
					$action $rest
	} else {
	    $state($token,response,ctcp,error) \
					$token $nick $user $comm $dest \
					$action $rest
	}

	return ""
    }

    # This procedure deals with JOIN actions:

    proc RECV,JOIN {token nick user comm dest rest} {
	variable state

	# Check if it's us that joined the channel or someone else.

	if {[isus $token $nick]} {
#	    puts "You joined $dest"
          ::orxonox::irc::info $token "You joined $dest"

	    addChannel $token $dest
	} else {
#	    puts "$nick joined $dest"
          ::orxonox::irc::info $token "$nick joined $dest"

	    addToChannel $token $nick $dest
	}

	return ""
    }

    # This procedure deals with PART actions:

    proc RECV,PART {token nick user comm dest rest} {
	variable state

	set chan [string tolower $dest]

	if {[isus $token $nick]} {
#	    puts "You left channel $chan"
          ::orxonox::irc::info $token "You left channel $chan"
	} else {
#	    puts "$nick left [string tolower $dest]"
          ::orxonox::irc::info $token "$nick left [string tolower $dest]"
	}
	removeFromChannel $token $nick $chan
	removeFromChannel $token @$nick $chan

	return ""
    }

    # This procedure deals with MODE actions:

    proc RECV,MODE {token nick user comm dest rest} {
	set rest [eval concat $rest]
#	puts "$nick sets mode $dest $rest"
      ::orxonox::irc::info "$nick $token sets mode $dest $rest"

	return ""
    }

    # This procedure deals with NICK actions:

    proc RECV,NICK {token nick user comm dest rest} {
	variable state

	set newnick [string range $dest 0 end]

	# If our nick changed, remember it as the nick associated with
	# this connection:

	if {[isus $token $nick]} {
	    set state($token,nick) $newnick
	    set state($token,$newnick,PRIVMSG)	$state(PRIVMSG,unsolicited)
	    catch {unset state($token,$nick,PRIVMSG)}
	}

	# Replace the old nick with the new in all channels that we're on:

	replaceAllChannels $token $nick $newnick

#	puts "$nick ${user} ($token) changes his/her nickname to $newnick"
      ::orxonox::irc::info $token "$nick changes his/her nickname to $newnick"

	return ""
    }

    # This procedure deals with QUIT actions:

    proc RECV,QUIT {token nick user comm dest rest} {
	variable state

	set rest [eval concat $rest]
#	puts "Received QUIT $token $nick $rest"
      ::orxonox::irc::info $token "Received QUIT $token $nick $rest"

	if {[string match ":*" $dest]} {
	    set dest [string range $dest 1 end]
	}
	if {[isus $token $nick]} {
#	    puts "You left the server $state($token,host) ($dest $rest)"
          ::orxonox::irc::info $token "You left the server $state($token,host) ($dest $rest)"
	    forgetConnection $token
	} else {
	    puts "$nick quits IRK ($dest $rest)"
	    removeFromAllChannels $token $nick
	    removeFromAllChannels $token @$nick
	}

	return ""
    }

    # This procedure deals with expected PRIVMSG actions:

    proc RECV,PRIVMSG {token nick user comm dest rest} {
	if {[string match "\001*" [lindex $rest 0]]} {
	    set rest [ctcpcleanup $rest]
	    RECV,PRIVMSG,CTCP,CHANNEL $token $nick $user $comm $dest \
		    [lindex $rest 0] [lrange $rest 1 end]
	} else {
#	    puts "$nick$dest: [eval concat $rest]"
          ::orxonox::irc::say $token $nick [eval concat $rest]
	}

	return ""
    }

    # This procedure handles CTCP actions on the channel:

    proc RECV,PRIVMSG,CTCP,CHANNEL {token nick user comm dest action rest} {
	variable state

	if {[info exists state($token,channel,ctcp,$action)]} {
	    $state($token,channel,ctcp,$action) \
					$token $nick $user $comm $dest \
					$action $rest
	} else {
	    $state($token,channel,ctcp,error) \
					$token $nick $user $comm $dest \
					$action $rest
	}

	return ""
    }

    # This procedure stores the result of USERHOST actions:

    proc RECV,USERHOST {token nick user comm dest rest} {
	return ""
    }

    # This procedure stores the channel topic:

    proc RECV,CHANNEL,TOPIC {token nick user comm dest rest} {
	variable state

	set chan [lindex $rest 0]
	set rest [lrange $rest 1 end]
	if {[string match ":*" [lindex $rest 0]]} {
	    set rest [lreplace $rest 0 0 \
			       [string range [lindex $rest 0] 1 end]]
	}
	set state($token,$chan,TOPIC) $rest
	set state($token,$chan,TOPIC,SETBY) $nick
	set state($token,$chan,TOPIC,SETAT) [clock format [clock seconds]]

	return ""
    }

    # This procedure stores the channel byline:

    proc RECV,CHANNEL,SETBY {token nick user comm dest rest} {
	variable state

	set chan [lindex $rest 0]
	set rest [lrange $rest 1 end]
	if {[string match ":*" [lindex $rest 0]]} {
	    set rest [lreplace $rest 0 0 \
			       [string range [lindex $rest 0] 1 end]]
	}
	set state($token,$chan,TOPIC,SETBY) [lindex $rest 0]
	set state($token,$chan,TOPIC,SETAT) [clock format [lindex $rest 1]]

	return ""
    }

    # This procedure deals with unsolicited PRIVMSG actions:

    proc RECV,PRIVMSG,unsolicited {token nick user comm dest rest} {
	if {[string match "\001*" [lindex $rest 0]]} {
	    set rest [ctcpcleanup $rest]
	    RECV,PRIVMSG,CTCP,USER $token $nick $user $comm $dest \
		    [lindex $rest 0] [lrange $rest 1 end]
	} else {
#	    puts "$nick: [eval concat $rest]"
          ::orxonox::irc::privmsg $nick $nick [eval concat $rest]
	}

	return ""
    }

    # This procedure helps with CTCP private messages:

    proc RECV,PRIVMSG,CTCP,USER {token nick user comm dest action rest} {
	variable state

	if {[info exists state($token,cmd,ctcp,$action)]} {
	    $state($token,cmd,ctcp,$action) \
					$token $nick $user $comm $dest \
					$action $rest
	} else {
	    $state($token,cmd,ctcp,error) \
					$token $nick $user $comm $dest \
					$action $rest
	}

	return ""
    }

    # This procedure deals with a KICK action:

    proc RECV,KICK {token nick user comm dest rest} {
	set kicked [lindex $rest 0]
	if {[string match ":*" $kicked]} {
	    set kicked [string range $kicked 1 end]
	}
	set reason [eval concat [lrange $rest 1 end]]
	if {[string match ":*" $reason]} {
	    set reason [string range $reason 1 end]
	}

	if {[isus $token $kicked]} {
#	    puts "$nick kicked you from $dest because $reason"
          ::orxonox::irc::info $token "$nick kicked you from $dest because $reason"

	    removeChannel $token $dest
	} else {
#	    puts "$nick kicks $kicked from $dest because $reason"
          ::orxonox::irc::info $token "$nick kicks $kicked from $dest because $reason"

	    removeFromChannel $token $kicked $dest
	    removeFromChannel $token @$kicked $dest
	}

	return ""
    }

    # These procedures collect the name list for a channel:

    proc RECV,CHANNEL,NAMELIST {token nick user comm dest rest} {
	variable state

	# Scan forward in $rest for the channel name:

	for {set i 0; set l [llength $rest]} {$i < $l} {incr i} {
	    if {[string match "#*" [lindex $rest $i]]} {
		break
	    }
	}

	# Didn't find it?

	if {$i == $l} {
	    return
	}

	# Extract the channel name and the rest of the input:

	set chan [lindex $rest $i]
	set rest [lrange $rest [expr $i + 1] end]
	set rest [lreplace $rest 0 0 [string range [lindex $rest 0] 1 end]]
	set rest [eval concat $rest]

	if {![info exists state($token,$chan,NAMES)]} {
	    set state($token,$chan,NAMES) ""
	}
	set state($token,$chan,NAMES) [concat $state($token,$chan,NAMES) $rest]

	return ""
    }

    proc RECV,CHANNEL,NAMELIST,END {token nick user comm dest rest} {
	variable state

	set chan [lindex $rest 0]
	set $state($token,$chan,NAMES) [split $state($token,$chan,NAMES) " "]
    }

    # This procedure deals with a request from the server to send a PONG
    # with a given code.

    proc RECV,PONG,REQUEST {token nick user comm dest rest} {
	set pongcode [lindex $rest [expr [llength $rest] - 1]]
	puts $token "PONG $pongcode"

	return ""
    }

    # This procedure deals with a CTCP PING request:

    proc RECV,CTCP,PING {token nick user comm dest action rest} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid IRK connection"
	}
	puts $sock "NOTICE $nick :\001PING ${rest}\001"

	return ""
    }

    # This procedure deals with a CTCP TIME request:

    proc RECV,CTCP,TIME {token nick user comm dest action rest} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid IRK connection"
	}
	puts $sock \
	    "NOTICE $nick :\001TIME :[clock format [clock seconds]]\001"

	return ""
    }

    # This procedure deals with a CTCP VERSION request:

    proc RECV,CTCP,VERSION {token nick user comm dest action rest} {
	variable state
	global tcl_platform

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid IRK connection"
	}
	set version "$state(-useragent):$state(-version):$tcl_platform(os)"
	puts $sock "NOTICE $nick :\001VERSION ${version}\001"
    }

    # This procedure deals with a CTCP USERINFO request:

    proc RECV,CTCP,USERINFO {token nick user comm dest action rest} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid IRK connection"
	}
	puts $sock "NOTICE $nick :\001USERINFO $state(-$token,user)\001"
    }

    # This procedure deals with CTCP ACTION messages:

    proc RECV,CTCP,ACTION {token nick user comm dest action rest} {
#	puts "$nick $rest"
      ::orxonox::irc::action $token $nick $rest

	return ""
    }

    # This procedure is a catch all for CTCP actions that we do not
    # understand:

    proc RECV,CTCP,ERROR {token nick user comm dest action rest} {
	variable state

	if {[catch {set sock $state($token,socket)}]} {
	    error "$token: not a valid IRC connection"
	}
	if {[llength $rest] > 0} {
	    puts $sock \
		"NOTICE $nick :\001ERRMSG $action $rest: unknown CTCP\001"
	} else {
	    puts $sock "NOTICE $nick :\001ERRMSG $action: unknown CTCP\001"
	}
    }

    # This is the default action, used by the default dispatcher
    # when no action can be found for the given $token, $nick, $user,
    # $comm, and $dest.

    proc defaultAction {token nick user comm dest rest} {
#	puts "$token: $nick $user: $comm -> $dest ... [eval concat $rest]"
      ::orxonox::execute log "$token: $nick $user: $comm -> $dest ... [eval concat $rest]"

	return ""
    }
}
