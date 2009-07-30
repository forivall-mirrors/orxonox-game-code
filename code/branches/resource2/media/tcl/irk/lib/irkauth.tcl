# irkauth.tcl:
#
# Various procedures that deal with user authentication:

namespace eval ::irk {

    # The AUTH module keeps information about each user and facility ($fac)
    # in the state array:
    #
    # auth(auth,$fac,$user)		If set, then $user has authenticated
    #					successfully for the facility $fac.
    # auth(user,$fac,$user,pass)	The password for this $user and $fac.
    # auth(user,$fac,$user,ident)	The ident token for this $user & $fac.
    #
    # auth(ident,$fac,$ident,pass)	The password for this $ident and $fac.
    # auth(ident,$fac,$ident,user)	The user for this $ident and $fac.
    #
    # auth(identcounter,$fac)		The ident token counter for $fac. This
    #					is incremented each time a new user
    #					establishes themselves with $fac.

    # This procedure can be called by programs using the IRK library
    # when a user sends a password.

    proc pass {token fac nick user rest} {
	variable auth

	if {[llength $rest] != 1} {
	    # Incorrect syntax:

	    set reply "$nick, syntax is !pass <yourpass> (one word, no spaces)"
	} elseif {[info exists auth(user,$fac,$user,pass)]} {

	    # If this user has already established a password,
	    # check that they're giving the right password.

	    if {[string compare $auth(user,$fac,$user,pass) \
				[lindex $rest 0]]} {
		set reply "$nick, sorry, wrong password!"
	    } else {
		set reply "$nick, thanks for entering your password!"
		set auth(auth,$fac,$user) 1
	    }
	} else {

	    # This is the first time we're seeing this user. Accept
	    # their password and send them an ident token. They can
	    # use the ident token to reestablish themselves when their
	    # user mask changes significantly.

	    if {![info exists auth(identcounter,$fac)]} {
		set auth(identcounter,$fac) 0
	    }
	    set ident $auth(identcounter,$fac)
	    incr auth(identcounter,$fac)

	    set auth(ident,$fac,$ident,user) $user
	    set auth(ident,$fac,$ident,pass) [lindex $rest 0]

	    set auth(user,$fac,$user,ident) $ident
	    set auth(user,$fac,$user,pass) [lindex $rest 0]

	    # Save the changes

	    saveauth

	    # Save them a step and also authorize them:

	    set auth(ident,$fac,$user) 1

	    set reply [list \
	        $nick, your password is [lindex $rest 0]. Your ident is \
	        $ident, write it down, you will need it later to \
		reidentify yourself if your user mask changes. \
		You user mask is currently $user. You are now authorised \
		to use $fac.]
	}

	# Tell them what happened:

	::irk::say $token $nick $reply

	return ""
    }

    # This procedure can be called by programs when the user attempts to
    # reestablish themselves with the existing ident and password.

    proc id {token fac nick user rest} {
	variable auth

	set len [llength $rest]
	set reply "Wrong syntax. Call !ident or !ident <ident> <pass>"

	if {$len == 0} {

	    # Calling ident with zero arguments. The user is trying to
	    # retrieve their ident. Give it to them only if they did
	    # identify successfully with the correct password.

	    if {![info exists auth(user,$fac,$user,pass)]} {
		set reply "$nick, first set a password"
	    } elseif {[info exists auth(auth,$fac,$user)]} {
		set reply \
		    "$nick, your ident is $auth(user,$fac,$user,ident)"
	    } else {
		set reply \
		   "$nick, identify with password before getting your ident!"
	    }
	} elseif {$len == 2} {

	    # Calling ident with two arguments. The user is trying to
	    # establish a new value for $user to associate with this
	    # ident and password. If $auth($ident,pass) is the password
	    # she gave, then they're the rightfull owner of the ident and
	    # so we now recognize the new $user mask.

	    set ident [lindex $rest 0]
	    set pass [lindex $rest 1]

	    if {[info exists auth(ident,$fac,$ident,pass)]} {
		if {![string compare $auth(ident,$fac,$ident,pass) $pass]} {

		    # Identify the old user mask they were using:

		    set olduser $auth(ident,$fac,$ident,user)

		    # Clean up the state associated with the old mask:

		    array unset auth user,$fac,$olduser,*
		    catch {unset auth(ident,$face,$olduser)}

		    # Link up the new state:

		    set auth(ident,$fac,$ident,user) $user

		    set auth(user,$fac,$user,ident) $ident
		    set auth(user,$fac,$user,pass) $pass

		    # Save the changes

		    saveauth

		    # Save them a step and also treat them as authenticated:

		    set auth(ident,$fac,$user) 1

		    set reply \
			    "OK, $nick, I'm now recognising you as $user.\
			     You are now authorised to use $fac."
		} else {
		    set reply "$nick, sorry, wrong ident or password"
		}
	    } else {
		set reply "$nick, sorry, wrong ident or password"
	    }
	}

	# Tell them what happened:

	::irk::say $token $nick $reply

	return ""
    }

    # This procedure can be invoked by a program when a user tries to
    # change her password.

    proc np {token fac nick user rest} {
	variable auth

	set reply "Wrong syntax. Call !newpass <oldpass> <newpass>"

	if {[llength $rest] == 2} {
	    set opw [lindex $rest 0]
	    set npw [lindex $rest 1]

	    if {![info exists auth(user,$fac,$user,pass)]} {
		# Unknown $user, probably their user mask changed. Help
		# them reestablish the connection.

		set reply \
		   [list $nick, I don't have you in my database. Perhaps \
			 your user mask changed drastically. If so, please \
			 reestablish your user mask by using !ident <ident> \
			 <oldpass>.]
	    } elseif {[string compare $auth(user,$fac,$user,pass) $opw]} {
		# Wrong old password!

		set reply "$nick, sorry, wrong old password!"
	    } else {
		# Their user mask matches and they gave the correct old
		# password, so we accept their new password:

		set ident $auth(user,$fac,$user,ident)

		set auth(ident,$fac,$ident,pass) $npw
		set auth(user,$fac,$user,pass) $npw

		# Save the changes:

		saveauth

		# Save them a step by also recording that they
		# authenticated:

		set auth(auth,$fac,$user) 1

		set reply "OK, $nick, your new password is now $npw"
	    }
	}

	# Tell them what happened:

	::irk::say $token $nick $reply

	return ""
    }

    # This procedure can be called by programs when the user wants to
    # "log out" or lose her authentication with a given facility:

    proc logout {token fac nick user rest} {
	variable auth

	set reply "You were not logged into $fac. Now you certainly aren't."

	if {[info exists auth(auth,$fac,$user)]} {
	    unset auth(auth,$fac,$user)

	    set reply \
		[list $nick, you logged out successfully from $fac. Thank you \
		      for using $fac.]
	}

	# Tell them what happened:

	::irk::say $token $nick $reply

	return ""
    }

    # Is the user authenticated with the given facility?

    proc userauthenticated {fac user} {
	variable auth

	# If auth(auth,$fac,$user) exists, then she is authenticated.

	if {[info exists auth(auth,$fac,$user)]} {
	    return 1
	}
	return 0
    }

    # This procedure automatically saves the authorization database:

    proc saveauth {} {
	variable state
	variable auth

	puts "Saving!"

	# Define the patterns to save:

	set p1 "identcounter,*"
	set p2 "user,*"
	set p3 "ident,*"

	# Try to open the save file:

	if {[info exists state(auth,save,file)]} {
	    if {![catch {set fd [open $state(auth,save,file) w]}]} {
		puts $fd "array set ::irk::auth [list [array get auth $p1]]"
		puts $fd "array set ::irk::auth [list [array get auth $p2]]"
		puts $fd "array set ::irk::auth [list [array get auth $p3]]"

		catch {close $fd}
	    }
	}
    }

    # This procedure restores the authorization database:

    proc restoreauth {} {
	variable state

	if {[info exists state(auth,save,file)]} {
	    catch {uplevel #0 source $state(auth,save,file)}
	}
	set state(auth,restored) 1
    }

    # If this is the first time we're loading the IRK package, then
    # restore the authorization database. Otherwise we'd be overwriting
    # a potentially unsaved state.

    variable state

    if {![info exists state(auth,restored)]} {
	restoreauth
    }
}
