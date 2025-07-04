complete -c node -s 'v' -l 'version' -d 'Print node'"'"'s version'
complete -c node -s 'e' -l 'eval' -d 'Evaluate script'
complete -c node -s 'p' -l 'print' -d 'Print result of --eval'
complete -c node -s 'i' -l 'interactive' -d 'Always enter the REPL even if stdin does not appear to be a terminal'
complete -c node -l 'no-deprecation' -d 'Silence deprecation warnings'
complete -c node -l 'trace-deprecation' -d 'Show stack traces on deprecations'
complete -c node -l 'throw-deprecation' -d 'Throw errors on deprecations'
complete -c node -l 'v8-options' -d 'Print v8 command line options'
complete -c node -l 'max-stack-size' -d 'Set max v8 stack size (bytes)'
complete -c node -l 'use_strict' -d 'enforce strict mode'
complete -c node -l 'es5_readonly' -d 'activate correct semantics for inheriting readonliness'
complete -c node -l 'es52_globals' -d 'activate new semantics for global var declarations'
complete -c node -l 'harmony_typeof' -d 'enable harmony semantics for typeof'
complete -c node -l 'harmony_scoping' -d 'enable harmony block scoping'
complete -c node -l 'harmony_modules' -d 'enable harmony modules (implies block scoping)'
complete -c node -l 'harmony_proxies' -d 'enable harmony proxies'
complete -c node -l 'harmony_collections' -d 'enable harmony collections (sets, maps, and weak maps)'
complete -c node -l 'harmony' -d 'enable all harmony features (except typeof)'
complete -c node -l 'packed_arrays' -d 'optimizes arrays that have no holes'
complete -c node -l 'smi_only_arrays' -d 'tracks arrays with only smi values'
complete -c node -l 'clever_optimizations' -d 'Optimize object size, Array shift, DOM strings and string +'
complete -c node -l 'unbox_double_arrays' -d 'automatically unbox arrays of doubles'
complete -c node -l 'string_slices' -d 'use string slices'
complete -c node -l 'crankshaft' -d 'use crankshaft'
complete -c node -l 'hydrogen_filter' -d 'optimization filter'
complete -c node -l 'use_range' -d 'use hydrogen range analysis'
complete -c node -l 'eliminate_dead_phis' -d 'eliminate dead phis'
complete -c node -l 'use_gvn' -d 'use hydrogen global value numbering'
complete -c node -l 'use_canonicalizing' -d 'use hydrogen instruction canonicalizing'
complete -c node -l 'use_inlining' -d 'use function inlining'
complete -c node -l 'max_inlined_source_size' -d 'max source size in bytes considered for single inlining'
complete -c node -l 'max_inlined_nodes' -d 'max number of AST nodes considered for single inlining'
complete -c node -l 'max_inlined_nodes_cumulative' -d 'max cumulative number of AST nodes considered for inlining'
complete -c node -l 'loop_invariant_code_motion' -d 'loop invariant code motion'
complete -c node -l 'collect_megamorphic_maps_from_stub_cache' -d 'crankshaft harvests type feedback from stub cache'
complete -c node -l 'hydrogen_stats' -d 'print statistics for hydrogen'
complete -c node -l 'trace_hydrogen' -d 'trace generated hydrogen to file'
complete -c node -l 'trace_phase' -d 'trace generated IR for specified phases'
complete -c node -l 'trace_inlining' -d 'trace inlining decisions'
complete -c node -l 'trace_alloc' -d 'trace register allocator'
complete -c node -l 'trace_all_uses' -d 'trace all use positions'
complete -c node -l 'trace_range' -d 'trace range analysis'
complete -c node -l 'trace_gvn' -d 'trace global value numbering'
complete -c node -l 'trace_representation' -d 'trace representation types'
complete -c node -l 'stress_pointer_maps' -d 'pointer map for every instruction'
complete -c node -l 'stress_environments' -d 'environment for every instruction'
complete -c node -l 'deopt_every_n_times' -d 'deoptimize every n times a deopt point is passed'
complete -c node -l 'trap_on_deopt' -d 'put a break point before deoptimizing'
complete -c node -l 'deoptimize_uncommon_cases' -d 'deoptimize uncommon cases'
complete -c node -l 'polymorphic_inlining' -d 'polymorphic inlining'
complete -c node -l 'use_osr' -d 'use on-stack replacement'
complete -c node -l 'array_bounds_checks_elimination' -d 'perform array bounds checks elimination'
complete -c node -l 'array_index_dehoisting' -d 'perform array index dehoisting'
complete -c node -l 'trace_osr' -d 'trace on-stack replacement'
complete -c node -l 'stress_runs' -d 'number of stress runs'
complete -c node -l 'optimize_closures' -d 'optimize closures'
complete -c node -l 'inline_construct' -d 'inline constructor calls'
complete -c node -l 'inline_arguments' -d 'inline functions with arguments object'
complete -c node -l 'loop_weight' -d 'loop weight for representation inference'
complete -c node -l 'optimize_for_in' -d 'optimize functions containing for-in loops'
complete -c node -l 'experimental_profiler' -d 'enable all profiler experiments'
complete -c node -l 'watch_ic_patching' -d 'profiler considers IC stability'
complete -c node -l 'frame_count' -d 'number of stack frames inspected by the profiler'
complete -c node -l 'self_optimization' -d 'primitive functions trigger their own optimization'
complete -c node -l 'direct_self_opt' -d 'call recompile stub directly when self-optimizing'
complete -c node -l 'retry_self_opt' -d 're-try self-optimization if it failed'
complete -c node -l 'count_based_interrupts' -d 'trigger profiler ticks based on counting instead of timing'
complete -c node -l 'interrupt_at_exit' -d 'insert an interrupt check at function exit'
complete -c node -l 'weighted_back_edges' -d 'weight back edges by jump distance for interrupt triggering'
complete -c node -l 'interrupt_budget' -d 'execution budget before interrupt is triggered'
complete -c node -l 'type_info_threshold' -d '% of ICs that must have type info to allow optimization'
complete -c node -l 'self_opt_count' -d 'call count before self-optimization'
complete -c node -l 'trace_opt_verbose' -d 'extra verbose compilation tracing'
complete -c node -l 'debug_code' -d 'generate extra code (assertions) for debugging'
complete -c node -l 'code_comments' -d 'emit comments in code disassembly'
complete -c node -l 'enable_sse2' -d 'enable use of SSE2 instructions if available'
complete -c node -l 'enable_sse3' -d 'enable use of SSE3 instructions if available'
complete -c node -l 'enable_sse4_1' -d 'enable use of SSE4'
complete -c node -l 'enable_cmov' -d 'enable use of CMOV instruction if available'
complete -c node -l 'enable_rdtsc' -d 'enable use of RDTSC instruction if available'
complete -c node -l 'enable_sahf' -d 'enable use of SAHF instruction if available (X64 only)'
complete -c node -l 'enable_vfp3' -d 'enable use of VFP3 instructions if available (ARM only)'
complete -c node -l 'enable_armv7' -d 'enable use of ARMv7 instructions if available (ARM only)'
complete -c node -l 'enable_fpu' -d 'enable use of MIPS FPU instructions if available (MIPS only)'
complete -c node -l 'expose_natives_as' -d 'expose natives in global object'
complete -c node -l 'expose_debug_as' -d 'expose debug in global object'
complete -c node -l 'expose_gc' -d 'expose gc extension'
complete -c node -l 'expose_externalize_string' -d 'expose externalize string extension'
complete -c node -l 'stack_trace_limit' -d 'number of stack frames to capture'
complete -c node -l 'builtins_in_stack_traces' -d 'show built-in functions in stack traces'
complete -c node -l 'disable_native_files' -d 'disable builtin natives files'
complete -c node -l 'inline_new' -d 'use fast inline allocation'
complete -c node -l 'stack_trace_on_abort' -d 'print a stack trace if an assertion failure occurs'
complete -c node -l 'trace' -d 'trace function calls'
complete -c node -l 'mask_constants_with_cookie' -d 'use random jit cookie to mask large constants'
complete -c node -l 'lazy' -d 'use lazy compilation'
complete -c node -l 'trace_opt' -d 'trace lazy optimization'
complete -c node -l 'trace_opt_stats' -d 'trace lazy optimization statistics'
complete -c node -l 'opt' -d 'use adaptive optimizations'
complete -c node -l 'always_opt' -d 'always try to optimize functions'
complete -c node -l 'prepare_always_opt' -d 'prepare for turning on always opt'
complete -c node -l 'sparkplug' -d 'use non-optimizing sparkplug compiler'
complete -c node -l 'always_sparkplug' -d 'always use non-optimizing sparkplug compiler'
complete -c node -l 'trace_deopt' -d 'trace deoptimization'
complete -c node -l 'min_preparse_length' -d 'minimum length for automatic enable preparsing'
complete -c node -l 'always_full_compiler' -d 'try to use the dedicated run-once backend for all code'
complete -c node -l 'trace_bailout' -d 'print reasons for falling back to the classic V8 backend'
complete -c node -l 'compilation_cache' -d 'enable compilation cache'
complete -c node -l 'cache_prototype_transitions' -d 'cache prototype transitions'
complete -c node -l 'trace_debug_json' -d 'trace debugging JSON request/response'
complete -c node -l 'debugger_auto_break' -d 'autoset debug break flag when debugger commands are in queue'
complete -c node -l 'enable_liveedit' -d 'enable liveedit experimental feature'
complete -c node -l 'break_on_abort' -d 'always cause a debug break before aborting'
complete -c node -l 'stack_size' -d 'default size of stack region v8 is allowed to use (in kBs)'
complete -c node -l 'max_stack_trace_source_length' -d 'max length of function source code printed in stack trace'
complete -c node -l 'always_inline_smi_code' -d 'always inline smi code in non-opt code'
complete -c node -l 'max_new_space_size' -d 'max size of the new generation (in kBytes)'
complete -c node -l 'max_old_space_size' -d 'max size of the old generation (in Mbytes)'
complete -c node -l 'max_executable_size' -d 'max size of executable memory (in Mbytes)'
complete -c node -l 'gc_global' -d 'always perform global GCs'
complete -c node -l 'gc_interval' -d 'garbage collect after <n> allocations'
complete -c node -l 'trace_gc' -d 'print 1 trace line following each garbage collection'
complete -c node -l 'trace_gc_nvp' -d 'print 1 full trace(w/ name=value) after garbage collections'
complete -c node -l 'print_cumulative_gc_stat' -d 'print cumulative GC statistics(w/ name=value) on exit'
complete -c node -l 'trace_gc_verbose' -d 'print more details following each garbage collection'
complete -c node -l 'trace_fragmentation' -d 'report fragmentation for old pointer and data pages'
complete -c node -l 'collect_maps' -d 'garbage collect maps from which no objects can be reached'
complete -c node -l 'flush_code' -d 'flush code that we expect not to use again before full gc'
complete -c node -l 'incremental_marking' -d 'use incremental marking'
complete -c node -l 'incremental_marking_steps' -d 'do incremental marking steps'
complete -c node -l 'trace_incremental_marking' -d 'trace progress of the incremental marking'
complete -c node -l 'use_idle_notification' -d 'Use idle notification to reduce memory footprint'
complete -c node -l 'send_idle_notification' -d 'Send idle notification between stress runs'
complete -c node -l 'use_ic' -d 'use inline caching'
complete -c node -l 'native_code_counters' -d 'generate extra code for manipulating stats counters'
complete -c node -l 'always_compact' -d 'Perform compaction on every full GC'
complete -c node -l 'lazy_sweeping' -d 'Use lazy sweeping for old pointer and data spaces'
complete -c node -l 'never_compact' -d 'Never perform compaction on full GC - testing only'
complete -c node -l 'compact_code_space' -d 'Compact code space on full non-incremental collections'
complete -c node -l 'cleanup_code_caches_at_gc' -d 'Flush inline/code caches prior to mark compact collection/cycle'
complete -c node -l 'random_seed' -d 'Default seed for random generator init(0:default-sys random)'
complete -c node -l 'use_verbose_printer' -d 'allows verbose printing'
complete -c node -l 'allow_natives_syntax' -d 'allow natives syntax'
complete -c node -l 'trace_sim' -d 'Trace simulator execution'
complete -c node -l 'check_icache' -d 'Check icache flushes in ARM and MIPS simulator'
complete -c node -l 'stop_sim_at' -d 'Simulator stop after x number of instructions'
complete -c node -l 'sim_stack_alignment' -d 'Stack alignment in bytes in simulator (4/8, 8 is default)'
complete -c node -l 'trace_exception' -d 'print stack trace when throwing exceptions'
complete -c node -l 'preallocate_message_memory' -d 'preallocate some memory to build stack traces'
complete -c node -l 'randomize_hashes' -d 'randomize hashes(w/ snapshots cannot override baked-in seed)'
complete -c node -l 'hash_seed' -d 'Fix seed to hash prop. key(w/ snapshots no BI seed overr.)'
complete -c node -l 'preemption' -d 'activate a 100ms timer that switches between V8 threads'
complete -c node -l 'regexp_optimization' -d 'generate optimized regexp code'
complete -c node -l 'testing_bool_flag' -d 'testing_bool_flag'
complete -c node -l 'testing_int_flag' -d 'testing_int_flag'
complete -c node -l 'testing_float_flag' -d 'float-flag'
complete -c node -l 'testing_string_flag' -d 'string-flag'
complete -c node -l 'testing_prng_seed' -d 'Seed used for threading test randomness'
complete -c node -l 'testing_serialization_file' -d 'file in which to serialize heap'
complete -c node -l 'help' -d 'Print usage message, including flags, on console'
complete -c node -l 'dump_counters' -d 'Dump counters on exit'
complete -c node -l 'debugger' -d 'Enable JavaScript debugger'
complete -c node -l 'remote_debugger' -d 'Connect JS debugger to the debugger agent in another process'
complete -c node -l 'debugger_agent' -d 'Enable debugger agent'
complete -c node -l 'debugger_port' -d 'Port to use for remote debugging'
complete -c node -l 'map_counters' -d 'Map counters to a file'
complete -c node -l 'js_arguments' -d 'Pass all remaining arguments to the script'
complete -c node -l 'debug_compile_events' -d 'Enable debugger compile events'
complete -c node -l 'debug_script_collected_events' -d 'Enable debugger script collected events'
complete -c node -l 'gdbjit' -d 'enable GDBJIT interface (disables compacting GC)'
complete -c node -l 'gdbjit_full' -d 'enable GDBJIT interface for all code objects'
complete -c node -l 'gdbjit_dump' -d 'dump elf objects with debug info to disk'
complete -c node -l 'gdbjit_dump_filter' -d 'dump only objects containing this substring'
complete -c node -l 'force_marking_deque_overflows' -d 'force overflow of marking deque by reducing size to 64 words'
complete -c node -l 'stress_compaction' -d 'stress the GC compactor to flush out bugs'
complete -c node -l 'log' -d 'Minimal logging (no API,code,GC,suspect,or handles samples)'
complete -c node -l 'log_all' -d 'Log all events to the log file'
complete -c node -l 'log_runtime' -d 'Activate runtime system %Log call'
complete -c node -l 'log_api' -d 'Log API events to the log file'
complete -c node -l 'log_code' -d 'Log code events to the log file without profiling'
complete -c node -l 'log_gc' -d 'Log heap samples on garbage collection for the hp2ps tool'
complete -c node -l 'log_handles' -d 'Log global handle events'
complete -c node -l 'log_snapshot_positions' -d 'log positions of (de)serialized objects in the snapshot'
complete -c node -l 'log_suspect' -d 'Log suspect operations'
complete -c node -l 'prof' -d 'Log statistical profiling information (implies --log-code)'
complete -c node -l 'prof_auto' -d 'Used w/ --prof,starts profiling automatically)'
complete -c node -l 'prof_lazy' -d 'Used w/ --prof,only samples and logs w/ active profiler'
complete -c node -l 'prof_browser_mode' -d 'Used w/ --prof,set on browser-compatible mode for profiling'
complete -c node -l 'log_regexp' -d 'Log regular expression execution'
complete -c node -l 'sliding_state_window' -d 'Update sliding state window counters'
complete -c node -l 'logfile' -d 'Specify the name of the log file'
complete -c node -l 'll_prof' -d 'Enable low-level linux profiler'
