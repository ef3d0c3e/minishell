/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REDIR_PARSER_H
# define REDIR_PARSER_H

#include "util/util.h"
# include <tokenizer/tokenizer.h>
# include <parser/words/words.h>

typedef struct s_parser	t_parser;

/** @brief Maximum number of heredocuments to parse */
#define HEREDOC_MAX 16

/******************************************************************************/
/* Redirection definitions and utilities                                      */
/******************************************************************************/

/** @brief The types of redirections */
enum e_redir_type
{
	R_OUTPUT_DIRECTION,
	R_INPUT_DIRECTION,
	R_APPENDING_TO,
	R_READING_UNTIL,
	R_READING_STRING,
	R_DUPLICATING_INPUT,
	R_DUPLICATING_OUTPUT,
	R_DEBLANK_READING_UNTIL,
	R_CLOSE_THIS,
	R_ERR_AND_OUT,
	R_INPUT_OUTPUT,
	R_OUTPUT_FORCE,
	R_DUPLICATING_INPUT_WORD,
	R_DUPLICATING_OUTPUT_WORD,
	R_MOVE_INPUT,
	R_MOVE_OUTPUT,
	R_MOVE_INPUT_WORD,
	R_MOVE_OUTPUT_WORD,
	R_APPEND_ERR_AND_OUT,
};

/** @brief Redirect from/to this type */
typedef union u_redirectee
{
	/** @brief Redirect to/from file descriptor */
	int					fd;
	/** @brief Redirect to/from file */
	struct s_word	filename;
}	t_redirectee;

/** @brief Stores a redirection */
typedef struct s_redirection
{
	/** @brief Redirect from */
	t_redirectee		redirector;
	/** @brief Flags for `open()` */
	int					flags;			
	/** @brief Redirection type */
	enum e_redir_type	type;
	/** @brief Redirect to */
	t_redirectee		redirectee;
	/** @brief Heredoc content */
	t_string_buffer		heredoc;
}	t_redirection;

/**
 * @brief Checks if the redirection destination is a WORD
 *
 * @param redir The redirection
 * @return 1 If the redirection destination is a WORD, 0 if it is a file
 * descriptor.
 */
int
redir_dest_word(const t_redirection *redir);
/**
 * @brief Checks if there's a minus token after the current word
 *
 * @param parser The parser
 * @param offset Start offset
 *
 * @returns 1 if there's a `-` at the end of the current word
 */
int
redir_has_minus(t_parser *parser, int offset);

/** @brief Stores multiple redirections */
typedef struct s_redirections
{
	/** @brief Redirections */
	t_redirection	*redirs;
	/** @brief Number of redirections */
	size_t			redirs_size;
	/** @brief Redirections capacity */
	size_t			redirs_capacity;
}	t_redirections;

/**
 * @brief Frees an array of redirections
 *
 * @param redirs The array to free
 */
void
redirs_free(t_redirections *redirs);

/**
 * @brief Prints a list of redirections to stderr
 *
 * @param redirs List of redirections to print
 * @param depth Display depth
 */
void
print_redir(
	const t_redirections *redirs,
	size_t depth);
/**
 * @brief Pushes a here document to the HEREDOC stack
 *
 * @param parser Parser to push to
 * @param redir Redirection to push
 */
void
push_heredoc(t_parser *parser, t_redirection *redir);
/**
 * @brief Adds a redirection to `redirs`
 *
 * @param redirs Redirection list to append to
 * @param source Redirection source (fd)
 * @param desc Redirection destination (fd or filename)
 * @param type Type of redirection
 */
void
make_redirection(
	t_redirections *redirs,
	t_redirectee source,
	t_redirectee dest,
	enum e_redir_type type);

/** @brief Pattern types for matching redirection tokens */
typedef struct s_redir_tok_type
{
	/** @brief The token to match */
	const char			*tok;
	/** @brief The type of redirection */
	enum e_redir_type	type;
}	t_redir_tok_type;

/**
 * @brief Matches from an array `t_redir_tok_type`
 *
 * @param types The redirection types to match from
 * @param len Number of `types` to match from
 * @param token The word to match against
 *
 * @returns The matched redirection type from `types`, or NULL if not found
 */
const t_redir_tok_type
*redir_alternatives(
	const t_redir_tok_type *types,
	size_t len,
	const char *token);

/******************************************************************************/
/* Redirection parsing                                                        */
/******************************************************************************/

/**
 * @brief Parses redirections of the form `[REDIR][WORD|NUM|-]`,
 * using the following grammar:
 * (Extract from bash Yacc grammar)
 * @code{.y}
 * // [REDIR][N]
 * |	LESS_AND NUMBER
 * {
 * 	source.dest = 0;
 * 	redir.dest = $2;
 * 	$$ = make_redirection (source, r_duplicating_input, redir, 0);
 * }
 * |	GREATER_AND NUMBER
 * {
 * 	source.dest = 1;
 * 	redir.dest = $2;
 * 	$$ = make_redirection (source, r_duplicating_output, redir, 0);
 * }
 * // [REDIR][-]
 * |	GREATER_AND '-'
 * {
 * 	source.dest = 1;
 * 	redir.dest = 0;
 * 	$$ = make_redirection (source, r_close_this, redir, 0);
 * }
 * |	LESS_AND '-'
 * {
 * 	source.dest = 0;
 * 	redir.dest = 0;
 * 	$$ = make_redirection (source, r_close_this, redir, 0);
 * }
 * // [REDIR OUT][WORD]
 * |	'>' WORD
 * {
 * 	source.dest = 1;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_output_direction, redir, 0);
 * }
 * |	GREATER_GREATER WORD
 * {
 * 	source.dest = 1;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_appending_to, redir, 0);
 * }
 * |	GREATER_BAR WORD
 * {
 * 	source.dest = 1;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_output_force, redir, 0);
 * }
 * |	GREATER_AND WORD
 * {
 * 	source.dest = 1;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_duplicating_output_word, redir, 0);
 * }
 * |	AND_GREATER WORD
 * {
 * 	source.dest = 1;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_err_and_out, redir, 0);
 * }
 * |	AND_GREATER_GREATER WORD
 * {
 * 	source.dest = 1;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_append_err_and_out, redir, 0);
 * }
 * ;
 * // [REDIR IN][WORD]
 * |	'<' WORD
 * {
 * 	source.dest = 0;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_input_direction, redir, 0);
 * }
 * |	LESS_GREATER WORD
 * {
 * 	source.dest = 0;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_input_output, redir, 0);
 * }
 * |	LESS_LESS WORD
 * {
 * 	source.dest = 0;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_reading_until, redir, 0);
 * 	push_heredoc ($$);
 * }
 * |	LESS_LESS_MINUS WORD
 * {
 * 	source.dest = 0;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_deblank_reading_until, redir, 0);
 * 	push_heredoc ($$);
 * }
 * |	LESS_LESS_LESS WORD
 * {
 * 	source.dest = 0;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_reading_string, redir, 0);
 * }
 * |	LESS_AND WORD
 * {
 * 	source.dest = 0;
 * 	redir.filename = $2;
 * 	$$ = make_redirection (source, r_duplicating_input_word, redir, 0);
 * }
 * @endcode
 *
 * @param parser The parser
 * @param redirs The redirections to store to
 *
 * @returns 1 On success, 0 on failure
*/
int
redir_parser2(
	t_parser *parser,
	t_redirections *redirs);
/**
 * @brief Parses redirections of the form `[NUM][REDIR][WORD|NUM|-]`,
 * using the following grammar:
 * (Extract from bash Yacc grammar)
 * @code{.y}
 * // [NUM][REDIR][NUM]
 * |	NUMBER LESS_AND NUMBER
 * {
 * 	source.dest = $1;
 * 	redir.dest = $3;
 * 	$$ = make_redirection (source, r_duplicating_input, redir, 0);
 * }
 * |	NUMBER GREATER_AND NUMBER
 * {
 * 	source.dest = $1;
 * 	redir.dest = $3;
 * 	$$ = make_redirection (source, r_duplicating_output, redir, 0);
 * }
 * // [NUM][REDIR][-]
 * |	NUMBER GREATER_AND '-'
 * {
 * 	source.dest = $1;
 * 	redir.dest = 0;
 * 	$$ = make_redirection (source, r_close_this, redir, 0);
 * }
 * |	NUMBER LESS_AND '-'
 * {
 * 	source.dest = $1;
 * 	redir.dest = 0;
 * 	$$ = make_redirection (source, r_close_this, redir, 0);
 * }
 * // [NUM][REDIR][WORD]
 * |	NUMBER '>' WORD
 * {
 * 	source.dest = $1;
 * 	redir.filename = $3;
 * 	$$ = make_redirection (source, r_output_direction, redir, 0);
 * }
 * |	NUMBER '<' WORD
 * {
 * 	source.dest = $1;
 * 	redir.filename = $3;
 * 	$$ = make_redirection (source, r_input_direction, redir, 0);
 * }
 * |	NUMBER GREATER_GREATER WORD
 * {
 * 	source.dest = $1;
 * 	redir.filename = $3;
 * 	$$ = make_redirection (source, r_appending_to, redir, 0);
 * }
 * |	NUMBER GREATER_BAR WORD
 * {
 * 	source.dest = $1;
 * 	redir.filename = $3;
 * 	$$ = make_redirection (source, r_output_force, redir, 0);
 * }
 * |	NUMBER LESS_GREATER WORD
 * {
 * 	source.dest = $1;
 * 	redir.filename = $3;
 * 	$$ = make_redirection (source, r_input_output, redir, 0);
 * }
 * |	NUMBER LESS_LESS WORD
 * {
 * 	source.dest = $1;
 * 	redir.filename = $3;
 * 	$$ = make_redirection (source, r_reading_until, redir, 0);
 * 	push_heredoc ($$);
 * }
 * |	NUMBER LESS_LESS_MINUS WORD
 * {
 * 	source.dest = $1;
 * 	redir.filename = $3;
 * 	$$ = make_redirection (source, r_deblank_reading_until, redir, 0);
 * 	push_heredoc ($$);
 * }
 * |	NUMBER LESS_LESS_LESS WORD
 * {
 * 	source.dest = $1;
 * 	redir.filename = $3;
 * 	$$ = make_redirection (source, r_reading_string, redir, 0);
 * }
 * |	NUMBER LESS_AND WORD
 * {
 * 	source.dest = $1;
 * 	redir.filename = $3;
 * 	$$ = make_redirection (source, r_duplicating_input_word, redir, 0);
 * }
 * |	NUMBER GREATER_AND WORD
 * {
 * 	source.dest = $1;
 * 	redir.filename = $3;
 * 	$$ = make_redirection (source, r_duplicating_output_word, redir, 0);
 * }
 * @endcode
 *
 * @param parser The parser
 * @param redirs The redirections to store to
 *
 * @returns 1 On success, 0 on failure
*/
int
redir_parser3(
	t_parser *parser,
	t_redirections *redirs);
/**
 * @brief Parses redirections of the form `[REDIR][NUM|WORD][-]`,
 *
 * @param parser The parser
 * @param redirs The redirections to store to
 *
 * @returns 1 On success, 0 on failure
 */
int
redir_parser3_move(
	t_parser *parser,
	t_redirections *redirs);
/**
 * @brief Parses redirections of the form `[NUM][REDIR][NUM|WORD][-]`,
 *
 * @param parser The parser
 * @param redirs The redirections to store to
 *
 * @returns 1 On success, 0 on failure
 */
int
redir_parser4(
	t_parser *parser,
	t_redirections *redirs);

/**
 * @brief Parses redirection into `redirs`
 *
 * The following forms of redirections are recognized:
 *  - [REDIR][WORD|NUM|-]
 *  - [REDIR][WORD|NUM][-]
 *  - [NUM][REDIR][WORD|NUM|-]
 *  - [NUM][REDIR][WORD|NUM][-]
 *
 * @param parser Token sourcer
 * @param start Start token
 * @param end End token
 * @param cmd Command to parse into
 *
 * @returns 1 If a redirection was successfully parsed, 0 otherwise
 */
int
parse_redir(
	t_parser *parser,
	t_redirections *redirs);
/**
 * @brief Parses redirections repeatedly into `redirs`
 *
 * @param parser Token sourcer
 * @param start Start token
 * @param end End token
 * @param cmd Command to parse into
 */
void
parse_redir_repeat(
	t_parser *parser,
	t_redirections *redirs);

#endif // REDIR_PARSER_H
