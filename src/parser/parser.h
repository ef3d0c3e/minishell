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

#include "util/util.h"
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
	/** @brief Reference to the next argument, NULL for none */
	struct s_arg_item		*next;
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
 */
void
parse_param(t_parser *parser, struct s_arg_item *arg);

/** @brief Pushes a token to an argument */
void
arg_push(t_parser *parser, struct s_argument *arg);
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
/**
 * @brief Pushes the current token to the argument list
 *
 * @param parser The parser
 * @param list Pointer to the array of arguments
 * @param len Pointer to the array's length
 */
void
arglist_push(t_parser *parser, struct s_argument **list, size_t *len);

/******************************************************************************/
/* Assignment nodes                                                           */
/******************************************************************************/

/** @brief A variable assignment */
struct s_assignment
{
	/** @brief Name of the variable being assigned to */
	t_string_buffer		variable;
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
	/** @brief Binary logic operators, e.g `||`, `&&` */
	NODE_BINARY,
	/** @brief Pipe logic operators: `|`, `|&` */
	NODE_PIPE,
	/** @brief Function definition node */
	NODE_FUNCTION,
	/** @brief If statement node */
	NODE_IF,
	/** @brief While or until loop statement node */
	NODE_LOOP,
	/** @brief For statement node */
	NODE_FOR,
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

/** @brief Loop statement node */
struct s_loop_node
{
	/** @brief Set to 1 for until node, 0 for while */
	int			until;
	/** @brief Loop condition */
	t_ast_node	*cond;
	/** @brief Loop body */
	t_ast_node	*body;
};

/** @brief Creates a loop node */
t_ast_node
*make_loop_node(t_ast_node *cond, t_ast_node *body, int until);
/** @brief Frees a loop node */
void
free_loop_node(t_ast_node *node);
/** @brief Prints a loop node */
void
print_loop_node(size_t depth, const t_ast_node *node);

/** @brief While statement node */
struct s_for_node
{
	/** @brief Loop identifier, must be valid */
	char				*ident;
	/** @brief Word list (must undergo expansion) */
	struct s_argument	*args;
	/** @brief Number of words */
	size_t				nargs;
	/** @brief For body */
	t_ast_node			*body;
};

/** @brief Creates a for node */
t_ast_node
*make_for_node(char *ident);
/** @brief Frees a for node */
void
free_for_node(t_ast_node *node);
/** @brief Prints a for node */
void
print_for_node(size_t depth, const t_ast_node *node);

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
		/** @brief Loop (until or while) node */
		struct s_loop_node		st_loop;
		/** @brief For node */
		struct s_for_node		st_for;
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
 *
 * @return The parsed node, may be NULL (e.g empty expressions)
 */
t_ast_node
*parse(t_parser *parser);
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

/**
 * @brief Parses a list of commands:
 *
 * <list_of_commands> ::= <and_or> ( (';' | '&' | NEWLINE) <list_of_commands> )?
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_cmdlist(t_parser *parser);
/**
 * @brief Parses a compound command clause:
 *
 * <compound_command> ::= '(' <list_of_commands> ')'
 *                        | '{' <list_of_commands> '}'
 *                        | <if_clause>
 *                        | <while_clause>
 *                        | <until_clause>
 *                        | <for_clause>
 *                        | <case_clause>
 *
 * @param parser The parser
 * @return The parsed node
 */
t_ast_node
*parse_compound_command(t_parser *parser);
/**
 * @brief Parses a command clause:
 *
 * <command> ::= <simple_command>
 *               | <compound_command>
 *               | <function_def>
 *
 * @param parser The parser
 * @return The parsed node
 */
t_ast_node
*parse_command(t_parser *parser);
/**
 * @brief Parses a simple command clause:
 *
 * <simple_command> ::= cmd_prefix cmd_word?
 * <cmd_prefix>     ::= io_redirect
 *                      | cmd_prefix io_redirect
 *                      | ASSIGNMENT_WORD
 *                      | cmd_prefix ASSIGNMENT_WORD
 *
 * @param parser The parser
 * @return The parsed node
 */
t_ast_node
*parse_simple_command(t_parser *parser);
/**
 * @brief Parses an and or or cllaus:
 *
 * <and_or> ::= <pipeline> ( ('&&' | '||') <pipeline> )*
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_and_or(t_parser *parser);
/**
 * @brief Parses a pipeline cllaus:
 *
 * <pipeline> ::= <command> ( ('|' | '|&') <command> )*
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_pipeline(t_parser *parser);
/**
 * @brief Parses a function definition clause:
 *
 * <function_def> ::= WORD '()' <compound_command>
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_function_def(t_parser *parser);
/**
 * @brief Parses an if clause:
 *
 * <if_clause> ::= 'if' <list_of_commands> 'then' <list_of_commands>
 *                 ('elif' <list_of_commands> 'then' <list_of_commands>)*
 *                 ['else' <list_of_commands>]
 *                 'fi'
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_if(t_parser *parser);
/**
 * @brief Parses a while clause:
 *
 * <while_clause> ::= 'while' <list_of_commands> 'do' <list_of_commands> 'done'
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_while(t_parser *parser);
/**
 * @brief Parses an until clause:
 *
 * <until_clause> ::= 'until' <list_of_commands> 'do' <list_of_commands> 'done'
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_until(t_parser *parser);
/**
 * @brief Parses a for clause:
 *
 * <for_clause> ::= 'for' WORD 'in' WORD* 'do' <list_of_commands> 'done'
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_for(t_parser *parser);


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
 * @brief Checks if the current token is a token of type `type`
 *
 * @param parser The parser
 * @param offset Offset from current position
 * @param type The type to check for
 *
 * @returns 1 If the condition is fulfilled. 0 otherwise
 */
int
accept_tok(t_parser *parser, int offset, enum e_token_type type);
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
