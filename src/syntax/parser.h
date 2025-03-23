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

# include "tokenizer.h"
#include <stddef.h>
#include <string.h>

/* ************************************************************************** */
/* The AST                                                                    */
/* ************************************************************************** */

typedef struct s_ast_node	t_ast_node;

/** @brief Type for nodes */
enum e_node_type
{
	/** @brief Token stream node, not present in the final AST */
	NODE_TOKENSTREAM,
	/** @brief Command (atom) */
	NODE_COMMAND,
	/** @brief Binary logic operator, e.g `||`, `|&`, `>` */
	NODE_LOGIC,
	/** @brief Unary operator (unhandled currently) e.g: `&`, `!` */
	NODE_UNARY,
};

/**
 * @brief Redirection data, can be part of all nodes, all files need to be
 * created before executing the nodes, only the last redirection is actually
 * used, other are left as empty files. `clobbering` may apply,
 * see: 3.6.2 Redirecting Output
 */
typedef struct s_redir_data
{
	/** @brief Associated redirection token */
	t_token				token;
	/** @brief FD to redirect from/to (defaults to 1) */
	int					fd;
	/** @brief FD to redirect from/to (defaults to 2) */
	int					fd2;
	/** @brief File to redirect from/to */
	t_string_buffer		word;
	/**
	 * @brief Moves the content of \1 to \2, then close \2,
	 * see 3.6.9 Moving File Descriptors.
	 *
	 * This can only be used in combination
	 * with `fd2`
	 */
	int	move;
}	t_redir_data;

/** @brief Tokenstream node */
struct s_node_stream
{
	t_token_list	list;
	size_t			start;
	size_t			end;
};

/** @brief Command name and arguments */
struct s_node_cmd
{
	/** @brief Program arguments */
	t_string_buffer	*args;
	/** @brief Number of arguments */
	size_t			nargs;
	/** @brief Redirections */
	t_redir_data	*redirs;
	/** @brief Number of redirections */
	size_t			redirs_size;
	/** @brief Redirections capacity */
	size_t			redirs_capacity;
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
		struct s_node_stream	stream;
		struct s_node_cmd		cmd;
		struct s_logic_node		logic;
	};
}	t_ast_node;

/** @brief Frees the AST */
void
ast_free(t_ast_node *head);

/** @brief Prints debug info for the AST */
void
ast_print_debug(
	t_string input,
	t_token_list stream,
	t_ast_node *head,
	size_t depth);

/* ************************************************************************** */
/* The parser                                                                 */
/* ************************************************************************** */

/** @brief The parser */
typedef struct s_parser
{
	t_string_buffer	*errors;
	size_t			errors_size;
	size_t			errors_cap;
}	t_parser;

/** @brief Initializes a new parser */
t_parser
parser_init();

/** @brief Appends error to parser */
void
parser_error(t_parser *parser, t_string_buffer msg);

/** @brief Frees the parser */
void
parser_free(t_parser *parser);

/* ************************************************************************** */
/* Individual parsers                                                         */
/* ************************************************************************** */

/**
 * @brief Utility function to get the next logic (binary) operator in the list
 */
size_t
parser_next_operator(t_parser *parser, size_t start, size_t end);

/** @brief Command parser */
t_ast_node
*parse_cmd(t_parser *parser, size_t start, size_t end);

/**
 * @brief Parses next word in input
 *
 * @param parser Token source
 * @param start token
 * @param end end token
 * @param buf Output buffer
 *
 * @returns The number of consumed token, 0 if no tokens were consumed
 */
size_t
parse_word(
	t_parser *parser,
	size_t start,
	size_t end,
	t_string_buffer *buf);

/**
 * @brief Parses redirections into command
 *
 * @param parser Token sourcer
 * @param start Start token
 * @param end End token
 * @param cmd Command to parse into
 *
 * @returns The number of consumed token, 0 if no tokens were consumed
 */
size_t
parse_redir(
	t_parser *parser,
	size_t start,
	size_t end,
	struct s_node_cmd *cmd);

/**
 * @brief Parses redirections repeatedly, into commands
 *
 * @param parser Token sourcer
 * @param start Start token
 * @param end End token
 * @param cmd Command to parse into
 *
 * @returns The number of consumed token, 0 if no tokens were consumed
 */
size_t
parse_redir_repeat(
	t_parser *parser,
	size_t start,
	size_t end,
	struct s_node_cmd *cmd);

#endif // PARSER_H
