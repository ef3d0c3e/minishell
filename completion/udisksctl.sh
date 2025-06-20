complete -c udisksctl -l 'help' -d 'Shows help'
complete -c udisksctl -d 'Shows help'
complete -c udisksctl -d 'Shows information about an object'
complete -c udisksctl -d 'Shows information about all objects'
complete -c udisksctl -d 'Shows high-level status'
complete -c udisksctl -d 'Monitor changes to objects'
complete -c udisksctl -d 'Mount a filesystem'
complete -c udisksctl -d 'Unmount a filesystem'
complete -c udisksctl -d 'Unlock an encrypted device'
complete -c udisksctl -d 'Lock an encrypted device'
complete -c udisksctl -d 'Set-up a loop device'
complete -c udisksctl -d 'Delete a loop device'
complete -c udisksctl -d 'Safely power off a drive'
complete -c udisksctl -d 'Set SMART data for a drive'
complete -c udisksctl -s 'p' -l 'object-path' -d 'Object to get information about'
complete -c udisksctl -s 'b' -l 'block-device' -d 'Block device to get information about'
complete -c udisksctl -s 'd' -l 'drive' -d 'Drive to get information about'
complete -c udisksctl -s 't' -l 'filesystem-type' -d 'Filesystem type to use'
complete -c udisksctl -s 'o' -l 'options' -d 'Mount options'
complete -c udisksctl -s 'f' -l 'force' -d 'Force/layzy unmount'
complete -c udisksctl -d 'Mount point'
complete -c udisksctl -s 'b' -l 'block-device' -d 'Mounted block device'
complete -c udisksctl -s 'f' -l 'file' -d 'File to set-up a loop device for'
complete -c udisksctl -s 'r' -l 'read-only' -d 'Setup read-only device'
complete -c udisksctl -s 'o' -l 'offset' -d 'Start at <num> bytes into file'
complete -c udisksctl -s 's' -l 'size' -d 'Limit size to <num> bytes'
complete -c udisksctl -s 'p' -l 'object-path' -d 'Object for loop device to delete'
complete -c udisksctl -s 'b' -l 'block-device' -d 'Loop device to delete'
complete -c udisksctl -s 'f' -l 'file' -d 'File with libatasmart blob'
complete -c udisksctl -l 'no-user-interaction' -d 'Do not authenticate the user if needed'
