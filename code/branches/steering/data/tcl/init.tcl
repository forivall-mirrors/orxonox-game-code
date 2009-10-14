# Check if Tcl was properly initialized
info library

# Create orxonox namespace
namespace eval orxonox {}

# query --
# Sends a query to the CommandExecutor of Orxonox and waits for the response.
# This dummy procedure will be changed to it's real implementation by Orxonox itself.
#
# Arguments:
# args - The command to send to Orxonox

proc query args {
    return -code error "Can't query Orxonox now"
}


# crossquery --
# Sends a query to another Tcl-interpreter in Orxonox and waits for the response.
# This dummy procedure will be changed to it's real implementation by Orxonox itself.
#
# Arguments:
# id   - The ID of the other interpreter
# args - The command to send to Orxonox

proc crossquery {id args} {
    return -code error "Can't query interpreter with ID $id now"
}


# execute --
# Sends a command to the queue of Orxonox where it will be executed by the CommandExecutor after some time
# This dummy procedure will be changed to it's real implementation by Orxonox itself.
#
# Arguments:
# args - The command

proc execute args {
    return -code error "Can't execute a command now"
}


# crossexecute --
# Sends a command to the queue of another Tcl-interpreter where it will be executed by after some time
# This dummy procedure will be changed to it's real implementation by Orxonox itself.
#
# Arguments:
# id   - The ID of the other interpreter
# args - The command

proc crossexecute {id args} {
    return -code error "Can't execute a command now"
}


# running --
# Returns true if the interpreter is still suposed to be running
# This dummy procedure will be changed to it's real implementation by Orxonox itself.

proc running {} {
    return 1
}


# orxonox::while --
# Works like while but breaks the loop if orxonox::running returns false

proc ::orxonox::while {condition body} {
    set condition_cmd [list expr $condition]
    ::tcl::while {1} {
        if {![uplevel 1 $condition_cmd] || ![::running]} {
            break
        }
        uplevel 1 $body
    }
}


# orxonox::for --
# Works like for but breaks the loop if orxonox::running returns false

proc ::orxonox::for {start condition step body} {
    set condition_cmd [list expr $condition]
    uplevel 1 $start
    ::tcl::while {1} {
        if {![uplevel 1 $condition_cmd] || ![::running]} {
            break
        }
        uplevel 1 $body
        uplevel 1 $step
    }
}


# add the path to this file to the auto path

set filepath [info script]
#set ::orxonox::mediapath [string range $filepath 0 [string last "/" $filepath]]
set ::orxonox::mediapath [file dirname $filepath]
if {[lsearch $auto_path $::orxonox::mediapath] == -1} {
    lappend auto_path $::orxonox::mediapath
}
unset filepath


# save the start directory and the library directory

proc psd {} "return [pwd]"
proc pld {} "return $::orxonox::mediapath"

set pwd [pwd]
set psd [psd]
set pld [pld]


# modify cd to automatically set $pwd

if {[llength [info command ::tcl::cd]] == 0} {
    rename cd ::tcl::cd
}
proc cd {{path "~"}} {
    global pwd
    ::tcl::cd $path
    set pwd [pwd]
}


# change the working directory to the media path

cd $::orxonox::mediapath


# Redefines puts to write directly into the Orxonox console if the channel is stdout or stderr.

if {[llength [info command ::tcl::puts]] == 0} {
    rename puts ::tcl::puts
}
proc puts args {
    set argc [llength $args]
    if {$argc < 1 || $argc > 3} {
        error "wrong # args: should be \"puts ?-nonewline? ?channelId? string\""
    }

    set newline 1
    set input $args

    if {$argc > 1 && [lindex $input 0] == "-nonewline"} {
        set newline 0
        set input [lrange $input 1 end]
    } elseif {$argc == 3} {
        if {[lindex $input 2] == "nonewline"} {
            set newline 0
            set input [lrange $input 0 1]
        } else {
            error "bad argument \"[lindex $input 2]\": should be \"nonewline\""
        }
    }

    if {[llength $input] == 1} {
        set input [list stdout [join $input]]
    }

    foreach {channel s} $input break

    if {$channel == "stdout" || $channel == "stderr"} {
        execute puts $newline $s
    } else {
        eval [concat ::tcl::puts $args]
    }
}


# Redefines unknown to send unknown commands back to orxonox

if {[llength [info commands unknown]] != 0} {
    # check if a command named "undefined_proc" exists, if yes rename it temporarily
    set undefined_was_defined 0
    if {[llength [info commands undefined_proc]] != 0} {
        set undefined_was_defined 0
        rename undefined_proc _undefined
    }

    # get the returned errormessage if an undefined_proc command is called
    if {[llength [info commands ::tcl::unknown]] == 0} {
        set errorcode [catch {unknown undefined_proc} result]
    } else {
        set errorcode [catch {::tcl::unknown undefined_proc} result]
    }

    if {$errorcode} {
        set result_list [split $result]
        set ::orxonox::errormessage_unknown [list]

        # parse the error message (the original message was something like "invalid command name "undefined_proc"" but we just want "invalid command name")
        foreach token $result_list {
            if {![string match "*undefined_proc*" $token]} {
                lappend ::orxonox::errormessage_unknown $token
            }
        }

        unset result_list
        unset token

        set ::orxonox::errormessage_unknown_length [llength $::orxonox::errormessage_unknown]

        # rename the original unknown procedure
        if {[llength [info commands ::tcl::unknown]] == 0} {
            rename unknown ::tcl::unknown
        }

        # define the modified version of unknown
        # we implement two versions, because tcl8.4 complains if this fork is moved inside [unknown]
        if {[info tclversion] >= 8.5} {
            # version >= 8.5
            proc unknown args {
                global ::orxonox::errormessage_unknown ::orxonox::errormessage_unknown_length

                set cmd [concat ::tcl::unknown $args]
                set errorcode [catch {eval $cmd} result options]
                set resultlist [split $result]
                set success 1

                if {$errorcode && [llength $resultlist] >= $::orxonox::errormessage_unknown_length} {
                    for {set i 0} {$i < $::orxonox::errormessage_unknown_length} {incr i} {
                        if {[lindex $::orxonox::errormessage_unknown $i] != [lindex $resultlist $i]} {
                            set success 0
                            break
                        }
                    }
                } else {
                    set success 0
                }

                if {!$success} {
                    return -code $errorcode -options $options $result
                } else {
                    return [query $args]
                }
            }
        } else {
            # version < 8.5
            proc unknown args {
                global ::orxonox::errormessage_unknown ::orxonox::errormessage_unknown_length

                set cmd [concat ::tcl::unknown $args]
                set errorcode [catch {eval $cmd} result]
                set resultlist [split $result]
                set success 1

                if {$errorcode && [llength $resultlist] >= $::orxonox::errormessage_unknown_length} {
                    for {set i 0} {$i < $::orxonox::errormessage_unknown_length} {incr i} {
                        if {[lindex $::orxonox::errormessage_unknown $i] != [lindex $resultlist $i]} {
                            set success 0
                            break
                        }
                    }
                } else {
                    set success 0
                }

                if {!$success} {
                    return -code $errorcode $result
                } else {
                    return [query $args]
                }
            }
        }

        set success 1
    } else {
        set success 0
    }

    unset errorcode
    unset result

    # if the "undefined_proc" command was renamed previously, undo this
    if {$undefined_was_defined} {
        rename _undefined undefined_proc
    }

    unset undefined_was_defined

    if {!$success} {
        unset success
        # something went wrong, use the default method
        proc unknown args {
            return [query $args]
        }
    }
    unset success
} else {
    # no original unknown procedure defined, use the default method
    proc unknown args {
        return [query $args]
    }
}
