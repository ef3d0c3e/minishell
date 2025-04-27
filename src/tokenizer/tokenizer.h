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
/* Token list                                                                 */
/* ************************************************************************** */

typedef struct s_token	t_token;

typedef struct s_token_list
{
	t_token	*tokens;
	size_t	size;
	size_t	capacity;
}	t_token_list;

/**
 * @brief Initialze a token list with a given default capacity
 *
 * @param list List to initialize
 * @param initial_capacity The list's initial capacity
 */
void
token_list_init(t_token_list *list, size_t initial_capacity);
/**
 * @brief Frees the token list (and the contained tokens)
 *
 * @param list Token list to free
 */
void
token_list_free(t_token_list *list);
/**
 * @brief Push `token` to the end of `list`
 *
 * @param list List to push `token` to
 * @param token Token to push to `list`
 */
void
token_list_push_token(t_token_list *list, t_token token);
/**
 * @brief Constructs a new token at the end of the list
 *
 * @param list List to create `token`
 * @param type Token type
 * @param start Token's start position
 * @param end Token's end position
 *
 * @returns The newly created token
 */
t_token
*token_list_push(
	t_token_list *list,
	int type,
	size_t start,
	size_t end);
/**
 * @brief Pushes codepoint from iterator into list
 *
 * This function will create a single-quote token or append to the last
 * token if it is a single-quote
 *
 * @param list List to push into
 * @param it Iterator to get the codepoint from
 */
void
list_push_codepoint(t_token_list *list, const t_u8_iterator *it);
/**
 * @brief Extends `result` while consuming `from`
 *
 * Inserts the content of `from` into `result` by applying `offset` to
 * all elements of `from`. List `from` is freed by this function.
 *
 * @param result List that will contain the result of the expansion
 * @param from List to add to `result`
 * @param offset Offset to apply to every elements being added to result
 */
void
list_extend(t_token_list *result, t_token_list *from, size_t offset);

/* ************************************************************************** */
/* Token definition                                                           */
/* ************************************************************************** */

enum e_token_flag
{
	FL_NONE = 0,
	/** @brief Token is single quoted */
	FL_SQUOTED = (1<<0),
	/** @brief Token is double quoted */
	FL_DQUOTED = (1<<1),
};

/**
 * @brief Frees a token
 *
 * Most token do not own thir data, but strings/words do
 *
 * @param token Token to free
 */
void
token_free(t_token *token);

/**
 * @brief Token types definitions
 */
enum e_token_type
{
	/** @brief Whitespace (non newline) character */
	TOK_SPACE,
	/** @brief A number stored as a string */
	TOK_DIGIT,
	/** @brief The minus sign token: `-` */
	TOK_MINUS,
	/** @brief Grouping character: `{`, `}`, `(` and `)` */
	TOK_GROUPING,
	/**
	 * @brief Pipeline token: `|` and `|&`
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Pipelines
	 */
	TOK_PIPELINE,
	/**
	 * @brief Logic operator tokens: `||` and `&&`
	 */
	TOK_OPERATOR,
	/**
	 * @brief Sequence separator tokens: `;` `\n`, `&` and EOF
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
	 * Single quoted text undergoes no further transformations
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Single-Quotes
	 */
	TOK_SINGLE_QUOTE,
	/** @brief Leftover content from tokenization */
	TOK_WORD,
	/**
	 * @brief Redirection tokens, e.g `<` `>`
	 *
	 * https://www.gnu.org/software/bash/manual/bash.html#Redirections
	 */
	TOK_REDIR,

	TOK_COMMENT,
	TOK_HEREDOC,
	TOK_HERESTRING,

	/** @brief Error message */
	TOK_ERROR,
};

/** @brief String data for tokens */
struct s_token_string
{
	t_string_buffer	string;
};

typedef struct s_token
{
	/** @brief Token type */
	enum e_token_type	type;
	/** @brief Token start byte position in the original input */
	size_t				start;
	/** @brief Token end byte position in the original input */
	size_t				end;
	/** @brief Flag for the token */
	enum e_token_flag	flags;

	union {
		/** @brief Reserved name for token, e.g keyword name, grouping char */
		const char		*reserved_word;
		/** @brief String content */
		t_string_buffer	word;
		/** @brief Error message */
		t_string		err;
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

/** @brief Tokenizer function, returns 1 and advances it on match */
typedef int	(*t_tokenizer_fn)(t_token_list *list, t_u8_iterator *it);

/**
 * @brief Tokenize a prompt
 *
 * @param t Tokenizer to use for tokenization
 * @param prompt Prompt to tokenize
 */
t_token_list
tokenizer_tokenize(t_string prompt);

/* ************************************************************************** */
/* Tokenizer utils                                                            */
/* ************************************************************************** */

/**
 * @brief Check if a given character can be part of a parameter identifier
 *
 * @param c The character to check
 * @return 1 if c is in `[a-zA-Z0-9_]`
 */
int
is_param_ident(char codepoint);

/** @brief Finds the next unescaped instance of `token` in `input` */
size_t
find_unescaped(t_string input, const char *token);

/**
 * @brief Finds the matching delimiter
 *
 * This function will try to find the next matching instance of `closing`.
 * For instance, finding parenthesis in `(a (b\)))` will return the last `)`
 * of the sequence.
 *
 * @param input Input string
 * @param opening Opening delimiter
 * @param closing Closing delimiter
 * @param escaped Apply escape rules via `\`
 *
 * @return Finds the matching delimiter while handling escapes and nested
 * delimiters
 */
size_t
find_matching(
	t_string input,
	const char *opening,
	const char *closing,
	int escaped);

/** @brief Checks if token is word-like */
int
token_isword(enum e_token_type type);
/**
 * @brief Appends string word content of token to buffer
 *
 * If the token is not word-like, this function will return 0 and do nothing
 *
 * @param buf Buffer to append to
 * @param tok Token to append
 *
 * @returns 1 if `tok` is a word, 0 otherwise
 */
int
token_wordcontent(t_string_buffer *buf, const t_token *tok);
/**
 * @brief Gets the precedence of an operator token
 *
 * @note There are currently 4 precedence levels:
 *  - 0 For pipes
 *  - 1 For `;` and `&`
 *  - 2 For `&&` and `||`
 *
 * @param tok Token to get precedence software
 * @return The precedence of `tok`, -1 if `tok` is not an operator
 */
int
token_precedence(const t_token *tok);
/**
 * @brief Filters escaped sequences according to `filter`
 *
 * @warning The filter has to be sorted by reverse length, e.g the longest
 * entries must be at the start of the filter.
 *
 * @param buf Buffer to filter, will contain the filtered result
 * @param filter The filter to use, must be NULL terminated
 */
void
escape_filter(t_string_buffer *buf, const char **filter);

int	token_comment(t_token_list *list, t_u8_iterator *it);
int	token_space(t_token_list *list, t_u8_iterator *it);
int	token_redir(t_token_list *list, t_u8_iterator *it);
int	token_digit(t_token_list *list, t_u8_iterator *it);
int	token_minus(t_token_list *list, t_u8_iterator *it);
int	token_grouping(t_token_list *list, t_u8_iterator *it);
int	token_sequence(t_token_list *list, t_u8_iterator *it);
int	token_operator(t_token_list *list, t_u8_iterator *it);
int	token_pipeline(t_token_list *list, t_u8_iterator *it);
int	token_keyword(t_token_list *list, t_u8_iterator *it);
int	token_double_quote(t_token_list *list, t_u8_iterator *it);
int	token_single_quote(t_token_list *list, t_u8_iterator *it);
int	token_word(t_token_list *list, t_u8_iterator *it);
int	token_arith(t_token_list *list, t_u8_iterator *it);
int	token_cmd_sub(t_token_list *list, t_u8_iterator *it);
int	token_param_simple(t_token_list *list, t_u8_iterator *it);
int	token_param(t_token_list *list, t_u8_iterator *it);

/**
 * @brief Prints errors in token list
 *
 * @param input Input string that generated the token list `list`
 * @param list The list to print errors from
 *
 * @returns 1 if no errors were displayed, 0 if at least one error occured
 */
int
report_tokens(t_string input, const t_token_list *list);

#endif // TOKENIZER_H
