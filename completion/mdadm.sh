complete -c mdadm -s 'A' -l 'assemble' -d 'Assemble a pre-existing array'
complete -c mdadm -s 'B' -l 'build' -d 'Build a legacy array without superblocks'
complete -c mdadm -s 'C' -l 'create' -d 'Create a new array'
complete -c mdadm -s 'F' -l 'follow' -l 'monitor' -d 'Select monitor mode'
complete -c mdadm -s 'G' -l 'grow' -d 'Change the size or shape of an active array'
complete -c mdadm -s 'I' -l 'incremental' -d 'Manage devices in array, and possibly start it'
complete -c mdadm -l 'auto-detect' -d 'Start all auto-detected arrays'
complete -c mdadm -s 'h' -l 'help' -d 'Display help'
complete -c mdadm -l 'help-options' -d 'Display more detailed help'
complete -c mdadm -s 'V' -l 'version' -d 'Print version information'
complete -c mdadm -s 'v' -l 'verbose' -d 'Be more verbose'
complete -c mdadm -s 'q' -l 'quiet' -d 'Be quiet'
complete -c mdadm -s 'f' -l 'force' -d 'Force operation'
complete -c mdadm -s 'c' -l 'config' -d 'Specify config file or directory'
complete -c mdadm -s 's' -l 'scan' -d 'Scan for missing information'
complete -c mdadm -s 'e' -l 'metadata' -d 'Set metadata style to use'
complete -c mdadm -l 'homehost' -d 'Provide home host identity'
complete -c mdadm -l 'prefer' -d 'Give name format preference'
complete -c mdadm -l 'home-cluster' -d 'Give cluster name'
complete -c mdadm -s 'n' -l 'raid-devices' -d 'Specify the number of active devices'
complete -c mdadm -s 'x' -l 'spare-devices' -d 'Specify the number of spare devices'
complete -c mdadm -s 'z' -l 'size' -d 'Specify the space to use from each drive'
complete -c mdadm -s 'Z' -l 'array-size' -d 'Specify the size made available on the array'
complete -c mdadm -s 'c' -l 'chunk' -d 'Specify the chunk size'
complete -c mdadm -l 'rounding' -d 'Specify rounding factor'
complete -c mdadm -s 'l' -l 'level' -d 'Specify RAID level'
complete -c mdadm -s 'p' -l 'layout' -l 'parity' -d 'Specify data layout'
complete -c mdadm -s 'b' -l 'bitmap' -d 'Specify file for write-intent bitmap'
complete -c mdadm -l 'bitmap-chunk' -d 'Specify chunksize of bitmap'
complete -c mdadm -s 'W' -l 'write-mostly' -d 'Prefer reading from other devices than these'
complete -c mdadm -l 'readwrite' -d 'Enable writes on array or device'
complete -c mdadm -s 'o' -l 'readonly' -d 'Disable writes on array'
complete -c mdadm -l 'write-behind' -d 'Enable write-behind mode'
complete -c mdadm -l 'assume-clean' -d 'Assume the array is clean'
complete -c mdadm -l 'backup-file' -d 'Use this file as a backup'
complete -c mdadm -l 'data-offset' -d 'Specify start of array data'
complete -c mdadm -l 'continue' -d 'Resume frozen --grow command'
complete -c mdadm -s 'N' -l 'name' -d 'Set array name'
complete -c mdadm -s 'R' -l 'run' -d 'Run array despite warnings'
complete -c mdadm -s 'a' -l 'auto' -l 'level' -d 'Give instruction for device file'
complete -c mdadm -s 'a' -l 'add' -d 'Add devices to array'
complete -c mdadm -l 'nodes' -d 'Specify max nodes in the cluster'
complete -c mdadm -l 'write-journal' -d 'Specify journal device for RAID-4/5/6 array'
complete -c mdadm -s 'u' -l 'uuid' -d 'UUID of array to assemble'
complete -c mdadm -s 'm' -l 'super-minor' -d 'Minor number of array device'
complete -c mdadm -l 'no-degraded' -d 'Refuse to start without all drives'
complete -c mdadm -l 'invalid-backup' -d 'Do not ask for backup file, unavailable'
complete -c mdadm -s 'U' -l 'update' -d 'Update superblock properties'
complete -c mdadm -l 'freeze-reshape' -d 'Freeze --grow command'
complete -c mdadm -s 't' -l 'test' -d 'Test mode'
complete -c mdadm -s 'a' -l 'add' -d 'Hot-add listed devices'
complete -c mdadm -l 're-add' -d 'Re-add a previously removed device'
complete -c mdadm -l 'add-spare' -d 'Hot-add listed devices as spare'
complete -c mdadm -s 'r' -l 'remove' -d 'Remove listed inactive devices'
complete -c mdadm -s 'f' -l 'fail' -l 'set-faulty' -d 'Mark listed devices as faulty'
complete -c mdadm -l 'replace' -d 'Mark listed devices as requiring replacement'
complete -c mdadm -l 'with' -d 'Give devices as replacement'
complete -c mdadm -l 'cluster-confirm' -d 'Confirm existence of device'
complete -c mdadm -s 'Q' -l 'query' -d 'Examine device for md use'
complete -c mdadm -s 'D' -l 'detail' -d 'Print details on array'
complete -c mdadm -l 'detail-platform' -d 'Print details on platform capabilities'
complete -c mdadm -s 'Y' -l 'export' -d 'Format data output as key=value pairs'
complete -c mdadm -s 'E' -l 'examine' -d 'Print content of device metadata'
complete -c mdadm -l 'sparc2.2' -d 'Fix examination for buggy SPARC 2.2 kernel RAID'
complete -c mdadm -s 'X' -l 'examine-bitmap' -d 'Print report about bitmap'
complete -c mdadm -l 'examine-badblocks' -d 'List recorded bad blocks'
complete -c mdadm -l 'dump' -d 'Dump metadata to directory'
complete -c mdadm -l 'restore' -d 'Restore metadata from directory'
complete -c mdadm -s 'S' -l 'stop' -d 'Deactivate array'
complete -c mdadm -l 'zero-superblock' -d 'Erase possible superblock'
complete -c mdadm -l 'kill-subarray' -d 'Delete subarray'
complete -c mdadm -l 'update-subarray' -d 'Update subarray'
complete -c mdadm -s 'W' -l 'wait' -d 'Wait for pending operations'
complete -c mdadm -l 'wait-clean' -d 'Mark array as clean ASAP'
complete -c mdadm -l 'action' -d 'Set sync action for md devices'
complete -c mdadm -s 'r' -l 'rebuild-map' -d 'Rebuild /run/mdadm/map'
complete -c mdadm -l 'path' -d 'Automatically add eventually appearing device to array'
complete -c mdadm -s 'm' -l 'mail' -d 'Mail address to send alerts to'
complete -c mdadm -s 'p' -l 'program' -l 'alert' -d 'Program to run in case of an event'
complete -c mdadm -s 'y' -l 'syslog' -d 'Record events in syslog'
complete -c mdadm -s 'd' -l 'delay' -d 'Polling interval'
complete -c mdadm -s 'r' -l 'increment' -d 'Generate RebuildNN events each given percentage'
complete -c mdadm -s 'f' -l 'daemonise' -d 'Run monitor mode as a daemon'
complete -c mdadm -s 'i' -l 'pid-file' -d 'Write PID file when running as a daemon'
complete -c mdadm -s '1' -l 'oneshot' -d 'Check arrays only once'
complete -c mdadm -l 'no-sharing' -d 'Do not move spares between arrays'
complete -c mdadm -s 'b' -l 'brief' -d 'Be more concise'
