NAME := minishell
CC := gcc
CFLAGS := -Wall -Wextra -pedantic -ggdb
IFLAGS := -I./src
LFLAGS :=

# :^] `.!find src -name "*.c" -exec echo "{} \\" \;`
SOURCES := $(wildcard src/*.c) \
	$(wildcard src/util/*.c) \
	$(wildcard src/util/memory/*.c) \
	$(wildcard src/util/rbtree/*.c) \
	$(wildcard src/util/string/*.c) \
	$(wildcard src/tokenizer/*.c) \
	$(wildcard src/expansion/*.c) \
	$(wildcard src/builtins/*.c) \
	$(wildcard src/shell/*.c) \
	$(wildcard src/shell/redir/*.c) \
	$(wildcard src/parser/*.c)

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

build/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

# Default target
$(NAME): LFLAGS += $(LIB_PRINTF) $(LIB_GNL)
$(NAME): $(LIB_PRINTF) $(LIB_GNL) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LFLAGS)

# Tester
ITESTS := \
	-include ./tester/tests/echo.tests \
	-include ./tester/tests/grammar.tests \
	-include ./tester/tests/param.tests \
	-include ./tester/tests/pipe.tests \
	-include ./tester/tests/redir.tests \
	-include ./tester/tests/sub.tests
tests: CFLAGS += -D_XOPEN_SOURCE=500
tests: LFLAGS += $(LIB_PRINTF) $(LIB_GNL)
tests: $(LIB_PRINTF) $(LIB_GNL) $(OBJECTS_TESTER)
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
all: $(NAME)

.PHONY: clean
clean:
	$(RM) -r build/

.PHONY: lclean
lclean:
	echo "Cleaning printf..."
	cd $(dir $(LIB_PRINTF)) && make fclean

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)
	$(RM) tests

.PHONY: re
re: fclean all
