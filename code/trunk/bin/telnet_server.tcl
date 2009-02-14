 #!/usr/bin/env tclsh
 # Pseudo-telnet server.  Includes basic auth, but no separate identities
 # or proper multi-threaded operation, so whoever runs this had better
 # trust those he gives identities/passwords to and they had better trust
 # each other too.  Note this script does not support command-line arguments.

 ## The names of this array are IP addresses of hosts that are not permitted
 ## to connect to any of our services.  Admin account(s) can change this
 ## at run-time, though this info is not maintained across whole-server shutdowns.
 array set denyHosts {}

 ## Keep the Tcl-thread busy
 proc every {ms body} { eval $body; after $ms [list every $ms $body] }
 every 200 {}

 ## Create a server on the given port with the given name/password map
 ## and the given core interaction handler.
 proc telnetServer {port {passmap} {handlerCmd remoteCommand}} {
     if {$port == 0} {
         return -code error "Only non-zero port numbers are supported"
     }
     set server [socket -server [list connect $port $handlerCmd] $port]
     global passwords services
     foreach {id pass} $passmap {set passwords($port,$id) $pass}
     set services($server) $handlerCmd
     return $server
 }

 ## Removes the server on the given port, cleaning up the extra state too.
 proc closedownServer {server} {
     global services passwords connections auth
     set port [lindex [fconfigure $server -sockname] 2]
     catch {close $server}
     unset services($server)
     foreach passmap [array names passwords $port,*] {
         unset passwords($passmap)
     }
     # Hmph!  Have to remove unauthorized connections too, though any
     # connection which has been authorized can continue safely.
     foreach {client data} [array get connections] {
         if {$port == [lindex $data 0] && !$auth($client)} {
             disconnect $client
         }
     }
 }

 ## Handle an incoming connection to the given server
 proc connect {serverport handlerCmd client clienthost clientport} {
     global auth cmd denyHosts connections
     if {[info exist denyHosts($clienthost)]} {
         puts stdout "${clienthost}:${clientport} attempted connection"
         catch {puts $client "Connection denied"}
         catch {close $client}
         return
     }
     puts stdout "${clienthost}:${clientport} connected on $client"
     fileevent $client readable "handle $serverport $client"
     set auth($client) 0
     set cmd($client) $handlerCmd
     set connections($client) [list $serverport $clienthost $clientport]
     fconfigure $client -buffering none
     catch {puts -nonewline $client "Login: "}
 }

 ## Disconnect the given client, cleaning up any connection-specific data
 proc disconnect {client} {
     catch {close $client}
     global auth cmd connections
     unset auth($client)
     unset cmd($client)
     unset connections($client)
     puts stdout "$client disconnected"
 }

 ## Handle data sent from the client.  Log-in is handled directly by this
 ## procedure, and requires the name and password on the same line
 proc handle {serverport client} {
     global passwords auth cmd
     if {[gets $client line] < 0} {
         disconnect $client
         return
     }
     if {[string equal $line "logout"] || [string equal $line "quit"]} {
         disconnect $client
         return
     }
     if {[string equal $line "exit"]} {
         set ::termination 1
         return
     }
     if {$auth($client)} {
         eval $cmd($client) [list $client $line 0]
         eval $cmd($client) [list $client $line 1]
         return
     }
     foreach {id pass} [split $line] {break}
     if {![info exist pass]} {
         catch {puts -nonewline $client "Login: "}
         return
     }
     if {
         [info exist passwords($serverport,$id)] &&
         [string equal $passwords($serverport,$id) $pass]
     } then {
         set auth($client) 1
         puts stdout "$id logged in on $client"
         catch {puts $client "Welcome, $id!"}
         eval $cmd($client) [list $client $line 1]
         return
     }
     puts stdout "AUTH FAILURE ON $client"
     catch {puts $client "Unknown name or password"}
     disconnect $client
 }

 ## Standard handler for logged-in conversations and prompt-generation.
 proc execCommand {client line prompt} {
     global tcl_platform
     if {$prompt} {
         catch {puts -nonewline $client "\$ "}
         return
     }
     switch $tcl_platform(platform) {
         unix {
             catch {exec sh -c $line <@$client >@$client 2>@$client}
         }
         default {
             catch {exec $line} data
             puts $client $data
         }
     }
 }

 ## Administration service handler.  Chains to the normal handler for
 ## everything it doesn't recognise itself.
 proc admin {client line prompt} {
     if {$prompt} {
         catch {puts -nonewline $client "# "}
         return
     }
     set cmd [split $line]
     global denyHosts connections services
     if {[string equal $line "shutdown"]} {
         set ::termination 1
         puts stdout "Shutdown requested on $client"
         catch {puts $client "System will shut down as soon as possible"}
         return -code return "SHUTTING DOWN"
     } elseif {[string equal [lindex $cmd 0] "deny"]} {
         set denyHosts([lindex $cmd 1]) 1
     } elseif {[string equal [lindex $cmd 0] "allow"]} {
         catch {unset denyHosts([lindex $cmd 1])}
     } elseif {[string equal $line "denied"]} {
         foreach host [array names denyHosts] {
             catch {puts $client $host}
         }
     } elseif {[string equal $line "connections"]} {
         set len 0
         foreach conn [array names connections] {
             if {$len < [string length $conn]} {
                 set len [string length $conn]
             }
         }
         foreach {conn details} [array get connections] {
             catch {puts $client [format "%-*s = %s" $len $conn $details]}
         }
     } elseif {[string equal [lindex $cmd 0] "close"]} {
         set sock [lindex $cmd 1]
         if {[info exist connections($sock)]} {
             disconnect $sock
         }
     } elseif {[string equal $line "services"]} {
         set len 0
         foreach serv [array names services] {
             if {$len < [string length $serv]} {
                 set len [string length $serv]
             }
         }
         foreach {serv handler} [array get services] {
             set port [lindex [fconfigure $serv -sockname] 2]
             catch {puts $client [format "%-*s (port %d) = handler %s" $len $serv $port $handler]}
         }
     } elseif {[string equal [lindex $cmd 0] "addService"]} {
         set service [eval telnetServer [lrange $cmd 1 end]]
         catch {puts $client "Created service as $service"}
     } elseif {[string equal [lindex $cmd 0] "removeService"]} {
         set service [lindex $cmd 1]
         if {[info exist services($service)]} {
             closedownServer $service
         }
     } else {
         # CHAIN TO DEFAULT
         execCommand $client $line 0
     }
 }
 
 ## Executes a given command
 proc remoteCommand {client line prompt} {
     global tcl_platform
     if {$prompt} {
         catch {puts -nonewline $client "\$ "}
         return
     }
     catch {eval $line} data
     puts $client $data
 }

 telnetServer 2560 {orxonox rocks} remoteCommand
 telnetServer 2561 {orxadmin *****} admin

 puts stdout "Ready for service"

 vwait termination
 execute exit
