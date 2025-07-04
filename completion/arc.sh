complete -c arc -l 'trace' -d 'Debugging command'
complete -c arc -l 'ansi' -d 'Always use fromatting'
complete -c arc -l 'load-phutil-library' -d 'Specify which libraies to load'
complete -c arc -l 'conduit-uri' -d 'Specify the Conduit URI'
complete -c arc -l 'conduit-token' -d 'Specify the Conduit token'
complete -c arc -l 'conduit-version' -d 'Force a version'
complete -c arc -l 'conduit-timeout' -d 'Sets the timeout'
complete -c arc -l 'config' -d 'Sets a config'
complete -c arc -l 'skip-arcconfig' -d 'Skip the working copy configuration file'
complete -c arc -l 'arcrc-file' -d 'Use provided file instead of ~/.arcrc'
complete -c arc -d 'Implements shell completion'
complete -c arc -l 'current' -d 'Current term in the argument list being completed'
complete -c arc -d 'Reads an arc configuration option'
complete -c arc -l 'verbose' -d 'Show detailed information about options'
complete -c arc -d 'Download a file to local disk'
complete -c arc -l 'as' -d 'Save the file with a specific name rather than the default'
complete -c arc -l 'show' -d 'Write file to stdout instead of to disk'
complete -c arc -d 'List your open Differential revisions'
complete -c arc -d 'Run static analysis on changes to check for mistakes'
complete -c arc -l 'engine' -d 'Override configured lint engine for this project'
complete -c arc -l 'apply-patches' -d 'Apply patches suggested by lint to the working copy'
complete -c arc -l 'severity' -d 'Set minimum message severity'
complete -c arc -l 'never-apply-patches' -d 'Never apply patches suggested by lint'
complete -c arc -l 'rev' -d 'Lint changes since a specific revision'
complete -c arc -l 'outfile' -d 'Output the linter results to a file'
complete -c arc -l 'lintall' -d 'Show all lint warnings, not just those on changed lines'
complete -c arc -l 'amend-all' -d 'When linting git repositories, amend HEAD with all patches'
complete -c arc -l 'everything' -d 'Lint all files in the project'
complete -c arc -l 'output' -d 'Specify how results will be displayed'
complete -c arc -l 'only-new' -d 'Display only messages not present in the original code'
complete -c arc -l 'only-changed' -d 'Show lint warnings just on changed lines'
complete -c arc -l 'amend-autofixes' -d 'When linting git repositories, amend HEAD with autofix'
complete -c arc -l 'edit' -d 'Edit the flag on an object'
complete -c arc -l 'color' -d 'Set the color of a flag'
complete -c arc -l 'clear' -d 'Delete the flag on an object'
complete -c arc -l 'note' -d 'Set the note on a flag'
complete -c arc -d 'Export the local changeset to a file'
complete -c arc -l 'unified' -d 'Export change as a unified patch'
complete -c arc -l 'git' -d 'Export change as a git patch'
complete -c arc -l 'encoding' -d 'Attempt to convert non UTF-8 patch into specified encoding'
complete -c arc -l 'arcbundle' -d 'Export change as an arc bundle'
complete -c arc -l 'diff' -d 'Export from Differential diff'
complete -c arc -l 'revision' -d 'Export from a Differential revision'
complete -c arc -d 'Open a file or object in your web browser'
complete -c arc -l 'force' -d 'Open arguments as paths, even if they do not exist in the working copy'
complete -c arc -l 'branch' -d 'Default branch name to view on server'
complete -c arc -d 'Quickly create a task for yourself'
complete -c arc -l 'cc' -d 'Other users to CC on the new task'
complete -c arc -l 'project' -d 'Projects to assign to the task'
complete -c arc -l 'browse' -d 'After creating the task, open it in a web browser'
complete -c arc -d 'what they do and which versions are installed'
complete -c arc -l 'search' -d 'Search for linters'
complete -c arc -l 'verbose' -d 'Show detailed information, including options'
complete -c arc -d 'Stop tracking work in Phrequent'
complete -c arc -l 'note' -d 'A note to attach to the tracked time'
complete -c arc -d 'Create an alias'
complete -c arc -d 'Sets an arc configuration option'
complete -c arc -l 'local' -d 'Set a local config value instead of a user one'
complete -c arc -d 'Start tracking work in Phrequent'
complete -c arc -d 'Close a task or otherwise update its status'
complete -c arc -l 'message' -d 'Provide a comment with your status change'
complete -c arc -l 'list-statuses' -d 'Show available status options and exit'
complete -c arc -d 'Publish an accepted revision after review'
complete -c arc -l 'preview' -d 'Prints the commits that would be landed'
complete -c arc -l 'remote' -d 'Push to a remote other than the default'
complete -c arc -l 'delete-remote' -d 'Delete the feature branch in the remote after landing it'
complete -c arc -l 'update-with-rebase' -d 'When updating the feature branch, use rebase instead of merge'
complete -c arc -l 'squash' -d 'Use squash strategy'
complete -c arc -l 'keep-branch' -d 'Keep the feature branch'
complete -c arc -l 'merge' -d 'Use merge strategy'
complete -c arc -l 'update-with-merge' -d 'When updating the feature branch, use merge instead of rebase'
complete -c arc -l 'hold' -d 'Prepare the change to be pushed, but do not actually push it'
complete -c arc -l 'onto' -d 'Land feature branch onto a branch other than the default'
complete -c arc -l 'revision' -d 'Use the message from a specific revision'
complete -c arc -d 'Show which commits will be selected'
complete -c arc -l 'show-base' -d 'Print base commit only and exit'
complete -c arc -l 'base' -d 'Additional rules for determining base revision'
complete -c arc -l 'head' -d 'Specify the end of the commit range to select'
complete -c arc -l 'any-status' -d 'Show committed and abandoned revisions'
complete -c arc -d 'Alias for arc feature'
complete -c arc -d 'Amend the working copy'
complete -c arc -l 'revision' -d 'Use the message from a specific revision'
complete -c arc -l 'show' -d 'Show the amended commit message'
complete -c arc -d 'Upgrade arcanist and libphutil to the latest versions'
complete -c arc -d 'Shows the help'
complete -c arc -l 'full' -d 'Print detailed information about each command'
complete -c arc -d 'Share and grab text using the Paste application'
complete -c arc -l 'lang' -d 'Language for syntax highlighting'
complete -c arc -l 'json' -d 'Output in JSON format'
complete -c arc -l 'title' -d 'Title for the paste'
complete -c arc -d 'Commit a revision which has been accepted by a reviewer'
complete -c arc -l 'revision' -d 'Commit a specific revision'
complete -c arc -l 'show' -d 'Show the command which would be issued'
complete -c arc -d 'Apply changes to the working copy'
complete -c arc -l 'force' -d 'Do not run any sanity checks'
complete -c arc -l 'encoding' -d 'Attempt to convert non UTF-8 patch into specified encoding'
complete -c arc -l 'nocommit' -d 'Do not commit the changes'
complete -c arc -l 'update' -d 'Update the local working copy before applying the patch'
complete -c arc -l 'patch' -d 'Apply changes from a git patch file or unified patch file'
complete -c arc -l 'arcbundle' -d 'Apply changes from an arc bundlej'
complete -c arc -l 'skip-dependencies' -d 'Do not apply dependencies'
complete -c arc -l 'diff' -d 'Apply changes from a Differential diff'
complete -c arc -l 'nobranch' -d 'Do not create a branch'
complete -c arc -l 'revision' -d 'Apply changes from a Differential revision'
complete -c arc -d 'Installs Conduit credentials into your ~/.arcc'
complete -c arc -d 'Please use backout instead'
complete -c arc -d 'Upload a file from local disk'
complete -c arc -l 'json' -d 'Output upload information in JSON format'
complete -c arc -l 'temporary' -d 'Mark the file as temporary'
complete -c arc -d 'Alias for arc feature'
complete -c arc -d 'Show blame for the lines you changed'
complete -c arc -l 'rev' -d 'Cover changes since a specific revision'
complete -c arc -d 'Close a revision'
complete -c arc -l 'quiet' -d 'Do not print a success message'
complete -c arc -l 'finalize' -d 'Close only if the repository is untracked and the revision is accepted'
complete -c arc -d 'View all assigned tasks'
complete -c arc -l 'status' -d 'Show tasks that are open or closed, default is open'
complete -c arc -l 'owner' -d 'Only show tasks assigned to the given username,'
complete -c arc -l 'unassigned' -d 'Only show tasks that are not assigned'
complete -c arc -l 'limit' -d 'Limit the amount of tasks outputted, default is all'
complete -c arc -l 'order' -d 'Arrange tasks based on priority, created, or modified,'
complete -c arc -l 'output' -d 'Specify the output format'
complete -c arc -l 'view-all' -d 'Include closed and abandoned revisions'
complete -c arc -l 'by-status' -d 'Sort branches by status instead of time'
complete -c arc -d 'Run unit tests that cover specified paths'
complete -c arc -l 'engine' -d 'Override configured unit engine for this project'
complete -c arc -l 'detailed-coverage' -d 'Show a detailed coverage report on the CLI'
complete -c arc -l 'target' -d 'Record a copy of the test results on the specified build target'
complete -c arc -l 'ugly' -d 'Use uglier formatting'
complete -c arc -l 'rev' -d 'Run unit tests covering changes since a specific revision'
complete -c arc -l 'everything' -d 'Run every test'
complete -c arc -l 'json' -d 'Report results in JSON format'
complete -c arc -l 'coverage' -d 'Always enable coverage information'
complete -c arc -l 'output' -d 'Specify the output format'
complete -c arc -l 'no-coverage' -d 'Always disable coverage information'
complete -c arc -d 'Backouts on a previous commit'
complete -c arc -d 'Make a raw Conduit method call'
complete -c arc -d 'Generate a Differential diff or revision from local changes'
complete -c arc -l 'raw-command' -d 'Generate diff by executing a specified command'
complete -c arc -l 'encoding' -d 'Attempt to convert non UTF-8 hunks into specified encoding'
complete -c arc -l 'cc' -d 'When creating a revision, add CCs'
complete -c arc -l 'reviewers' -d 'When creating a revision, add reviewers'
complete -c arc -l 'skip-staging' -d 'Do not copy changes to the staging area'
complete -c arc -l 'raw' -d 'Read diff from stdin'
complete -c arc -l 'uncommitted' -d 'Suppress warning about uncommitted changes'
complete -c arc -l 'message-file' -d 'Read revision information from file'
complete -c arc -l 'nolint' -d 'Do not run lint'
complete -c arc -l 'message' -d 'Use the specified message when updating a revision'
complete -c arc -l 'plan-changes' -d 'Create or update a revision without requesting a code review'
complete -c arc -l 'browse' -d 'After creating a diff or revision, open it in a web browser'
complete -c arc -l 'create' -d 'Always create a new revision'
complete -c arc -l 'cache' -d 'Disable lint cache'
complete -c arc -l 'use-commit-message' -d 'Read revision information from a specific commit'
complete -c arc -l 'only' -d 'Only generate a diff, without running lint, unit tests, or other'
complete -c arc -l 'skip-binaries' -d 'Do not upload binaries'
complete -c arc -l 'preview' -d 'only create a diff'
complete -c arc -l 'amend-autofixes' -d 'When linting git repositories, amend HEAD with autofix'
complete -c arc -l 'apply-patches' -d 'Apply patches suggested by lint'
complete -c arc -l 'head' -d 'Specify the end of the commit range'
complete -c arc -l 'verbatim' -d 'When creating a revision, try to use the working copy commit'
complete -c arc -l 'less-context' -d 'Create a diff with a few lines of context.'
complete -c arc -l 'advice' -d 'Require excuse for lint advice in addition to lint warnings and errors'
complete -c arc -l 'json' -d 'Emit machine-readable JSON'
complete -c arc -l 'update' -d 'Always update a specific revision'
complete -c arc -l 'ignore-unsound-tests' -d 'Ignore unsound test failures without prompting'
complete -c arc -l 'excuse' -d 'Provide a prepared in advance excuse for any lints/tests'
complete -c arc -l 'base' -d 'Additional rules for determining base revision'
complete -c arc -l 'no-amend' -d 'Never amend commits in the working copy with lint patches'
complete -c arc -l 'add-all' -d 'Automatically add all unstaged and uncommitted'
complete -c arc -l 'never-apply-patches' -d 'Never apply patches suggested by lint'
complete -c arc -l 'edit' -d 'Edit revision information'
complete -c arc -l 'nounit' -d 'Do not run unit tests'
complete -c arc -l 'lintall' -d 'Raise all lint warnings'
complete -c arc -l 'amend-all' -d 'When linting git repositories, amend HEAD with all patches'
complete -c arc -l 'no-diff' -d 'Only run lint and unit tests'
complete -c arc -l 'allow-untracked' -d 'Skip checks for untracked files in the working copy'
complete -c arc -l 'only-new' -d 'Display only new lint messages'
complete -c arc -l 'no-coverage' -d 'Always disable coverage information'
complete -c arc -d 'Create or update a libphutil library'
complete -c arc -l 'remap' -d 'Run the remap step of liberation'
complete -c arc -l 'upgrade' -d 'Upgrade library to v2'
complete -c arc -l 'verify' -d 'Run the verify step of liberation'
complete -c arc -l 'all' -d 'Drop the module cache before liberating'
complete -c arc -l 'force-update' -d 'Force the library map to be updated'
complete -c arc -l 'library-name' -d 'Set the library name'
complete -c arc -d 'Shows the current version of arcanist'
