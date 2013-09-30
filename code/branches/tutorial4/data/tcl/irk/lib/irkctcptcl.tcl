# irkctcptcl.tcl:
#
# This file implements the IRK CTCP TCL protocol:

namespace eval ::irk {

    # This procedure sets up a safe interpreter for the channel in which
    # the CTCP TCL action occurs:

    proc setupInterp {token chan} {
	variable state

	if {![info exists state($token,$chan,interp)]} {
	    catch {interp delete ${token}${chan}}
	    set state($token,$chan,interp) [safe::interpCreate ${token}${chan}]
	    safe::loadTk $state($token,$chan,interp)
	}
	if {![interp exists ${token}${chan}]} {
	    set state($token,$chan,interp) [safe::interpCreate ${token}${chan}]
	    safe::loadTk $state($token,$chan,interp)
	}
    }

    # This procedure dispatches the Tcl command embedded within the
    # CTCP TCL action to the associated interpreter:

    proc RECV,CTCP,TCL {token nick user comm dest action rest} {
	variable state

	puts stderr "Tcl command is: $rest [llength $rest]"
	setupInterp $token $dest
	interp eval $state($token,$dest,interp) [::join $rest]
    }

    # This procedure dispatches the Tcl command embedded within the
    # CTCP TCL action for local execution on the issuer's side:

    proc RECV,CTCP,TCL,LOCAL {token nick user comm dest action rest} {
	variable state

	puts stderr "Tcl command is: $rest [llength $rest]"
	setupInterp $token $dest
	interp eval $state($token,$dest,interp) $rest
    }
}
