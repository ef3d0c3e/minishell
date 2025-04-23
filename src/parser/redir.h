/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REDIR_H
# define REDIR_H

# include <tokenizer/tokenizer.h>

typedef struct s_parser	t_parser;

/** @brief The types of redirections */
enum e_redir_type
{
	R_OUTPUT_DIRECTION,
	R_INPUT_DIRECTION,
	R_INPUTA_DIRECTION,
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
	int				fd;
	/** @brief Redirect to/from file */
	t_string_buffer	filename;
}	t_redirectee;

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
	/** @brief Delimiter word for heredoc */
	char				*here_doc_eof;
}	t_redirection;

///**
// * @brief Redirection data, can be part of all nodes, all files need to be
// * created before executing the nodes, only the last redirection is actually
// * used, other are left as empty files. `clobbering` may apply,
// * see: 3.6.2 Redirecting Output
// */
//typedef struct s_redir_data
//{
//	t_redirectee	from;
//	t_redirectee	to;
//}	t_redir_data;
//
/** @brief Stores redirections */
typedef struct s_redirections
{
	/** @brief Redirections */
	t_redirection	*redirs;
	/** @brief Number of redirections */
	size_t			redirs_size;
	/** @brief Redirections capacity */
	size_t			redirs_capacity;
}	t_redirections;

/** @brief Pattern types for matching redirection tokens */
typedef struct s_redir_tok_type
{
	const char			*tok;
	enum e_redir_type	type;
}	t_redir_tok_type;

void
print_redir(
	const t_redirections *redirs,
	size_t depth);

/**
 * @brief Parses redirections in the following formats:
 *
 * (Extract from bash Yacc grammar)
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
 * 
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
 *
 * @param parser The parser
 * @param start Redirection start position
 * @param redirs The redirections to store to
 *
 * @returns The number of skipped tokens
*/
size_t
	redir_parse(
	t_parser *parser,
	size_t start,
	t_redirections *redirs);

#endif // REDIR_H
