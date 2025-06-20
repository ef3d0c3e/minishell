complete -c aura -s 'A' -l 'aursync' -d 'Synchronize AUR packages'
complete -c aura -s 'B' -l 'save' -d 'Save and restore package state'
complete -c aura -s 'C' -l 'downgrade' -d 'Package cache actions'
complete -c aura -s 'D' -l 'database' -d 'Modify the package database'
complete -c aura -s 'L' -l 'viewlog' -d 'Pacman log actions'
complete -c aura -s 'O' -l 'orphans' -d 'Operate on orphan packages'
complete -c aura -s 'Q' -l 'query' -d 'Query the package database'
complete -c aura -s 'R' -l 'remove' -d 'Remove packages from the system'
complete -c aura -s 'S' -l 'sync' -d 'Synchronize packages'
complete -c aura -s 'T' -l 'deptest' -d 'Check dependencies'
complete -c aura -s 'U' -l 'upgrade' -d 'Upgrade or add a local package'
complete -c aura -l 'check' -d 'Validate your system'
complete -c aura -l 'conf' -d 'View various configuration settings and files'
complete -c aura -l 'deps' -d 'Output a dependency graph'
complete -c aura -l 'free' -d 'State of Free Software install on the system'
complete -c aura -l 'stats' -d 'View statistics about your machine or about Aura itself'
complete -c aura -l 'thanks' -d 'The people behind Aura'
complete -c aura -s 'V' -l 'version' -d 'Display version and exit'
complete -c aura -s 'h' -l 'help' -d 'Display help'
complete -c aura -l 'log-level' -d 'Minimum level of Aura log messages to display'
complete -c aura -l 'noconfirm' -d 'Do not ask for any confirmation'
complete -c aura -s 'i' -l 'info' -d 'View package information'
complete -c aura -s 's' -l 'search' -d 'Search the AUR for matching terms'
complete -c aura -s 'u' -l 'sysupgrade' -d 'Upgrade all installed AUR packages'
complete -c aura -s 'v' -l 'provides' -d 'Search for packages that "provide" some package identity'
complete -c aura -s 'w' -l 'clone' -d 'Download the source tarball'
complete -c aura -s 'y' -l 'refresh' -d 'Pull the latest changes for every local package clone'
complete -c aura -l 'build' -d 'Specify a build location'
complete -c aura -l 'builduser' -d 'User to build as'
complete -c aura -l 'hotedit' -d 'Prompt for PKGBUILD editing'
complete -c aura -l 'ignore' -d 'Ignore a package upgrade'
complete -c aura -l 'nocheck' -d 'Do not run the check() functin of the PKGBUILD'
complete -c aura -l 'shellcheck' -d 'Run shellcheck on PKGBUILDs before building'
complete -c aura -l 'skipdepcheck' -d 'Perform no dependency resolution'
complete -c aura -l 'skipinteg' -d 'Do not perform any verifcation checks on source files'
complete -c aura -l 'skippgpcheck' -d 'Do not verify source files with PGP signatures'
complete -c aura -l 'abc' -d 'Sort alphabetically'
complete -c aura -l 'limit' -d 'Limit the results to N results'
complete -c aura -s 'r' -l 'reverse' -d 'Reverse the search results'
complete -c aura -s 'q' -l 'quiet' -d 'Only print matching package names'
complete -c aura -s 'c' -l 'clean' -d 'Remove all but the given number of backups'
complete -c aura -s 'l' -l 'list' -d 'Show all saved package snapshot filenames'
complete -c aura -s 'r' -l 'restore' -d 'Restores a record kept with -B'
complete -c aura -s 'b' -l 'backup' -d 'Backup to directory'
complete -c aura -s 'c' -l 'clean' -d 'Save this many versions'
complete -c aura -s 'i' -l 'info' -d 'Look up specific packages for info on their cache entries'
complete -c aura -s 'l' -l 'list' -d 'Print the contents of the package cache'
complete -c aura -s 's' -l 'search' -d 'Search via regex'
complete -c aura -s 't' -l 'invalid' -d 'Delete invalid tarballs from the cache'
complete -c aura -s 'y' -l 'refresh' -d 'Download missing tarballs of installed packages'
complete -c aura -s 'i' -l 'info' -d 'Show package history'
complete -c aura -s 's' -l 'search' -d 'Search via regex'
complete -c aura -s 'a' -l 'adopt' -d 'Mark a package as being explicitly installed'
complete -c aura -s 'e' -l 'elderly' -d 'Display all explicitly installed, top-level packages'
complete -c aura -s 'j' -l 'abandon' -d 'Uninstall orphan packages'
complete -c aura -s 'c' -l 'changelog' -d 'View the change log of PACKAGE'
complete -c aura -s 'd' -l 'deps' -d 'List only non-explicit packages (dependencies)'
complete -c aura -s 'e' -l 'explicit' -d 'List only explicitly installed packages'
complete -c aura -s 'k' -l 'check' -d 'Check if all files owned by PACKAGE are present'
complete -c aura -s 'l' -l 'list' -d 'List all files owned by PACKAGE'
complete -c aura -s 'm' -l 'foreign' -d 'List all packages not in the database'
complete -c aura -s 'o' -l 'owns' -d 'Search for the package that owns FILE'
complete -c aura -s 'p' -l 'file' -d 'Apply the query to a package file, not package'
complete -c aura -s 't' -l 'unrequired' -d 'List only unrequired packages'
complete -c aura -s 'u' -l 'upgrades' -d 'List only out-of-date packages'
complete -c aura -d 'Installed package'
complete -c aura -s 'c' -l 'cascade' -d 'Also remove packages depending on PACKAGE'
complete -c aura -s 'n' -l 'nosave' -d 'Ignore file backup designations'
complete -c aura -s 's' -l 'recursive' -d 'Also remove dependencies of PACKAGE'
complete -c aura -s 'u' -l 'unneeded' -d 'Only remove targets not required by PACKAGE'
complete -c aura -d 'Installed package'
complete -c aura -s 'c' -l 'clean' -d 'Remove [all] packages from cache'
complete -c aura -s 'l' -l 'list' -d 'List all packages in REPOSITORY'
complete -c aura -s 'u' -l 'sysupgrade' -d 'Upgrade all packages that are out of date'
complete -c aura -s 'w' -l 'downloadonly' -d 'Only download the target packages'
complete -c aura -s 'y' -l 'refresh' -d 'Download fresh copy of the package list'
complete -c aura -d 'Package file'
complete -c aura -s 'a' -l 'aura' -d 'View the contents of your Aura config'
complete -c aura -s 'g' -l 'gen' -d 'Output your full current config as legal TOML'
complete -c aura -s 'm' -l 'makepkg' -d 'View the Makepkg conf'
complete -c aura -s 'p' -l 'pacman' -d 'View the Pacman conf'
complete -c aura -s 'l' -l 'limit' -d 'The number of layers up or down to allow'
complete -c aura -s 'o' -l 'optional' -d 'Include optional dependencies'
complete -c aura -s 'r' -l 'reverse' -d 'Display packages that depend on the given args'
complete -c aura -l 'open' -d 'Open the output image automatically'
complete -c aura -l 'raw' -d 'Print the raw DOT output'
complete -c aura -l 'copyleft' -d 'Consider only Copyleft licenses'
complete -c aura -l 'lenient' -d 'Allow FOSS-derived custom licenses'
