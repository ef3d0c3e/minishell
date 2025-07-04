complete -c gitk -l 'all' -d 'Show all refs (branches, tags, etc.)'
complete -c gitk -l 'since=YYYY-MM-DD' -d 'Show commits more recent that a specific date'
complete -c gitk -l 'until=YYYY-MM-DD' -d 'Show commits older than a specific date'
complete -c gitk -l 'date-order' -d 'Sort commits by date when possible'
complete -c gitk -l 'merge' -d 'On a merge conflict, show commits that modify conflicting files'
complete -c gitk -l 'left-right' -d 'Mark which side of a symmetric difference a commit is reachable from'
complete -c gitk -l 'full-history' -d 'When filtering history with -- path..., do not prune some history'
complete -c gitk -l 'simplify-merges' -d 'Hide needless merges from history'
complete -c gitk -l 'ancestry-path' -d 'Only display commits that exist directly on the ancestry chain between the range'
complete -c gitk -l 'argscmd=' -d 'Command to be run to determine th revision range to show'
complete -c gitk -l 'select-commit=' -d 'Select the specified commit after loading the graph, instead of HEAD'
complete -c gitk -l 'select-commit=HEAD' -d 'Select the specified commit after loading the graph, instead of HEAD'
complete -c gitk -s 'n' -l 'max-count' -d 'Limit the number of commits to output'
complete -c gitk -d '-L<start>,<end>:<file> trace the evolution of a line range'
complete -c gitk -d '-L<funcname>:<file> trace the evolution of a function name regex'
