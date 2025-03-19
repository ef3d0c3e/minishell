#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <util/util.h>

/**
 * @brief Token types definitions
 */
enum e_token_type
{
	TOK_WORD,
	TOK_SINGLE_QUOTED,
	TOK_DOUBLE_QUOTED,

	/** @brief Identifier for variables */
	TOK_IDENT,
	/** @brief Token for variable expansion: `$VAR` */
	TOK_PARAM,
	/** @brief Token for brace variable expansion: `${VAR}` */
	TOK_PARAM_BRACE,
	/** @brief Arithmetic: `$((a+b))` */
	TOK_ARITH,
	/** @brief Parenthesis evaluation: `$(cmd)` */
	TOK_EVAL_PAR,

	TOK_REDIR,
	TOK_OPERATOR,
	TOK_NEWLINE,
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

typedef struct s_token
{
	/** @brief Token type */
	enum e_token_type	token;
	/** @brief Token content */
	t_string			str;
	/** @brief Token byte position in the original input */
	size_t				pos;
}	t_token;

void
token_print_debug(int fd, const t_token *token);

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
	
typedef int	(*t_tokenizer_fn)(t_token_list *, t_u8_iterator *it);

typedef struct s_tokenizer
{
	t_tokenizer_fn	*munchers;

}	t_tokenizer;

#endif // TOKENIZER_H
