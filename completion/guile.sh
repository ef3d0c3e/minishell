complete -c $command -s 'h' -l 'help' -d 'Show help'
complete -c $command -s 'v' -l 'version' -d 'Show version'
complete -c $command -s 's' -d 'Specify the script to run'
complete -c $command -s 'c' -d 'Specify the code to run'
complete -c $command -s 'l' -d 'Specify the script to load'
complete -c $command -l 'debug' -d 'Use debug mode'
complete -c $command -l 'no-debug' -d 'Don'"'"'t use debug mode'
complete -c $command -s 'q' -d 'Don'"'"'t load .guile file'
complete -c $command -l 'auto-compile' -d 'Compile scripts automatically'
complete -c $command -l 'no-auto-compile' -d 'Don'"'"'t compile scripts automatically'
