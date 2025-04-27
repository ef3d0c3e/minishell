/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PARSER_H
# define PARSER_H

# include <tokenizer/tokenizer.h>
# include <parser/redir_parser.h>

typedef struct s_ast_node	t_ast_node;
typedef struct s_parser		t_parser;

/******************************************************************************/
/* Argument nodes                                                             */
/******************************************************************************/

/** @brief The type of an argument item */
enum e_arg_type
{
	/** @brief Content is to be interpreted as literal text */
	ARG_LITERAL,
	/** @brief Content is to be interpreted as a variable needing expansion
	 * at run-time */
	ARG_PARAMETER,
	/** @brief Sub expression, needs evaluation at run-time */
	ARG_SUBEXPR,
};

/** @brief Data for parameter expaision */
struct s_arg_param
{
	/** @brief Parameter name */
	char			*name;
	/** @brief The special operator: ":-", "-", ":=", "=", ":?", "?", ":+", "+",
	 * "#", "##", "%", "%%", ":offset", ":offset:length" (or NULL) */
	const char		*op;
	/** @brief Ast for the special operator */
	t_ast_node		*word;
	/** @brief Offset for substring */
	size_t			offset;
	/** @brief Length for substring */
	size_t			length;
};

/** @brief A single argument item */
struct s_arg_item
{
	/** @brief Flags of the source token */
	int						flags;
	/** @brief Type of arugment */
	enum e_arg_type			type;
	union
	{
		/** @brief Text data */
		t_string_buffer		text;
		/** @brief Param data */
		struct s_arg_param	param;
	};
};

/** @brief Data for command arguments */
struct s_argument
{
	/** @brief Items in this argument */
	struct s_arg_item	*items;
	/** @brief Number of items in this argument */
	size_t				nitems;
};

/**
 * @brief Parses parameter token into argument
 *
 * @param parser The parser
 * @param arg Argument to parse into
 * @param pos Position of the token to parse
 */
void
parse_param(t_parser *parser, struct s_arg_item *arg, size_t pos);

/** @brief Pushes a token to an argument */
void
arg_push(t_parser *parser, struct s_argument *arg, size_t pos);
/** @brief Frees an argument structure */
void
arg_free(struct s_argument *arg);
/** @brief Displays an argument to stderr */
void
arg_print(size_t depth, struct s_argument *arg);
/** @brief Frees an array of arguments */
void
arglist_free(struct s_argument *list, size_t size);
/** @brief Displays an array of arguments to stderr */
void
arglist_print(size_t depth, struct s_argument *list, size_t size);

/******************************************************************************/
/* Assignment nodes                                                           */
/******************************************************************************/

/** @brief A variable assignment */
struct s_assignment
{
	/** @brief Name of the variable being assigned to */
	t_string_buffer		*variable;
	/** @brief Value to assign to `variable`, may require lazy expansion */
	struct s_argument	value;
};

/** @brief Frees an array of assignments */
void
assignlist_free(struct s_argument *list, size_t size);
/** @brief Displays an array of assignments to stderr */
void
assignlist_print(size_t depth, struct s_argument *list, size_t size);

/******************************************************************************/
/* The AST                                                                    */
/******************************************************************************/

typedef struct s_ast_node	t_ast_node;

/** @brief Type for nodes */
enum e_node_type
{
	/** @brief Block node, delimited by `{` and `}` */
	NODE_BLOCK,
	/** @brief Expression that should run in a sub-shell, delimited by `{` and
	 * `}` */
	NODE_SUBSHELL,
	/** @brief Command */
	NODE_COMMAND,
	/** @brief Command list */
	NODE_LIST,
	/** @brief Binary logic operator, e.g `||`, `|&`, `>` */
	NODE_LOGIC,
	/** @brief Function definition node */
	NODE_FUNCTION,
	/** @brief If statement node */
	NODE_IF,
	/** @brief While statement node */
	NODE_WHILE,
};

/** @brief A block node */
struct s_node_block
{
	/** @brief Subshell inner */
	t_ast_node		*inner;
};

/** @brief Creates a block node */
t_ast_node
*make_block_node(t_ast_node *inner);
/** @brief Frees a block node */
void
free_block_node(t_ast_node *node);
/** @brief Prints a block node */
void
print_block_node(size_t depth, const t_ast_node *node);

/** @brief Sub expression data */
struct s_subshell_node
{
	/** @brief Subshell inner */
	t_ast_node		*head;
	/** @brief Redirections */
	t_redirections	redirs;
};

/** @brief Creates a subshell node */
t_ast_node
*make_subshell_node(t_ast_node *inner);
/** @brief Frees a subshell node */
void
free_subshell_node(t_ast_node *node);
/** @brief Prints a subshell node */
void
print_subshell_node(size_t depth, const t_ast_node *node);

/** @brief Command name and arguments */
struct s_cmd_node
{
	/** @brief Program arguments */
	struct s_argument	*args;
	/** @brief Number of arguments */
	size_t				nargs;
	/** @brief Assignments for this command */
	struct s_assignment	*assigns;
	/** @brief Number of assignments */
	size_t				nassigns;
	/** @brief Redirections */
	t_redirections		redirs;
};

/** @brief Creates a command node */
t_ast_node
*make_cmd_node(void);
/** @brief Frees a command node */
void
free_cmd_node(t_ast_node *node);
/** @brief Prints a command node */
void
print_cmd_node(size_t depth, const t_ast_node *node);

/** @brief Command list node */
struct s_cmdlist_node
{
	/** @brief The commands */
	t_ast_node	**cmds;
	/** @brief Separator tokens */
	char		*seps;
	/** @brief Number of commands */
	size_t		ncmds;
};

t_ast_node
*make_list_node(void);
/** @brief Frees a command list node */
void
free_list_node(t_ast_node *node);
/** @brief Prints a command list node */
void
print_list_node(size_t depth, const t_ast_node *node);
/**
 * @brief Pushes a command to a command list
 *
 * @param list The command list to push to
 * @param cmd Command to push to the command list
 * @param sep Separator
 */
void
list_node_push(t_ast_node *list, t_ast_node *cmd, char sep);

/** @brief Data for logic (binary) nodes */
struct s_logic_node
{
	/** @brief Operator token */
	t_token		token;
	/** @brief Left operand */
	t_ast_node	*left;
	/** @brief Right operand */
	t_ast_node	*right;
};

/** @brief Creates a logic node */
t_ast_node
*make_logic_node(t_token op, t_ast_node *left, t_ast_node *right);
/** @brief Frees a logic node */
void
free_logic_node(t_ast_node *node);
/** @brief Prints a logic node */
void
print_logic_node(size_t depth, const t_ast_node *node);

/** @brief A node for function definition */
struct s_function_node
{
	/** @brief Function's name */
	t_string_buffer	name;
	/** @brief The function's body */
	t_ast_node		*body;
	/** @brief Set to 1 when the function is registered. This prevent the
	 * function from being freed when the ast is freed. */
	int				registered;
};

/** @brief Creates a function node */
t_ast_node
*make_function_node(t_string_buffer name, t_ast_node *body);
/** @brief Frees a function node */
void
free_function_node(t_ast_node *node);
/** @brief Prints a function node */
void
print_function_node(size_t depth, const t_ast_node *node);

/** @brief If statement node */
struct s_if_node
{
	/** @brief Conditions, at most `nbodies` */
	t_ast_node	**conds;
	size_t		nconds;
	/** @brief Bodies, at most `nconds + 1` */
	t_ast_node	**bodies;
	size_t		nbodies;
};

/** @brief Creates a if node */
t_ast_node
*make_if_node(void);
/** @brief Frees a if node */
void
free_if_node(t_ast_node *node);
/** @brief Prints a if node */
void
print_if_node(size_t depth, const t_ast_node *node);

/** @brief While statement node */
struct s_while_node
{
	/** @brief While condition */
	t_ast_node	*cond;
	/** @brief While body */
	t_ast_node	*body;
};

/** @brief Creates a while node */
t_ast_node
*make_while_node(t_ast_node *cond, t_ast_node *body);
/** @brief Frees a while node */
void
free_while_node(t_ast_node *node);
/** @brief Prints a while node */
void
print_while_node(size_t depth, const t_ast_node *node);

/** @brief AST Node type */
typedef struct s_ast_node
{
	/** @brief Node type */
	enum e_node_type			type;
	/** @brief Node-specific data */
	union {
		/** @brief Block expression AST */
		struct s_node_block		block;
		/** @brief Sub expressions AST */
		struct s_subshell_node	sub;
		/** @brief Atom, for commands (name & parameters) */
		struct s_cmd_node		cmd;
		/** @brief Command list token */
		struct s_cmdlist_node	list;
		/** @brief Binary nodes, e.g `|` or `&&` */
		struct s_logic_node		logic;
		/** @brief Function definition node */
		struct s_function_node	function;
		/** @brief If node */
		struct s_if_node		st_if;
		/** @brief While node */
		struct s_while_node		st_while;
	};
}	t_ast_node;

/**
 * @brief Frees the AST
 *
 * @param node Node to free
 * @param cleanup Forces cleanup of functions
 */
void
ast_free(t_ast_node *node, int cleanup);
/** @brief Prints debug info for the AST */
void
ast_print(size_t depth, t_ast_node *node);

/******************************************************************************/
/* The parser                                                                 */
/******************************************************************************/

/** @brief The parser */
typedef struct s_parser
{
	t_string		input;
	t_token_list	list;
	char			**errors;
	size_t			errors_size;
	size_t			errors_cap;
	
	/** @brief Current position in the parser's input list */
	size_t			pos;
	/** @brief Determines if keywords have to be treated as words */
	int				allow_reserved;
}	t_parser;

/** @brief Initializes a new parser */
t_parser
parser_init(t_string input, t_token_list list);
/** @brief Frees the parser */
void
parser_free(t_parser *parser);
/**
 * @brief Parser entry point
 *
 * @param parser The parser
 * @param start Start position in token list
 * @param end End position in token list
 * @param min_prec Operator precedence for this parsing pass [0, 3]
 *
 * @return The parsed node, may be NULL (e.g empty expressions)
 */
t_ast_node
*parse(t_parser *parser, size_t start, size_t end, int min_prec);
/**
 * @brief Appends an error to the parser
 *
 * @param parser The parser
 * @param msg The error message (an owned string)
 * @param start The error's start range
 * @param end The error's end range
 */
void
parser_error(t_parser *parser, char *msg, size_t start, size_t end);
/**
 * @brief Flushes errors to stderr
 *
 * @param parser The parser
 * @return 1 if the parser has no errors, 0 if at least one error occured
 */
int
parser_error_flush(t_parser *parser);

/******************************************************************************/
/* Individual parsers                                                         */
/******************************************************************************/

/** @brief Command parser */
t_ast_node
*parse_cmd(t_parser *parser, size_t start, size_t end);

/** @brief Parses sub-shell expression: `(...)` */
t_ast_node
*parse_subshell(t_parser *parser, size_t start, size_t end);

/******************************************************************************/
/* Individual parsers                                                         */
/******************************************************************************/

/**
 * @brief Utility function to get the next logic (binary) operator in the list
 *
 * @param parser The parser
 * @param start Start position in the list
 * @param end End position in the list
 * @param min_prec A value indicating the minimum required precedence of the
 * operator to parse
 *
 * @returns The position of the operator if found `(size_t)-1` if not found
 */
size_t
parser_next_operator(
	t_parser *parser,
	size_t start,
	size_t end,
	int min_prec);

/******************************************************************************/
/* Parsing helpers                                                            */
/******************************************************************************/

/**
 * @brief Checks if the current token is a control token and it's content
 * matches against `word`
 *
 * @param parser The parser
 * @param offset Offset from current position
 * @param word Word to match against
 *
 * @returns 1 If the condition is fulfilled.
 */
int
accept(t_parser *parser, int offset, const char *word);
/**
 * @brief Checks if the current token can be treated as a word
 *
 * @param parser The parser
 * @param offset Offset from current position
 *
 * @returns 1 If the token can be treated as a plain word.
 */
int
accept_word(t_parser *parser, int offset);
/**
 * @brief Checks if the current token is a control token and it's content
 * matches against `word`. If it doesn't match an error is reported
 *
 * @param parser The parser
 * @param offset Offset from current position
 * @param word Word to match against
 *
 * @returns 1 If the condition is fulfilled. 0 otherwise and an error is
 * reported
 */
int
expect(t_parser *parser, int offset, const char *word);
/**
 * @brief Utility that parses a token to an integer
 *
 * If the token is not a TOK_DIGIT, or the integer overflows, an error is
 * reported.
 *
 * @param parser The parser
 * @param pos Position of the token in `parser`
 * @param value Stores the result on success
 *
 * @returns 1 on success, 0 if an error has been reported
 */
int
token_atoi(t_parser *parser, size_t pos, int *value);

/**
 * @brief Prints `pad` `n` times
 *
 * @param pad Padding string
 * @param n Number of times to print `pad`
 */
void
print_pad(const char *pad, size_t n);

#endif // PARSER_H
