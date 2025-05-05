/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REGEX_H
# define REGEX_H

# include <parser/parser.h>

/** @brief Glob options, controlled by the `shopt` builtin and variables */
typedef struct s_globopts
{
	/* @brief allow extended glob patterns */
	int		extglob;
	/* @brief allow ** matching directories */
	int		globstar;
	/* @brief include leading-dot names in * globs */
	int		dotglob;
	/* @brief case-insensitive matching */
	int		nocaseglob;
	/* @brief non-matches expand to zero args */
	int		nullglob;
	/* @brief non-matches cause error */
	int		failglob;
	/* @brief patterns to exclude */
	char	**globinore;
	size_t	globignore_len;
	/* @brief suffixes to ignore */
	char	**fignore;
	size_t	finore_len;
}	t_globopts;

/******************************************************************************/
/* Regex AST                                                                  */
/******************************************************************************/

enum e_match_type
{
	M_LITERAL,
	/* @brief '?' */
	M_ANY,
	/* @brief '*' */
	M_STAR,
	/* @brief ?(...), *(...), etc. */
	M_EXTGLOB,
	/* @brief ** */
	M_GLOBSTAR,
	/* @brief sequence of nodes */
	M_SEQ,
	/* @brief alternation for extglob */
	M_ALT,
};

typedef struct s_regex_ast	t_regex_ast;

/** @brief Data for @ref M_EXTGLOB */
struct s_extglob
{
	/** @brief One of `?`, `*`, `+`, `@` or `!` */
	char		op;
	/** @brief Sub group */
	t_regex_ast	**groups;
	/** @brief Number of sub groups */
	size_t		ngroups;
};

/** @brief Data for @ref M_SEQ and @ref M_ALT */
struct s_compound
{
	/** @brief Sub group */
	t_regex_ast	**groups;
	/** @brief Number of sub groups */
	size_t		ngroups;
};

/** @brief Ast for the regex */
typedef struct s_regex_ast
{
	/** @brief Type of node */
	enum e_match_type		type;
	union
	{
		/** @brief Data for @ref M_LITERAL */
		char				*literal;
		/** @brief Data for @ref M_EXTGLOB */
		struct s_extglob	glob;
		/** @brief Data for @ref M_SEQ and @ref M_ALT */
		struct s_compound	compound;
	};
}	t_regex_ast;

void
regex_free(t_regex_ast *node);
t_regex_ast
*regex_new(enum e_match_type type);
void
regex_print(size_t depth, const t_regex_ast *node);

/**
 * @brief Appends to a @ref M_SEQ node
 *
 * @param seq @ref M_SEQ node
 * @param node Node to append
 */
void
regex_seq_append(t_regex_ast *seq, t_regex_ast *node);

/******************************************************************************/
/* Regex                                                                      */
/******************************************************************************/

typedef struct s_regex
{
	/** @brief Parsed expression */
	t_regex_ast			*expr;
	/** @brief Errors from the parser */
	char				**errors;
	/** @brief Number of errors */
	size_t				errors_size;
	/** @brief Errors capacity */
	size_t				errors_capacity;
	/** @brief Flag to indicate that the regex has at least one `dotglob`
	 * When this is turned on, the matcher will recursively scan directories */
	int					has_dotglob;
}	t_regex;

typedef struct s_reg_parser	t_reg_parser;

/******************************************************************************/
/* Regex parser                                                               */
/******************************************************************************/

typedef struct s_reg_parser
{
	/** @brief Pattern options */
	const t_globopts	*opts;
	/** @brief Input string */
	const char			*str;
	/** @brief Parser's position */
	size_t				pos;
	/** @brief Parsed regex */
	t_regex				regex;
}	t_reg_parser;

t_regex
regex_parse(const t_globopts *opts, const char *pattern);
/**
 * @brief Adds an error to the regex
 *
 * @param parser Regex parser
 * @param msg Error message
 */
void
regex_error(t_reg_parser *parser, const char *msg, size_t pos);
/**
 * @brief Writes all errors to stderr
 *
 * @returns 1 if no errors were written, 0 otherwise
 */
int
regex_error_flush(t_reg_parser *parser);

/******************************************************************************/
/* Regex builder                                                              */
/******************************************************************************/

typedef struct s_regex_builder
{
	t_regex	regex;
}	t_regex_builder;

/**
 * @brief Creates a new empty regex builder
 */
t_regex_builder
regex_builder_new(void);
/**
 * @brief Adds a regex expression to the builder
 *
 * @param builder Regex builder
 * @param expr Expression string to add
 *
 * @returns 1 on success, 0 on failure. On failure, an error message will be
 * printed to stderr and the regex will be freed.
 */
int
regex_builder_expr(
	const t_globopts *opts,
	t_regex_builder *builder,
	const char *expr);
/**
 * @brief Adds a literal string to the builder
 *
 * @param builder Regex builder
 * @param literal String to add
 *
 * @returns 1 on success, 0 on failure. On failure, an error message will be
 * printed to stderr and the regex will be freed.
 */
int
regex_builder_literal(
	const t_globopts *opts,
	t_regex_builder *builder,
	const char *literal);

#endif // REGEX_H
