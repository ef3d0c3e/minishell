complete -c diff -s 'i' -l 'ignore-case' -d 'Ignore case differences'
complete -c diff -l 'ignore-file-name-case' -d 'Ignore case when comparing file names'
complete -c diff -l 'no-ignore-file-name-case' -d 'Consider case when comparing file names'
complete -c diff -s 'E' -l 'ignore-tab-expansion' -d 'Ignore changes due to tab expansion'
complete -c diff -s 'b' -l 'ignore-space-change' -d 'Ignore changes in the amount of white space'
complete -c diff -s 'w' -l 'ignore-all-space' -d 'Ignore all white space'
complete -c diff -s 'B' -l 'ignore-blank-lines' -d 'Ignore changes whose lines are all blank'
complete -c diff -s 'I' -l 'ignore-matching-lines' -d 'Ignore changes whose lines match the REGEX'
complete -c diff -s 'a' -l 'text' -d 'Treat all files as text'
complete -c diff -s 'r' -l 'recursive' -d 'Recursively compare subdirectories'
complete -c diff -s 'N' -l 'new-file' -d 'Treat absent files as empty'
complete -c diff -s 'C' -l 'context' -d 'Output NUM lines of copied context'
complete -c diff -s 'c' -d 'Output 3 lines of copied context'
complete -c diff -s 'U' -d 'Output NUM lines of unified context'
complete -c diff -s 'u' -l 'unified' -d 'Output NUM lines of unified context (default 3)'
complete -c diff -s 'q' -l 'brief' -d 'Output only whether the files differ'
complete -c diff -l 'normal' -d 'Output a normal diff'
complete -c diff -s 'y' -l 'side-by-side' -d 'Output in two columns'
complete -c diff -s 'W' -l 'width' -d 'Output at most NUM print columns'
complete -c diff -s 'd' -l 'minimal' -d 'Try to find a smaller set of changes'
complete -c diff -l 'from-file' -d 'Compare FILE1 to all operands'
complete -c diff -l 'to-file' -d 'Compare FILE2 to all operands'
complete -c diff -s 'l' -l 'paginate' -d 'Pass the output through '"'"'pr'"'"''
complete -c diff -s 'v' -l 'version' -d 'Display version and exit'
complete -c diff -l 'help' -d 'Display help and exit'
complete -c diff -l 'color' -d 'Colorize the output'
