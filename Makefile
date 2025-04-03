NAME := minishell
CC := gcc
CFLAGS := -Wall -Wextra -pedantic -ggdb -fsanitize=address
IFLAGS := -I./src
LFLAGS := 

# :^] `.!find src -name "*.c" -exec echo "{} \\" \;`
SOURCES := $(wildcard src/*.c) \
	$(wildcard src/util/memory/*.c) \
	$(wildcard src/util/rbtree/*.c) \
	$(wildcard src/util/string/*.c) \
	$(wildcard src/tokenizer/*.c) \
	$(wildcard src/expansion/*.c) \
	$(wildcard src/shell/*.c) \
	$(wildcard src/parser/*.c)

# Objects
OBJECTS := $(addprefix build/,$(SOURCES:.c=.o))
# Libraries
LIB_PRINTF := ./libs/ft_printf/libftprintf.a

build/%.o: IFLAGS += -I$(dir $(LIB_PRINTF))/includes/
build/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

# Default target
$(NAME): LFLAGS += $(LIB_PRINTF)
$(NAME): $(LIB_PRINTF) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LFLAGS)

# Libraries build
# ft_printf
$(LIB_PRINTF):
	echo "Building libprintf..."
	$(MAKE) -C $(dir $(LIB_PRINTF))

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

.PHONY: re
re: fclean all
