complete -c kdeconnect-cli -s 'l' -l 'list-devices' -d 'List all devices'
complete -c kdeconnect-cli -s 'a' -l 'list-available' -d 'List available (paired and reachable) devices'
complete -c kdeconnect-cli -l 'id-only' -d 'Make --list-devices or --list-available print the devices id, to ease scripting'
complete -c kdeconnect-cli -l 'refresh' -d 'Search for devices in the network and re-establish connections'
complete -c kdeconnect-cli -l 'pair' -d 'Request pairing to a said device'
complete -c kdeconnect-cli -l 'ring' -d 'Find the said device by ringing it.'
complete -c kdeconnect-cli -l 'unpair' -d 'Stop pairing to a said device'
complete -c kdeconnect-cli -l 'ping' -d 'Sends a ping to said device'
complete -c kdeconnect-cli -l 'ping-msg' -d 'Same as ping but you can set the message to display'
complete -c kdeconnect-cli -l 'share' -d 'Share a file to a said device'
complete -c kdeconnect-cli -l 'list-notifications' -d 'Display the notifications on a said device'
complete -c kdeconnect-cli -l 'lock' -d 'Lock the specified device'
complete -c kdeconnect-cli -l 'send-sms' -d 'Sends an SMS. Requires destination'
complete -c kdeconnect-cli -l 'destination' -d 'Phone number to send the message'
complete -c kdeconnect-cli -s 'd' -l 'device'
complete -c kdeconnect-cli -s 'n' -l 'name' -d 'Device Name'
complete -c kdeconnect-cli -l 'encryption-info' -d 'Get encryption info about said device'
complete -c kdeconnect-cli -l 'list-commands' -d 'Lists remote commands and their ids'
complete -c kdeconnect-cli -l 'execute-command' -d 'Executes a remote command by id'
complete -c kdeconnect-cli -s 'h' -l 'help' -d 'Displays this help.'
complete -c kdeconnect-cli -s 'v' -l 'version' -d 'Displays version information.'
complete -c kdeconnect-cli -l 'author' -d 'Show author information.'
complete -c kdeconnect-cli -l 'license' -d 'Show license information.'
complete -c kdeconnect-cli -l 'desktopfile' -d 'The base file name of the desktop entry for this application.'
