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

enum e_match_type
{
	M_LITERAL,
	/* @brief '?' */
	M_ANY,
	/* @brief '*' */
	M_STAR,
	/* @brief [...] */
	M_CHARCLASS,
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

/** @brief Data for @ref M_CHARCLASS */
struct s_charclass
{
	/** @brief 1 for negation */
	int		neg;
	/** @brief List of matched ascii characters */
	char	*list;
};

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
		/** @brief Data for @ref M_CHARCLASS */
		struct s_charclass	range;
		/** @brief Data for @ref M_EXTGLOB */
		struct s_extglob	glob;
		/** @brief Data for @ref M_SEQ and @ref M_ALT */
		struct s_compound	compound;
	};
}	t_regex_ast;

#endif // REGEX_H
