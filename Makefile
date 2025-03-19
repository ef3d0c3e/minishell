NAME := minishell
CC := gcc
CFLAGS := -Wall -Wextra -pedantic -ggdb -fsanitize=address
IFLAGS := -I./src
LFLAGS := 

SOURCES := $(wildcard src/*.c) \
	$(wildcard src/util/*.c) \
	$(wildcard src/syntax/*.c)

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
