NAME := minishell
CC := clang
CFLAGS := -Wall -Wextra -O2
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
SOURCES := $(shell find src -name '*.c')

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
