# irkstate.tcl:
#
# This file defines the contents of the ::irk::state array

namespace eval ::irk {

    # The state array contains all the state for the IRK package:
    #
    # These options can be configured with irk::config:
    #
    # state(-useragent)		Name of this IRK user agent.
    # state(-version)		Version of this IRK user agent.
    #
    # state(-sockproxy)		Name of SOCKS5 proxy to use (optional).
    # state(-socksport)		Port on which SOCKS5 server accepts
    #				connections (optional).
    #
    # state(-$symsrv,nick)	If set and connect is called without giving
    #				a nick, use this as the nickname.
    # state(-$symsrv,user)	If set and connect is called without giving
    #				a user, this is used. Otherwise, if it's not
    #				set when connect is called without a user,
    #				it is set to the given nickname (and used).
    # state(-$symsrv,pass)	If set, and connect is called without giving
    #				a password, this is used.
    #
    # state(-quitmessage)	If set, the message is sent when a connection
    #				is closed, as part of a QUIT command.
    #
    # These options cannot be configured with irk::config:
    #
    # state(connections)	A list of all active connections.
    # state(dispatcher)		The default command dispatcher (must be set).
    # state(action)		The default action (must be set).
    # state(servers)		List of known servers (further described in
    #				the symsrv array below).
    #
    # The following state is kept per connection (identified by $contok):
    #
    # state($contok,socket)	The socket for a given connection
    # state($contok,port)	The port of a given connection
    # state($contok,host)	The host of a given connection
    # state($contok,symsrv)	The symbolic name of the server for
    #				this connection.
    # state($contok,nick)	The nick name used on a given connection
    # state($contok,user)	The real user used on a given connection
    # state($contok,pass)	The password used on a given connection
    # state($contok,disp)	The command dispatcher for the connection
    #
    # Depending on the dispatcher associated with each connection, there
    # may be many more settings of the general form:
    #
    # state($contok,action,...)
    #
    # System wide responses to common commands are handled by
    # special actions, stored in the state array:
    #
    # state(PRIVMSG,unsolicited)
    #				Action that responds to unsolicited private
    #				messages directed at this user's nickname.
    # state(special,PING)	Responds to PING messages.
    # state(special,NOTICE)	Responds to NOTICE messages.
    # state(special,ERROR)	Responds to ERROR messages.
    #
    # The IRK library provides a unified authorization mechanism, implemented
    # in irkauth.tcl. The state array contains two settings that affect the
    # behavior of the authorization mechanism:
    #
    # state(auth,save,file)	Name of a file to save the authorization
    #				information in. Defaults to auth.dat in the
    #				directory containing the IRK package.
    # state(auth,save,interval)	The number of milliseconds that pass between
    #				autosaves of the authorization data.
    
    variable state
    array set state {
	-useragent		"Tcl IRK Library"
	-version		0.1
	-quitmessage		"gives up the ghost"

	dispatcher		::irk::defaultDispatcher
	action			::irk::defaultAction

	cmd,QUIT		::irk::RECV,QUIT
	cmd,JOIN		::irk::RECV,JOIN
	cmd,PART		::irk::RECV,PART
	cmd,MODE		::irk::RECV,MODE
	cmd,NICK		::irk::RECV,NICK
	cmd,PONG		::irk::RECV,PONG
	cmd,PRIVMSG		::irk::RECV,PRIVMSG
	cmd,NOTICE		::irk::RECV,NOTICE,USER
	cmd,KICK		::irk::RECV,KICK
	cmd,001			::irk::RECV,MOTD
	cmd,002			::irk::RECV,MOTD
	cmd,003			::irk::RECV,MOTD
	cmd,004			::irk::RECV,MOTD
	cmd,005			::irk::RECV,MOTD
	cmd,250			::irk::RECV,MOTD
	cmd,251			::irk::RECV,MOTD
	cmd,252			::irk::RECV,MOTD
	cmd,253			::irk::RECV,MOTD
	cmd,254			::irk::RECV,MOTD
	cmd,255			::irk::RECV,MOTD
	cmd,265			::irk::RECV,MOTD
	cmd,266			::irk::RECV,MOTD
	cmd,301			::irk::RECV,AWAY
	cmd,307			::irk::RECV,WHOIS,NICK,IDENT
	cmd,311			::irk::RECV,WHOIS,NICK,USER
	cmd,312			::irk::RECV,WHOIS,NICK,SERVER
	cmd,317			::irk::RECV,WHOIS,NICK,CONNECTTIME
	cmd,318			::irk::RECV,WHOIS,NICK,END
	cmd,319			::irk::RECV,WHOIS,NICK,CHANNELS
	cmd,372			::irk::RECV,MOTD
	cmd,375			::irk::RECV,MOTD
	cmd,376			::irk::RECV,MOTD
	cmd,332			::irk::RECV,CHANNEL,TOPIC
	cmd,333			::irk::RECV,CHANNEL,SETBY
	cmd,353			::irk::RECV,CHANNEL,NAMELIST
	cmd,366			::irk::RECV,CHANNEL,NAMELIST,END

	cmd,513			::irk::RECV,PONG,REQUEST

	special,PING		::irk::RECV,PING
	special,NOTICE		::irk::RECV,NOTICE
	special,ERROR		::irk::RECV,ERROR

	PRIVMSG,unsolicited	::irk::RECV,PRIVMSG,unsolicited

	errorhandler		::irk::echoerror

	auth,save,file		[file join [file dir [info script]] auth.dat]
	auth,save,interval	300000
    }
}
