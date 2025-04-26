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
};

/** @brief A single argument item */
struct s_arg_item
{
	/** @brief Type of arugment */
	enum e_arg_type	type;
	/** @brief Text data */
	t_string_buffer	data;
};

/** @brief Data for command arguments */
struct s_argument
{
	/** @brief Items in this argument */
	struct s_arg_item	*items;
	/** @brief Number of items in this argument */
	size_t				nitems;
};

/** @brief Pushes a token to an argument */
void
arg_push(struct s_argument *arg, const t_token *token);
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
	/** @brief Expression that should run in a sub-shell */
	NODE_SUBSHELL,
	/** @brief Command */
	NODE_COMMAND,
	/** @brief Binary logic operator, e.g `||`, `|&`, `>` */
	NODE_LOGIC,
};

/** @brief Sub expression data */
struct s_node_subshell
{
	/** @brief Expression substring */
	//t_string		input;
	/** @brief Subshell inner */
	t_ast_node		*head;
	/** @brief Redirections */
	t_redirections	redirs;
};

/** @brief Command name and arguments */
struct s_node_cmd
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

/** @brief Data for logic (binary) nodes */
struct s_logic_node
{
	t_token		token;
	t_ast_node	*left;
	t_ast_node	*right;
};

/** @brief AST Node type */
typedef struct s_ast_node
{
	/** @brief Node type */
	enum e_node_type			type;
	/** @brief Node-specific data */
	union {
		t_string_buffer			atom;
		/** @brief Sub expressions AST */
		struct s_node_subshell	expr;
		/** @brief Atom, for commands (name & parameters) */
		struct s_node_cmd		cmd;
		/** @brief Binary nodes, e.g `|` or `&&` */
		struct s_logic_node		logic;
		/** @brief Compound node */
		struct s_argument	compound;
	};
}	t_ast_node;

/** @brief Frees the AST */
void
ast_free(t_ast_node *head);

/** @brief Prints debug info for the AST */
void
ast_print_debug(
	t_string input,
	t_ast_node *head,
	size_t depth);

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

#endif // PARSER_H
