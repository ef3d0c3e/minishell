/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <util/util.h>

// TEMP
#include <stdio.h>

/* ************************************************************************** */
/* Token definition                                                           */
/* ************************************************************************** */

/**
 * @brief Token types definitions
 */
enum e_token_type
{
	TOK_SPACE,
	TOK_DIGIT,
	/** @brief Newline token: `\n` */
	TOK_NEWLINE,

	/** @brief Grouping character, one of `{, (, }, )` */
	TOK_GROUPING,

	/**
	 * @brief Pipeline token, one of `|`, `|&`
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Pipelines
	 */
	TOK_PIPELINE,

	/**
	 * @brief Sequence separator tokens
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Simple-Commands
	 */
	TOK_SEQUENCE,

	/**
	 * @brief Redirection tokens, e.g `<` `>`
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Redirections
	 */
	TOK_REDIR,

	TOK_COMMENT,
	TOK_HEREDOC,
	TOK_HERESTRING,

	/** @brief End of input */
	TOK_EOF,
	/** @brief Error message */
	TOK_ERROR,
	/** @brief Error message that needs to be `free`d */
	TOK_ERROR_CUSTOM,
};

/** @brief String data for tokens */
struct s_token_string
{
	t_string_buffer	string;
};

struct s_token_redir
{
	/** @brief Input flag */
	int	input;
	/** @brief Output flag */
	int	output;
	/** @brief Output even if the file already exists and the `noclobber`
	 * option is set:
	 * https://www.gnu.org/software/bash/manual/bash.html#Redirecting-Output */
	int	bypass_noclobber;
	/** @brief Append flag for output */
	int	append_flag;
};

typedef struct s_token
{
	/** @brief Token type */
	enum e_token_type	type;
	/** @brief Token start byte position in the original input */
	size_t				start;
	/** @brief Token end byte position in the original input */
	size_t				end;

	union {
		/** @brief Digit token */
		int						digit;
		/** @brief Redirection token data */
		struct s_token_redir	redir;
		/** @brief Reserved name for token, e.g keyword name, grouping char */
		const char				*reserved_word;
		/** @brief Error message */
		t_string				err;
	};
}	t_token;

/** @brief Displays token */
void
token_print_debug(int fd, t_string prompt, const t_token *token);

/**
 * @brief Frees a token
 *
 * Most token do not own thir data, but strings/words do
 *
 * @param token Token to free
 */
void
token_free(t_token *token);

typedef struct s_token_list
{
	t_token	*tokens;
	size_t	size;
	size_t	capacity;
}	t_token_list;

/**
 * @brief Push `token` to the end of `list`
 *
 * @param list List to push `token` to
 * @param token Token to push to `list`
 */
void
token_list_push(t_token_list *list, t_token token);

/**
 * @brief Frees the token list (and the contained tokens)
 *
 * @param list Token list to free
 */
void
token_list_free(t_token_list *list);

/* ************************************************************************** */
/* Tokenizer                                                                  */
/* ************************************************************************** */

/**
 * @brief Adds an error to the token list
 *
 * @param list List to add an error to
 * @param start Error start range
 * @param end Error end range
 * @param msg Error message
 */
void
token_error(t_token_list *list, size_t start, size_t end, const char *msg);

typedef int	(*t_tokenizer_fn)(t_token_list *, t_u8_iterator *it);

/**
 * @brief Tokenizer
 */
typedef struct s_tokenizer
{
	t_tokenizer_fn	*munchers;

}	t_tokenizer;

/**
 * @brief Initializes a tokenizer
 *
 * @param t Tokenizer to initialize
 */
void
tokenizer_init(t_tokenizer *t);

/**
 * @brief Frees a tokenizer
 *
 * @param t Tokenizer to free
 */
void
tokenizer_free(t_tokenizer *t);

/**
 * @brief Tokenize a prompt
 *
 * @param t Tokenizer to use for tokenization
 * @param prompt Prompt to tokenize
 */
t_token_list
tokenizer_tokenize(const t_tokenizer *t, t_string prompt);

#endif // TOKENIZER_H
