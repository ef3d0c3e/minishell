NAME := minishell
CC := gcc
CFLAGS := -Wall -Wextra -pedantic -ggdb -fsanitize=address
IFLAGS := -I./src
LFLAGS := 

SOURCES := \
src/syntax/tokenize.c\
src/syntax/parser.c\
src/syntax/token.c\
src/util/unicode_util.c\
src/util/realloc.c\
src/util/string_buffer.c\
src/util/strlen.c\
src/util/memcpy_unaligned.c\
src/util/unicode.c\
src/util/xmalloc.c\
src/util/memcpy_aligned.c\
src/util/string.c\

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
