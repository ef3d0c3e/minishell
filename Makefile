NAME := minishell
CC := gcc
CFLAGS := -Wall -Wextra -pedantic -ggdb -fsanitize=address
IFLAGS := -I./src
LFLAGS := 

SOURCES := \
src/syntax/parser.c \
src/syntax/unicode.c \
src/util/strlen.c \
src/util/string_view.c

# Objects
OBJECTS := $(addprefix objs/,$(SOURCES:.c=.o))
# Libraries
LIB_PRINTF := ./libs/ft_printf/libftprintf.a

objs/%.o: IFLAGS += -I$(dir $(LIB_PRINTF))/includes/
objs/%.o: %.c
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

.PHONY: all
all: $(NAME)

.PHONY: clean
clean:
	$(RM) $(OBJECTS)

.PHONY: lclean
lclean:
	echo "Cleaning printf..."
	cd $(dir $(LIB_PRINTF)) && make fclean

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all
