# irkservers.tcl:
#
# This file defines the servers that are known to the Tcl IRK library.
# It is actually a Tcl script and is source'd by the irk.tcl file.

namespace eval ::irk {

    # This procedure helps define a new server:

    proc defineserver {symname host ports} {
	variable state
	variable symsrv

	# Auto initialize the list of known (symbolic names for) servers:

	if {![info exists state(servers)]} {
	    set state(servers) {}
	}

	# Add an entry in the list of (symbolic names for) servers:

	lappend state(servers) $symname

	# Add an entry for the contact information of this symbolic server:

	set symsrv($symname) [list $host $ports]
    }

    # OK, now define a bunch of servers. This is where you can add your
    # own, or edit if things change:

    defineserver twisted	twisted.ma.us.dal.net {6660 6661 6662 6663}
    defineserver dalnet		irc.dal.net			6667
    defineserver undernet	McLean.VA.us.undernet.org	{6667 6668}
    defineserver efnet		irc.Prison.NET			6666
    defineserver ircnet		irc.stealth.net			{6663 6664}
    defineserver powersurf	irc.powersurfr.com		6667
    defineserver coins		coins.dal.net	{6667 6668 6669 7000}
    defineserver sodre		sodre.on.ca.dal.net {6667 6668 6669 7000}
    defineserver austnet	au.austnet.org 6667
    defineserver nznet		nz.austnet.org 6667
    defineserver sgnet		sg.austnet.org 6667
    defineserver us.efnet	us.rr.efnet.net 6667
    defineserver eu.efnet	eu.rr.efnet.net 6667
    defineserver ca.efnet	ca.rr.efnet.net 6667
    defineserver au.efnet	au.rr.efnet.net 6667
    defineserver us.ircnet	us.ircnet.org {6665 6666 6667 6668}
    defineserver eu.ircnet	eu.ircnet.org {6665 6666 6667 6668}
    defineserver ca.ircnet	ca.ircnet.org {6665 6666 6667 6668}
    defineserver au.ircnet	au.ircnet.org {6665 6666 6667 6668}
    defineserver us.undernet	us.undernet.org 6667
    defineserver eu.undernet	eu.undernet.org 6667
    defineserver othernet	irc.othernet.org 6667
    defineserver de.quakenet  de.quakenet.org 6667
    defineserver datacore     irc.datacore.ch 6667
    defineserver orxonox      irc.orxonox.net 6667
}
