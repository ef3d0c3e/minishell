complete -c apk -d 'Package'
complete -c apk -d 'Package'
complete -c apk -d 'Package'
complete -c apk -d 'Package'
complete -c apk -s 'h' -l 'help' -d 'Show help'
complete -c apk -s 'p' -l 'root' -d 'Install packages to DIR'
complete -c apk -s 'X' -l 'repository' -d 'Use packages from REPO'
complete -c apk -s 'q' -l 'quiet' -d 'Print less information'
complete -c apk -s 'v' -l 'verbose' -d 'Print more information'
complete -c apk -s 'i' -l 'interactive' -d 'Ask confirmation for certain operations'
complete -c apk -s 'V' -l 'version' -d 'Print program version'
complete -c apk -l 'force-binary-stdout' -d 'Continue even if binary data is to be output'
complete -c apk -l 'force-broken-world' -d 'Continue even if '"'"'world'"'"' cannot be satisfied'
complete -c apk -l 'force-non-repository' -d 'Continue even if packages may be lost on reboot'
complete -c apk -l 'force-old-apk' -d 'Continue even if packages use unsupported features'
complete -c apk -l 'force-overwrite' -d 'Overwrite files in other packages'
complete -c apk -l 'force-refresh' -d 'Don'"'"'t use cached files'
complete -c apk -s 'U' -l 'update-cache' -d 'Alias for --cache-max-age 1'
complete -c apk -l 'progress' -d 'Show a progress bar'
complete -c apk -l 'progress-fd' -d 'Write progress to FD'
complete -c apk -l 'no-progress' -d 'Disable progress bar even for TTYs'
complete -c apk -l 'purge' -d 'Delete also modified configuration files and uninstalled packages from cache'
complete -c apk -l 'allow-untrusted' -d 'Install packages with untrusted signature or no signature'
complete -c apk -l 'wait' -d 'Wait for TIME seconds to get an exclusive repository lock before failing'
complete -c apk -l 'keys-dir' -d 'Override directory of trusted keys'
complete -c apk -l 'repositories-file' -d 'Override repositories file'
complete -c apk -l 'no-network' -d 'Don'"'"'t use network'
complete -c apk -l 'no-cache' -d 'Don'"'"'t use any local cache path'
complete -c apk -l 'cache-dir' -d 'Override cache directory'
complete -c apk -l 'cache-max-age' -d 'Maximum AGE for index in cache before refresh'
complete -c apk -l 'arch' -d 'Use architecture with --root'
complete -c apk -l 'print-arch' -d 'Print default architecture'
complete -c apk -d 'Add packages'
complete -c apk -d 'Remove packages'
complete -c apk -d 'Repair package'
complete -c apk -d 'Update repository indexes'
complete -c apk -d 'Give detailed information about packages'
complete -c apk -d 'List packages'
complete -c apk -d 'Search package'
complete -c apk -d 'Upgrade installed packages'
complete -c apk -d 'Manage local package cache'
complete -c apk -d 'Compare package versions'
complete -c apk -d 'Create repository index file'
complete -c apk -d 'Download packages'
complete -c apk -d 'Audit the directories for changes'
complete -c apk -d 'Verify package integrity and signature'
complete -c apk -d 'Generate graphviz graphs'
complete -c apk -d 'Show repository policy for packages'
complete -c apk -d 'Show statistics about repositories and installations'
complete -c apk -d 'Show checksums of package contents'
complete -c apk -s 's' -l 'simulate' -d 'Simulate the requested operation'
complete -c apk -l 'clean-protected' -d 'Don'"'"'t create .apk-new files'
complete -c apk -l 'overlay-from-stdin' -d 'Read list of overlay files from stdin'
complete -c apk -l 'no-scripts' -d 'Don'"'"'t execute any scripts'
complete -c apk -l 'no-commit-hooks' -d 'Skip pre/post hook scripts'
complete -c apk -l 'initramfs-diskless-boot' -d 'Enables options for diskless initramfs boot'
complete -c apk -l 'initdb' -d 'Initialize database'
complete -c apk -s 'u' -l 'upgrade' -d 'Prefer to upgrade package'
complete -c apk -s 'l' -l 'latest' -d 'Select latest version of package'
complete -c apk -s 't' -l 'virtual' -d 'Create virtual package'
complete -c apk -l 'no-chown' -d 'Don'"'"'t change file owner or group'
complete -c apk -s 'r' -l 'rdepends' -d 'Remove unneeded dependencies too'
complete -c apk -s 'd' -l 'depends' -d 'Fix all dependencies too'
complete -c apk -s 'r' -l 'reinstall' -d 'Reinstall the package'
complete -c apk -s 'u' -l 'upgrade' -d 'Prefer to upgrade package'
complete -c apk -s 'x' -l 'xattr' -d 'Fix packages with broken xattrs'
complete -c apk -l 'directory-permissions' -d 'Reset all directory permissions'
complete -c apk -s 'L' -l 'contents' -d 'List included files'
complete -c apk -s 'e' -l 'installed' -d 'Check PACKAGE installed status'
complete -c apk -s 'W' -l 'who-owns' -d 'Print who owns the file'
complete -c apk -s 'R' -l 'depends' -d 'List the dependencies'
complete -c apk -s 'P' -l 'provides' -d 'List virtual packages provided'
complete -c apk -s 'r' -l 'rdepends' -d 'List reverse dependencies'
complete -c apk -l 'replaces' -d 'List packages that PACKAGE might replace'
complete -c apk -l 'install-if' -d 'List install_if rule'
complete -c apk -l 'rinstall-if' -d 'List packages having install_if referencing PACKAGE'
complete -c apk -s 'w' -l 'webpage' -d 'Print the URL for the upstream'
complete -c apk -s 's' -l 'size' -d 'Show installed size'
complete -c apk -s 'd' -l 'description' -d 'Print the description'
complete -c apk -l 'license' -d 'Print the license'
complete -c apk -l 'triggers' -d 'Print active triggers'
complete -c apk -s 'a' -l 'all' -d 'Print all information'
complete -c apk -s 'I' -l 'installed' -d 'List installed packages only'
complete -c apk -s 'O' -l 'orphaned' -d 'List orphaned packages only'
complete -c apk -s 'a' -l 'available' -d 'List available packages only'
complete -c apk -s 'u' -l 'upgradable' -d 'List upgradable packages only'
complete -c apk -s 'o' -l 'origin' -d 'List packages by origin'
complete -c apk -s 'd' -l 'depends' -d 'List packages by dependency'
complete -c apk -s 'P' -l 'providers' -d 'List packages by provider'
complete -c apk -s 'a' -l 'all' -d 'Show all package versions'
complete -c apk -s 'd' -l 'description' -d 'Search package descriptions'
complete -c apk -s 'x' -l 'exact' -d 'Require exact match'
complete -c apk -s 'o' -l 'origin' -d 'Print origin package name'
complete -c apk -s 'r' -l 'rdepends' -d 'Print reverse dependencies'
complete -c apk -l 'has-origin' -d 'List packages that have the given origin'
complete -c apk -s 'a' -l 'available' -d 'Reset all packages to the provided versions'
complete -c apk -s 'l' -l 'latest' -d 'Select latest version of package'
complete -c apk -l 'no-self-upgrade' -d 'Don'"'"'t do early upgrade of the apk'
complete -c apk -l 'self-upgrade-only' -d 'Only do self-upgrade'
complete -c apk -l 'ignore' -d 'Ignore the upgrade of PACKAGE'
complete -c apk -l 'prune' -d 'Prune the WORLD by removing packages which are no longer available'
complete -c apk -s 'u' -l 'upgrade' -d 'Prefer to upgrade package'
complete -c apk -s 'l' -l 'latest' -d 'Select latest version of package'
complete -c apk -s 'I' -l 'indexes' -d 'Print description and versions of indexes'
complete -c apk -s 't' -l 'test' -d 'Compare two given versions'
complete -c apk -s 'c' -l 'check' -d 'Check the given version strings'
complete -c apk -s 'a' -l 'all' -d 'Consider packages from all repository tags'
complete -c apk -s 'l' -l 'limit' -d 'Limit to packages with output matching given operand'
complete -c apk -s 'o' -l 'output' -d 'Write the generated index to FILE'
complete -c apk -s 'x' -l 'index' -d 'Read an existing index from INDEX'
complete -c apk -s 'd' -l 'description' -d 'Add a description to the index'
complete -c apk -l 'rewrite-arch' -d 'Use ARCH as architecture for all packages'
complete -c apk -l 'no-warnings' -d 'Disable the warning about missing dependencies'
complete -c apk -s 'L' -l 'link' -d 'Create hard links'
complete -c apk -s 'R' -l 'recursive' -d 'Fetch all dependencies too'
complete -c apk -l 'simulate' -d 'Simulate the requested operation'
complete -c apk -s 's' -l 'stdout' -d 'Dump the .apk to stdout'
complete -c apk -s 'o' -l 'output' -d 'Write the downloaded files to DIR'
complete -c apk -l 'backup' -d 'Audit configuration files only'
complete -c apk -l 'system' -d 'Audit all system files'
complete -c apk -l 'check-permissions' -d 'Check file permissions too'
complete -c apk -s 'r' -l 'recursive' -d 'Descend into directories and audit them as well'
complete -c apk -l 'packages' -d 'List only the changed packages'
complete -c apk -l 'errors' -d 'Consider only packages with errors'
complete -c apk -l 'installed' -d 'Consider only installed packages'
