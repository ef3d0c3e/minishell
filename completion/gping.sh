complete -c gping -d 'Ping, but with a graph'
complete -c gping -l 'cmd' -d 'Graph execution time for this command'
complete -c gping -s 'h' -l 'help' -d 'Print help information'
complete -c gping -s '4' -d 'Resolve ping targets to IPv4 address'
complete -c gping -s '6' -d 'Resolve ping targets to IPv6 address'
complete -c gping -s 's' -l 'simple-graphics' -d 'Use dot character instead of braille'
complete -c gping -s 'b' -l 'buffer' -d 'Number of seconds to display on graph'
complete -c gping -s 'n' -l 'watch-interval' -d 'Watch interval in seconds'
