# Socks5 Client Library v1.2
#
# Original author: (C)2000 Kerem 'Waster_' HADIMLI
#
# Author contact information:
#   E-mail :  waster@iname.com
#   ICQ#   :  27216346
#   Jabber :  waster@jabber.org   (New IM System - http://www.jabber.org)
#
# Cleaned up a little by Jacob Levy, jyl@best.com (01/07/2001)
#
# * Moved everything into the ::socks:: namespace.
# * Parameterized so that constants do not get reset for every connection.
# * Uniform error handling and cleanup.
# * Incremented the version number to 1.2.
#
# How to use:
#
# Step 1: Create your client socket connected to the SOCKS5 server.
#
# Step 2: Call socks::init sock host port ?auth? ?user? ?pass?
#    where:
#
#	sock		The socket from step #1.
#	host		The remote host to connect to through the proxy.
#	port		The remote port to connect to through the proxy.
#	auth		Optional. If 1, it means that we do an
#			authentication with the proxy server.
#	user		Optional. If present, the user name to use for
#			authentication with the proxy server.
#	pass		Optional. If present, the password for the user
#			name to use for authentication with the
#			proxy server.
#
# If auth == 1, user and pass are not present, and socks::state(username)
# and socks::state(pass) are set, then socks::init uses the values from
# socks::state(username) and socks::state(password) for user name and
# password, respectively.
#
# If anything goes wrong during the attempt to connect through the
# proxy server, socks::init throws an appropriate error and closes
# the socket. If the operation succeeds, socks::init returns $sock.
#
# You can set up your socket with fconfigure before calling socks::init,
# the socks::init procedure is careful to restore the state to what it
# was before the call upon successful completion. Likewise, you can
# have fileevent handlers installed for handling socket readable and
# socket writable events. These will also be preserved.

# This file provides the "socks" package:

package provide socks 1.2

# Global settings:

namespace eval ::socks {
    variable state

    array set state {
	protocolversion		"\x05"
	cmdconnect		"\x01"
	addresstype		"\x03"
	reserved		"\x00"

	noauthmethod		"\x00"
	noauthlengthstr		"\x01"

	authmethod		"\x00\x02"
	authlengthstr		"\x02"

	nomatchingmethod	"\xFF"
	nomatchlengthstr	"\x01"

	authenticationversion	"\x01"

	errorheader		"Error: "
	error1			"General SOCKS server failure"
	error2			"Connection not allowed by ruleset"
	error3			"Network unreachable"
	error4			"Host unreachable"
	error5			"Connection refused"
	error6			"TTL expired"
	error7			"Command not supported"
	error8			"Address type not supported"
	errorUnknown		"Unknown error"

	errorDisconnect		"SOCKS server disconnected"
	errorNotSOCKS5		"SOCKS server does not support SOCKS 5"
	errorMustAuthenticate	"SOCKS server demands user/pass authentication"
	errorAuthMethodNotSup	"SOCKS server doesn't support user/pass auth"
	errorWrongUserPass	"Wrong user name or password"
	errorIncorrectAuthVal	"Incorrect value for $auth, expecting 1 or 0"
	errorMissingUserAndPass	"Missing user and pass, required for auth == 1"
    }
}

# Error handling and cleanup:

proc ::socks::Error {sock errorMsg} {
    variable state

    # Attempt to close the socket. This also cancels any installed
    # fileevent handlers, so no need to do that explicitly.

    catch {close $sock}

    # Clean up the state we keep about this socket:

    catch {array unset state $sock,*}

    # Report the requested error:

    error "$state(errorheader)$state($errorMsg)"
}
    
# Main entry point: socks::init
#
# See comment at head of file for how to use.

proc ::socks::init {sock addr port {auth 0} {user {}} {pass {}}} {
    variable state

    # Save current configuration state for $sock

    set currentConfiguration [fconfigure $sock]

    # We cannot configure -peername so if its present (it should be)
    # then remove it from the list of options.

    set idx [lsearch $currentConfiguration -peername]
    if {$idx != -1} {
	set currentConfiguration \
	    [lreplace $currentConfiguration $idx [expr $idx + 1]]
    }

    # Same for -sockname.

    set idx [lsearch $currentConfiguration -sockname]
    if {$idx != -1} {
	set currentConfiguration \
	    [lreplace $currentConfiguration $idx [expr $idx + 1]]
    }

    # Save any currently installed handler for fileevent readable:

    set currentReadableHandler [fileevent $sock readable]

    # If the user has "set and forget" user name and password, and
    # indicates that she wants to use them, use them now:

    if {($auth == 1) && (![string compare {} $user]) \
	    && (![string compare {} $pass])} {
	if {[info exists state(username)] && [info exists state(password)]} {
	    set auth 1
	    set user $state(username)
	    set pass $state(password)
	} else {
	    Error $sock errorMissingUserAndPass
	}
    }

    # Figure out the authentication method to use:

    if {$auth == 0} {
	set nmethods $state(noauthlengthstr)
	set method $state(noauthmethod)
    } elseif {$auth == 1} {
	set nmethods $state(authlengthstr)
	set method $state(authmethod)
    } else {
	Error $sock errorIncorrectAuthVal
    }

    # Encode the length of the address given (binary 1 byte):

    set domainlen "[binary format c [string length $addr]]"

    # Encode the port (network byte order, 2 bytes):

    set port [binary format S $port]

    if {$auth == 1} {
	# Encode the length of the user given (binary 1 byte):

	set userlen "[binary format c [string length $user]]"

	# Encode the length of the password given (binary 1 byte):

	set passlen "[binary format c [string length $pass]]"
    }

    # Set up initial state for the given socket:

    set state($sock,state) $sock
    set state($sock,data) ""

    # Prepare the socket:

    fconfigure $sock -translation {binary binary} -blocking 0
    fileevent $sock readable "::socks::readable $sock"

    # Tell the server what version and authentication method we're using:

    puts -nonewline $sock "$state(protocolversion)$nmethods$method"
    flush $sock

    # Wait for server response and retrieve the information sent by the
    # server:

    vwait ::socks::state($sock,state)
    set serverReply $state($sock,data)

    if {[eof $sock]} {
	Error $sock errorDisconnect
    }

    # Analyze the server's reply:

    set serverVersion ""
    set serverMethod $state(nomatchingmethod)

    binary scan $serverReply "cc" serverVersion serverMethod

    # Check for various error conditions:

    if {$serverVersion != 5} {

	# Server does not support SOCKS5 protocol

	Error $sock errorNotSOCKS5
    }

    # If server demands authentication, do that step now:

    if {$serverMethod == 2} {

	if {$auth == 0} {

	    # We didn't supply user/pass but server wants us to authenticate:

	    Error $sock errorMustAuthenticate
	}

	puts -nonewline $sock \
	    "$state(authenticationversion)$userlen$user$passlen$pass"
	flush $sock

	# Wait again for server reply:

	vwait ::socks::state($sock,state)
	set serverReply $state($sock,data)

	# Analyze the server reply:

	set authenticationVersion ""
	set serverStatus "\x00"

	binary scan $serverReply "cc" authenticationVersion serverStatus

	# Deal with errors:

	if {$authenticationVersion != 1} {

	    # Server does not support our user/pass authentication method:

	    Error $sock errorAuthMethodNotSup
	}

	if {$serverStatus != 0} {

	    # We supplied wrong user/pass combination:

	    Error $Sock errorWrongUserPass
	}
    } elseif {$serverMethod != 0} {

	# Unknown method. Clean up:

	Error $sock errorUnsupportedMethod
    }

    # Finally tell the server to connect us to the requested host and port:

    puts -nonewline $sock \
	"$state(protocolversion)$state(cmdconnect)$state(reserved)"
    puts -nonewline $sock "$state(addresstype)$domainlen$addr$port"
    flush $sock

    # Wait again for server response:

    vwait ::socks::state($sock,state)
    set serverReply $state($sock,data)

    if {[eof $sock]} {
	Error $sock errorDisconnect
    }

    # Analyze server reply:

    set serverVersion ""
    set serverReplyCode ""

    binary scan $serverReply "cc" serverVersion serverReplyCode

    # Deal with errors:

    if {$serverVersion != 5} {
	Error $sock errorNotSOCKS5
    }

    if {$serverReplyCode != 0} {
	if {($serverReplyCode > 0) && ($serverReplyCode < 9)} {
	    Error $sock error$serverReplyCode
	}
	Error $sock errorUnknown
    }

    # All done, clean up state, reconfigure $sock to its original state,
    # remove our fileevent handler and potentially restore the original
    # one if one was present.

    fileevent $sock readable {}
    if {[string compare $currentReadableHandler ""]} {
	fileevent $sock readable $currentReadableHandler
    }
    catch {eval fconfigure $sock $currentConfiguration}
    catch {array unset state $sock,*}

    # For good measure return the socket:

    return $sock
}

# This procedure reads input available from the server socket and then
# changes a state variable so that the main program will be woken up.

proc ::socks::readable {sock} {
    variable state

    # Wake up the vwait:

    set state($sock,state) $sock

    # Read the data:

    set state($sock,data) [read $sock]
}
