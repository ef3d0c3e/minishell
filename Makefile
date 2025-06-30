NAME := minishell
CC := clang
CFLAGS := -Wall -Wextra -Werror -O2
IFLAGS := -I./src
LFLAGS := -lncurses -ltinfo

# Compiler
ifdef C
	CC := $(C)
endif

# Debug mode
ifeq ($(DBG),1)
	CFLAGS += -ggdb
endif

# Compile with musl for x86_64
ifeq ($(MUSL),x86_64)
	MUSL_LIBS ?= /usr/lib/x86_64-linux-musl/
	MUSL_INCS ?= /usr/include/x86_64-linux-musl/
	MUSL_LD ?= /lib/ld-musl-x86_64.so.1
	CFLAGS += -static -nostdinc -isystem $(MUSL_INCS)
	LDFLAGS += -nostdlib -L$(MUSL_LIBS) \
		-Wl,--dynamic-linker=$(MUSL_LD) \
		-Wl,-rpath,$(MUSL_LIBS) -lc
endif


# :^] `.!find src -name "*.c" -exec echo "{} \\" \;`
SOURCES := src/getline/input.c \
src/getline/term.c \
src/getline/action.c \
src/getline/history.c \
src/getline/getline.c \
src/getline/prompt.c \
src/getline/cursor.c \
src/getline/handlers/getc.c \
src/getline/handlers/overflow.c \
src/getline/handlers/words.c \
src/getline/handlers/handlers.c \
src/getline/wcwidth/wcwidth_table_wide.c \
src/getline/wcwidth/wcwidth_table_fusers3.c \
src/getline/wcwidth/wcwidth_table_fusers2.c \
src/getline/wcwidth/wcwidth_table_fusers1.c \
src/getline/wcwidth/wcwidth.c \
src/getline/wcwidth/wcwidth_table.c \
src/getline/util.c \
src/getline/draw.c \
src/getline/buffer/highlight.c \
src/getline/buffer/buffer.c \
src/getline/buffer/buffer_util.c \
src/getline/buffer/cluster.c \
src/getline/buffer/cluster_heuristics.c \
src/getline/modes/input.c \
src/getline/modes/modes.c \
src/getline/modes/history.c \
src/getline/modes/input_actions.c \
src/getline/modes/complete.c \
src/getline/modes/complete_items.c \
src/getline/modes/complete_draw_fn.c \
src/getline/modes/input_draw.c \
src/getline/modes/input_delete.c \
src/getline/modes/history_draw.c \
src/getline/modes/complete_actions.c \
src/getline/modes/history_actions.c \
src/getline/modes/complete_draw.c \
src/getline/modes/input_draw_buffer.c \
src/getline/modes/input_move.c \
src/getline/keys.c \
src/tokenizer/token_assign.c \
src/tokenizer/token_single_quote.c \
src/tokenizer/token_find.c \
src/tokenizer/token_digit.c \
src/tokenizer/tokenize.c \
src/tokenizer/token_pipeline.c \
src/tokenizer/token_word.c \
src/tokenizer/token_list.c \
src/tokenizer/token_double_quote.c \
src/tokenizer/token_grouping.c \
src/tokenizer/token_redir.c \
src/tokenizer/list_utils.c \
src/tokenizer/token_error.c \
src/tokenizer/token_comment.c \
src/tokenizer/token_minus.c \
src/tokenizer/token_meta.c \
src/tokenizer/reporting.c \
src/tokenizer/token_param_simple.c \
src/tokenizer/token_arith.c \
src/tokenizer/token_cmd_sub.c \
src/tokenizer/token_operator.c \
src/tokenizer/token_sequence.c \
src/tokenizer/token_param.c \
src/tokenizer/token.c \
src/tokenizer/token_util.c \
src/tokenizer/token_keyword.c \
src/tokenizer/expand.c \
src/parser/parse_if.c \
src/parser/delimiters.c \
src/parser/parse_operators.c \
src/parser/parse.c \
src/parser/parser_util.c \
src/parser/parse_func.c \
src/parser/parse_command.c \
src/parser/parser.c \
src/parser/parse_for.c \
src/parser/parse_simple_command.c \
src/parser/redirs/redir_util.c \
src/parser/redirs/parse_redir2.c \
src/parser/redirs/parse_redir4.c \
src/parser/redirs/parse_redir3.c \
src/parser/redirs/parse_redir.c \
src/parser/redirs/redir.c \
src/parser/redirs/parse_redir_move.c \
src/parser/redirs/heredoc.c \
src/parser/ast/node_if.c \
src/parser/ast/node_for.c \
src/parser/ast/node_block.c \
src/parser/ast/node_list.c \
src/parser/ast/node_command.c \
src/parser/ast/node_loop.c \
src/parser/ast/ast.c \
src/parser/ast/node_subshell.c \
src/parser/ast/node_function.c \
src/parser/ast/node_logic.c \
src/parser/parser_util2.c \
src/parser/parse_loop.c \
src/parser/words/wordlist.c \
src/parser/words/word.c \
src/parser/words/word_sub.c \
src/parser/words/word_util.c \
src/parser/words/atom.c \
src/shell/opts.c \
src/shell/shell_wrappers.c \
src/shell/shell.c \
src/shell/error.c \
src/shell/utils.c \
src/shell/repl/highlighter.c \
src/shell/repl/data.c \
src/shell/repl/complete.c \
src/shell/repl/complete_util.c \
src/shell/repl/repl.c \
src/shell/repl/complete_filename.c \
src/shell/repl/complete_opts.c \
src/shell/repl/complete_cmd.c \
src/shell/repl/complete_param.c \
src/shell/main.c \
src/shell/args.c \
src/shell/env/envp.c \
src/shell/env/assign_prefix.c \
src/shell/env/path.c \
src/shell/env/profile.c \
src/shell/env/resolve.c \
src/shell/env/passwd.c \
src/shell/env/pathbuf.c \
src/shell/env/variable.c \
src/shell/env/ftw.c \
src/shell/env/envar.c \
src/shell/env/assign.c \
src/shell/temporaries.c \
src/shell/signal.c \
src/shell/regex/matcher.c \
src/shell/regex/regex_util.c \
src/shell/regex/builder.c \
src/shell/regex/matcher_glob.c \
src/shell/regex/regex_ast.c \
src/shell/regex/matcher_seq.c \
src/shell/regex/regex.c \
src/shell/regex/parser.c \
src/shell/redir/undo.c \
src/shell/redir/redir_internal.c \
src/shell/redir/redir_internal_file.c \
src/shell/redir/redir_internal_close.c \
src/shell/redir/redir_internal_dup.c \
src/shell/redir/redir.c \
src/shell/redir/open.c \
src/shell/redir/redir_internal_heredoc.c \
src/shell/eval/eval_for.c \
src/shell/eval/eval_util.c \
src/shell/eval/eval_function.c \
src/shell/eval/eval_cmd.c \
src/shell/eval/eval_builtin.c \
src/shell/eval/eval_loop.c \
src/shell/eval/eval_pipe.c \
src/shell/eval/eval_list.c \
src/shell/eval/eval.c \
src/shell/eval/eval_sequence.c \
src/shell/eval/eval_special.c \
src/shell/eval/eval_subshell.c \
src/shell/eval/eval_exec.c \
src/shell/eval/eval_if.c \
src/shell/ctx/ctx.c \
src/shell/ctx/evaluator.c \
src/shell/ctx/evaluator_string.c \
src/shell/fds/fds_util.c \
src/shell/fds/fds_data.c \
src/shell/fds/fds_wrappers2.c \
src/shell/fds/fds_wrappers.c \
src/shell/funs/funs.c \
src/shell/expand/param.c \
src/shell/expand/subexpr.c \
src/shell/expand/tilde.c \
src/shell/expand/brace_split.c \
src/shell/expand/brace_parse.c \
src/shell/expand/word_splitting.c \
src/shell/expand/braces.c \
src/shell/expand/brace_range.c \
src/shell/expand/fraglist.c \
src/shell/expand/expand_arg.c \
src/shell/expand/filename.c \
src/shell/expand/filename_util.c \
src/shell/expand/brace_group.c \
src/shell/expand/expand.c \
src/builtins/source.c \
src/builtins/env.c \
src/builtins/type.c \
src/builtins/export.c \
src/builtins/cowsay/cowsay.c \
src/builtins/cowsay/builtin.c \
src/builtins/pwd.c \
src/builtins/shopt.c \
src/builtins/complete/complete.c \
src/builtins/complete/builtin.c \
src/builtins/builtin.c \
src/builtins/unset.c \
src/builtins/fds.c \
src/builtins/echo.c \
src/builtins/cd.c \
src/util/memory/realloc.c \
src/util/memory/memmove.c \
src/util/memory/memcpy_unaligned.c \
src/util/memory/xmalloc.c \
src/util/memory/memcpy_aligned.c \
src/util/memory/memset.c \
src/util/arith.c \
src/util/string/unicode_util.c \
src/util/string/string_buffer.c \
src/util/string/strstr.c \
src/util/string/strlen.c \
src/util/string/string_buffer_append.c \
src/util/string/unicode.c \
src/util/string/string_buffer_util.c \
src/util/string/string_search.c \
src/util/string/string_buffer_create.c \
src/util/string/strdup.c \
src/util/string/str_from_range.c \
src/util/string/strcmp.c \
src/util/string/strchr.c \
src/util/string/strncmp.c \
src/util/string/string.c \
src/util/rbtree/rbtree_delete.c \
src/util/rbtree/rbtree_delete_fixup.c \
src/util/rbtree/rbtree_find.c \
src/util/rbtree/rbtree_insert.c \
src/util/rbtree/rbtree.c \
src/util/rbtree/rbtree_util.c \
src/util/quicksort.c \
src/term/capability.c

SOURCES_TESTER := $(filter-out %main.c,$(SOURCES))
SOURCES_TESTER += $(wildcard tester/*.c)

# Objects
OBJECTS := $(addprefix build/,$(SOURCES:.c=.o))
OBJECTS_TESTER := $(addprefix build/,$(SOURCES_TESTER:.c=.o))
# Libraries
LIB_PRINTF := ./libs/ft_printf/libftprintf.a
IFLAGS += -I$(dir $(LIB_PRINTF))includes/
LIB_GNL := ./libs/ft_gnl/libgnl.a
IFLAGS += -I$(dir $(LIB_GNL))includes/
LIB_OPTS := ./libs/libopts/libopts.a
IFLAGS += -I$(dir $(LIB_OPTS))includes/

build/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

# Default target
$(NAME): LFLAGS += $(LIB_PRINTF) $(LIB_GNL) $(LIB_OPTS)
$(NAME): $(LIB_PRINTF) $(LIB_GNL) $(LIB_OPTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LFLAGS)

# Tester
ITESTS := \
	-include ./tester/tests/echo.tests \
	-include ./tester/tests/grammar.tests \
	-include ./tester/tests/param.tests \
	-include ./tester/tests/pipe.tests \
	-include ./tester/tests/redir.tests \
	-include ./tester/tests/expand.tests \
	-include ./tester/tests/control.tests \
	-include ./tester/tests/regex.tests \
	-include ./tester/tests/sub.tests
tests: CFLAGS += -D_XOPEN_SOURCE=500
tests: LFLAGS += $(LIB_PRINTF) $(LIB_GNL) $(LIB_OPTS)
tests: $(LIB_PRINTF) $(LIB_GNL) $(LIB_OPTS) $(OBJECTS_TESTER)
	$(CC) $(IFLAGS) $(ITESTS) $(CFLAGS) -o $@ ./tester/tests/runner.c $(OBJECTS_TESTER) $(LFLAGS)

# Libraries build
# ft_printf
$(LIB_PRINTF):
	echo "Building libprintf..."
	$(MAKE) -C $(dir $(LIB_PRINTF))

# ft_gnl
$(LIB_GNL):
	echo "Building gnl..."
	$(MAKE) -C $(dir $(LIB_GNL))

# libots
$(LIB_OPTS):
	echo "Building libopts..."
	$(MAKE) -C $(dir $(LIB_OPTS))

# Create compile commands
.PHONY: clangd
clangd:
	@mkdir -p build
	cd build && cmake .. && cp compile_commands.json ..

# Build doxygen
.PHONY: docs
docs:
	@mkdir -p docs
	doxygen doxygen

.PHONY: all
bonus: $(NAME)

.PHONY: all
all: $(NAME)

.PHONY: clean
clean:
	$(RM) -r build/

.PHONY: lclean
lclean:
	echo "Cleaning printf..."
	cd $(dir $(LIB_PRINTF)) && make fclean
	echo "Cleaning gnl..."
	cd $(dir $(LIB_GNL)) && make fclean
	echo "Cleaning opts..."
	cd $(dir $(LIB_OPTS)) && make fclean

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)
	$(RM) tests

.PHONY: re
re: fclean all
