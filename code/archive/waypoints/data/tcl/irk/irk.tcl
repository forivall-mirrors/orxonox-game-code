# IRK: A client library for IRC programming written in Tcl.

# We need the socks package to get through firewalls:

package require socks

# We provide the IRK package:

package provide irk 0.1

set thisdir [file dir [info script]]

# Source the rest of the package:

source [file join $thisdir lib irkstate.tcl]
source [file join $thisdir lib irkconnect.tcl]
source [file join $thisdir lib irkdispatch.tcl]
source [file join $thisdir lib irkreceive.tcl]
source [file join $thisdir lib irkcommand.tcl]
source [file join $thisdir lib irkflow.tcl]
source [file join $thisdir lib irkutil.tcl]
source [file join $thisdir lib irkctcptcl.tcl]
source [file join $thisdir lib irkauth.tcl]
source [file join $thisdir lib irkservers.tcl]
