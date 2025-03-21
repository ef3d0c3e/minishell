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

typedef struct s_token	t_token;

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

/**
 * @brief Token types definitions
 */
enum e_token_type
{
	/** @brief Whitespace (non newline) character */
	TOK_SPACE,
	/** @brief Newline token: `\n` */
	TOK_NEWLINE,

	/** @brief A positive number between [0, INT_MAX] */
	TOK_DIGIT,

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
	 * @brief Keyword token
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Reserved-Words
	 */
	TOK_KEYWORD,

	/**
	 * @brief Command substitution token: `$(cmd)`
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Process-Substitution
	 */
	TOK_CMD_SUB,

	/**
	 * @brief Arithmetic token: `$((cmd))`
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Arithmetic-Expansion
	 */
	TOK_ARITH,

	/**
	 * @brief Variable expansion token: `$VAR`
	 *
	 * Var is matched from `[a-zA-Z0-9_]`
	 */
	TOK_PARAM_SIMPLE,

	/**
	 * @brief Variable expansion token: `${VAR}`
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Shell-Parameter-
	 * Expansion
	 */
	TOK_PARAM,

	/**
	 * @brief Single quoted text
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Single-Quotes
	 */
	TOK_SINGLE_QUOTE,
	/**
	 * @brief Double quoted text
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Double-Quotes
	 */
	TOK_DOUBLE_QUOTE,

	/** @brief Leftover content from tokenization */
	TOK_WORD,

	/**
	 * @brief Redirection tokens, e.g `<` `>`
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Redirections
	 */
	TOK_REDIR,

	/**  @brief Special character: `-` */
	TOK_SPECIAL,

	TOK_COMMENT,
	TOK_HEREDOC,
	TOK_HERESTRING,

	/** @brief End of input */
	TOK_EOF,
	/** @brief Error message */
	TOK_ERROR,
};

/** @brief String data for tokens */
struct s_token_string
{
	t_string_buffer	string;
};

/** @brief Redirection type */
enum e_redir_type
{
	/** @brief "<" */
	REDIR_INPUT,
	/** @brief ">" */
	REDIR_OUTPUT,
	/** @brief ">>" */
	REDIR_APPEND,
	/** @brief "&>", "&>>" */
	REDIR_OUTPUT_AND_ERR,
	/** @brief "<<", "<<-" */
	REDIR_HEREDOC,
	/** @brief "<<<" */
	REDIR_HERESTRING,
	/** @brief "<&" */
	REDIR_DUP_IN,
	/** @brief ">&" */
	REDIR_DUP_OUT,
	/** @brief "<>" */
	REDIR_READWRITE,
};

struct s_token_redir
{
	enum e_redir_type	type;
	/** @brief Output even if the file already exists and the `noclobber`
	 * option is set, see 3.6.2 Redirecting Output */
	int	clobber;
	/** @brief Moves the content of \1 to \2, then close \2,
	 * see 3.6.9 Moving File Descriptors */
	int	move;
	/** @brief Duplicates the content of \1,
	 * see 3.6.8 Duplicating File Descriptors */
	int	duplicate;
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
		/** @brief String content */
		t_string_buffer			word;
		/** @brief Error message */
		t_string				err;
		/** @brief Expansion result */
		t_token_list			expansion;
	};
}	t_token;

/** @brief Displays token */
void
token_print_debug(int fd, t_string prompt, const t_token *token);

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

typedef struct s_tokenizer t_tokenizer;

typedef int	(*t_tokenizer_fn)(t_token_list *list, t_u8_iterator *it);

/**
 * @brief Tokenizer
 */
typedef struct s_tokenizer
{
	const t_tokenizer_fn	*munchers;
}	t_tokenizer;

/**
 * @brief Initializes a tokenizer
 *
 * @param t Tokenizer to initialize
 */
void
tokenizer_init(t_tokenizer *t);

/**
 * @brief Tokenize a prompt
 *
 * @param t Tokenizer to use for tokenization
 * @param prompt Prompt to tokenize
 */
t_token_list
tokenizer_tokenize(const t_tokenizer *t, t_string prompt);

/* ************************************************************************** */
/* Tokenizer                                                                  */
/* ************************************************************************** */

/** @brief Finds the next unescaped instance of `token` in `input` */
size_t
find_unsecaped(t_string input, const char *token);

int	token_space(t_token_list *list, t_u8_iterator *it);
int	token_newline(t_token_list *list, t_u8_iterator *it);
int	token_redir(t_token_list *list, t_u8_iterator *it);
int	token_digit(t_token_list *list, t_u8_iterator *it);
int	token_grouping(t_token_list *list, t_u8_iterator *it);
int	token_sequence(t_token_list *list, t_u8_iterator *it);
int	token_pipeline(t_token_list *list, t_u8_iterator *it);
int	token_keyword(t_token_list *list, t_u8_iterator *it);
int	token_double_quote(t_token_list *list, t_u8_iterator *it);
int	token_single_quote(t_token_list *list, t_u8_iterator *it);
int	token_word(t_token_list *list, t_u8_iterator *it);
int	token_arith(t_token_list *list, t_u8_iterator *it);
int	token_cmd_sub(t_token_list *list, t_u8_iterator *it);
int	token_param_simple(t_token_list *list, t_u8_iterator *it);
int	token_param(t_token_list *list, t_u8_iterator *it);
int	token_special(t_token_list *list, t_u8_iterator *it);

/** @brief Checks if token is word-like */
int
token_isword(enum e_token_type type);

/**
 * @brief Appends string word content of token to buffer
 *
 * In the token is not word-like, this function will return 0 and do nothing
 *
 * @param buf Buffer to append to
 * @param tok Token to append
 *
 * @returns 1 if `tok` is a word, 0 otherwise
 */
int
token_wordcontent(t_string_buffer *buf, const t_token *tok);

#endif // TOKENIZER_H
