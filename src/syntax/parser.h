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

/* ************************************************************************** */
/* The AST                                                                    */
/* ************************************************************************** */

/** @brief Type for nodes */
enum e_node_type
{
	/** @brief Command */
	NODE_COMMAND,
	/** @brief Binary logic operator, e.g `||`, `|&`, `>` */
	NODE_LOGIC,
	/** @brief Unary operator (unhandled currently) e.g: `&`, `!` */
	NODE_UNARY,
};

typedef struct s_ast_node	t_ast_node;

/** @brief Command name and arguments */
struct s_node_cmd
{
	/** @brief Program arguments */
	t_string_buffer	*args;
	/** @brief Number of arguments */
	size_t			nargs;
	/**
	 * @brief Redirections, only the last redirection is actually populated,
	 * other files are populated according to clobbering rules
	 */
	t_token_list	redirs;
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
	enum e_node_type		type;
	union {
		struct s_node_cmd	cmd;
		struct s_logic_node	logic;
	};
}	t_ast_node;

/** @brief Frees the AST */
void
ast_free(t_ast_node *head);

/** @brief Prints debug info for the AST */
void
ast_print_debug(t_string input, t_ast_node *head, size_t depth);

/* ************************************************************************** */
/* The parser                                                                 */
/* ************************************************************************** */

/** @brief The parser */
typedef struct s_parser
{
	t_token_list	list;
	t_string_buffer	*errors;
	size_t			errors_size;
	size_t			errors_cap;
}	t_parser;

/** @brief Initializes a new parser from the given token list */
t_parser
parser_init(t_token_list list);

/** @brief Appends error to parser */
void
parser_error(t_parser *parser, t_string_buffer msg);

/** @brief Frees the parser */
void
parser_free(t_parser *parser);

/**
 * @brief Utility function to get the next logic (binary) operator in the list
 */
size_t
parser_next_operator(t_parser *parser, size_t start, size_t end);

/** @brief Command parser */
t_ast_node	*parse_cmd(t_parser *parser, size_t start, size_t end);

#endif // PARSER_H
