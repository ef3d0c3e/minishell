complete -c fab -s 'h' -l 'help' -d 'show this help message and exit'
complete -c fab -s 'd' -l 'display' -d 'print detailed info about command NAME'
complete -c fab -s 'F' -l 'list-format' -d 'formats --list, choices: short, normal, nested'
complete -c fab -s 'I' -l 'initial-password-prompt' -d 'Force password prompt up-front'
complete -c fab -l 'initial-sudo-password-prompt' -d 'Force sudo password prompt up-front'
complete -c fab -s 'l' -l 'list' -d 'print list of possible commands and exit'
complete -c fab -l 'set' -d 'comma separated KEY=VALUE pairs to set Fab env vars'
complete -c fab -l 'shortlist' -d 'alias for -F short --list'
complete -c fab -s 'V' -l 'version' -d 'show program'"'"'s version number and exit'
complete -c fab -s 'a' -l 'no_agent' -d 'don'"'"'t user the running SSH agent'
complete -c fab -s 'A' -l 'forward-agent' -d 'forward local agent to remote end'
complete -c fab -l 'abort-on-prompts' -d 'abort instead of prompting (for password, host, etc)'
complete -c fab -s 'c' -l 'config' -d 'specify location of config file to use'
complete -c fab -l 'colorize-errors' -d 'Color error output'
complete -c fab -s 'D' -l 'disable-known-host' -d 'do not load user known_hosts file'
complete -c fab -s 'e' -l 'eagerly-disconnect' -d 'disconnect from hosts as soon as possible'
complete -c fab -s 'f' -l 'fabfile' -d 'python module file to import, e.g. '"'"'../other.py'"'"''
complete -c fab -s 'g' -l 'gateway' -d 'gateway host to connect through'
complete -c fab -l 'gss-auth' -d 'Use GSS-API authentication'
complete -c fab -l 'gss-deleg' -d 'Delegate GSS-API client credentials or not'
complete -c fab -l 'gss-kex' -d 'Perform GSS-API Key Exchange and user authentication'
complete -c fab -l 'hide' -d 'comma-separated list of output levels to hide'
complete -c fab -s 'H' -l 'hosts' -d 'comma-separated list of hosts to operate on'
complete -c fab -s 'i' -d 'path to SSH private key file. May be repeated.'
complete -c fab -s 'k' -l 'no-keys' -d 'don'"'"'t load private key files from ~/.ssh/'
complete -c fab -l 'keepaplive' -d 'enables a keepalive every N seconds'
complete -c fab -l 'linewise' -d 'print line-by-line instead of byte-by-byte'
complete -c fab -s 'n' -l 'connection-attempts' -d 'make M attempts to connect before giving up'
complete -c fab -l 'no-pty' -d 'do not use pseudo-terminal in run/sudo'
complete -c fab -s 'p' -l 'password' -d 'password for use with authentication and/or sudo'
complete -c fab -s 'P' -l 'parallel' -d 'default to parallel execution method'
complete -c fab -l 'port' -d 'SSH connection port'
complete -c fab -s 'r' -l 'reject-unknown-host' -d 'reject unknown hosts'
complete -c fab -l 'sudo-password' -d 'password for use with sudo only'
complete -c fab -l 'system-known-hosts' -d 'load system known_hosts file before reading user known_hosts'
complete -c fab -s 'R' -l 'roles' -d 'comma-separated list of roles to operate on'
complete -c fab -s 's' -l 'shell' -d 'specify a new shell, defaults to '"'"'/bin/bash -l -c'"'"''
complete -c fab -l 'show' -d 'comma-separated list of output levels to show'
complete -c fab -l 'skip-bad-hosts' -d 'skip over hosts that can'"'"'t be reached'
complete -c fab -l 'skip-unknown-tasks' -d 'skip over unknown tasks'
complete -c fab -l 'ssh-config-path' -d 'Path to SSH config file'
complete -c fab -s 't' -l 'timeout' -d 'set connection timeout to N seconds'
complete -c fab -s 'T' -l 'command-timeout' -d 'set remote command timeout to N seconds'
complete -c fab -s 'u' -l 'user' -d 'username to use when connecting to remote hosts'
complete -c fab -s 'w' -l 'warn-only' -d 'warn, instead of abort, when commands fail'
complete -c fab -s 'x' -l 'exclude-hosts' -d 'comma-separated list of hosts to exclude'
complete -c fab -s 'z' -l 'pool-size' -d 'number of concurrent processes to use in parallel mode'
