complete -c mono -s 'h' -l 'help' -d 'Show help'
complete -c mono -s 'V' -l 'version' -d 'Show version'
complete -c mono -l 'aot-path' -d 'Show additional directories to search for AOT images'
complete -c mono -l 'attach' -d 'Disable the attach functionality'
complete -c mono -l 'config' -d 'Read configuration from file instead of using default one'
complete -c mono -l 'gc' -d 'Use the Garbage Collector engine'
complete -c mono -l 'gc-debug' -d 'Specify MONO_GC_DEBUG environment variable value'
complete -c mono -l 'gc-params' -d 'Specify MONO_GC_PARAMS environment variable value'
complete -c mono -l 'interpreter' -d 'Use Mono interpreter to execute a given assembly'
complete -c mono -l 'hybrid-aot' -d 'Run assemblies that have been stripped of IL'
complete -c mono -l 'llvm' -d 'Use the LLVM optimization and code generation engine to JIT or AOT compile'
complete -c mono -l 'ffast-math' -d 'Use Mono and LLVM aggressive floating point optimizations'
complete -c mono -l 'response' -d 'Use a response file'
complete -c mono -l 'runtime' -d 'Use Mono version'
complete -c mono -l 'server' -d 'Optimize the virtual machine to be better suited for server operations'
complete -c mono -l 'use-map-jit' -d 'Generate code using MAP_JIT on MacOS'
complete -c mono -l 'profile' -d 'Use a profiler module with the given arguments'
complete -c mono -l 'trace' -d 'Show method names as they are invoked'
complete -c mono -l 'jitmap' -d 'Generate a JIT method map in a /tmp/perf-PID.map file'
complete -c mono -l 'bisect' -d 'This flag is used by the automatic optimization bug bisector'
complete -c mono -l 'breakonex' -d 'Use a breakpoint on exceptions'
complete -c mono -l 'compile' -d 'Compile a method (namespace.name:methodname)'
complete -c mono -l 'compile-all' -d 'Compile all the methods in an assembly'
complete -c mono -s 'v' -l 'verbose' -d 'Show more messages'
