proc every {ms body} { eval $body; after $ms [list every $ms $body] }
package require irk
set conn [irk::connect orxonox $nickname]
irk::join $conn #orxonox
every 25 {}
vwait termination
