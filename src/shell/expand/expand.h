/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 12:51:49 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPAND_H
# define EXPAND_H

# include <parser/parser.h>
# include <shell/regex/regex.h>

typedef struct s_shell	t_shell;

/** @brief A fragment */
typedef struct s_fragment
{
	/** @brief String content */
	t_string_buffer	word;
	/** @brief Original token flags */
	int				flags;
	/** @brief Force this fragment to become the beginning of it's own
	 * argument */
	int				force_split;
}	t_fragment;

/** @brief A list of fragment */
typedef struct s_fragment_list
{
	/** @brief The fragments */
	t_fragment	*fragments;
	/** @brief Number of fragments */
	size_t		size;
	/** @brief Capacity of this list */
	size_t		capacity;
}	t_fragment_list;

struct s_filename_traversal
{
	t_fragment_list	*list;
	t_globopts		opts;
	t_regex			regex;
};

/** @brief Performs expansion over literals */
void
expand_literal(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param,
	const char *ifs);
/** @brief Performs expansion of a single argument */
int
expand_arg(
	t_shell *shell,
	t_fragment_list *list,
	t_word *arg,
	const char *ifs);
/**
 * @brief Initializes an empty fragment list
 *
 * @param list List to initialize
 */
void
fraglist_init(t_fragment_list *list);
/**
 * @brief Frees a fragment list
 *
 * @param list List to free
 */
void
fraglist_free(t_fragment_list *list);
/**
 * @brief Pushes a word to the fragment list
 *
 * @param list List to push to
 * @param word Word to push
 * @param Original word flags
 */
void
fraglist_push(t_fragment_list *list, t_string_buffer word, int flags);
/**
 * @brief Appends content to the last member of the fragment list
 *
 * If the list is empty, this is equivalent to `fraglist_push(list, word, 0)`.
 *
 * @param list List to append to
 * @param word Content to append to the last element of `list`
 */
void
fraglist_append(t_fragment_list *list, t_string_buffer word);
/**
 * @brief Consumes a fraglist into a list of arguments for execve
 *
 * @param list Fraglist to consume
 *
 * @returns A null-terminated array of strings
 */
char
**fraglist_to_argv(t_fragment_list *list);
/**
 * @brief Prints fragment list to stderr for debugging
 *
 * @param list List to print
 */
void
fraglist_print(const t_fragment_list *list);

/******************************************************************************/
/* Word expansion                                                             */
/******************************************************************************/

/**
 * @brief Resolves command arguments to an array of strings while performing
 * argument expansion
 *
 * @param shell The shell session
 * @param list Word list to expand
 *
 * @returns A null-terminated array of strings to be passed to `execve`
 */
char
**word_expansion(t_shell *shell, t_wordlist *list);
/**
 * @brief Expands a single word to a list of argument
 *
 * @param shell The shell session
 * @param word Single word to expand
 *
 * @returns A null-terminated array of strings
 */
char
**word_expansion_single(t_shell *shell, t_word *word);
/**
 * @brief Expands a single argument and concatenate the resulting words into
 * a single string separated by `ifs[0]`.
 *
 * @param shell The shell session
 * @param word Word to expand and concatenate using `ifs[0]`
 *
 * @returns The resulting expansion
 */
char
*word_expansion_cat(t_shell *shell, t_word *word);

/******************************************************************************/
/* Brace expansion                                                            */
/******************************************************************************/

/** @brief A candidate for brace-expansion, forest structure */
typedef struct s_brace_group
{
	/** @brief Prefix for this brace group */
	t_word					prefix;
	/** @brief All alternatives sub-group for this brace group */
	struct s_brace_group	*alternatives;
	/** @brief Number of alternative sub-groups */
	size_t					nalternatives;
	/** @brief Next brace sub-group */
	struct s_brace_group	*next;
	/** @brief Position for the iteration logic, initially `0` */
	size_t					selector;
}	t_brace_group;

/**
 * @brief Frees a brace group
 *
 * @param group Brace group to free
 * @param root Whether this is the root node
 */
void
brace_group_free(t_brace_group *group, int root);
/**
 * @brief Prints a brace group
 *
 * @param depth Print pad depth
 * @param group Brace group to print
 */
void
brace_group_print(size_t depth, const t_brace_group *group);
/**
 * @brief Splits a word for brace parsing
 *
 * The following input `prefix{a,b}next..` is split into the following parts:
 *  - Prefix `prefix`: becomes a @ref t_word `(START, (delim[0], delim[1]))`
 *  - Inner `{a,b}`: is recursively parsed to create further brace groups
 *	`((delim[0], delim[1]), (delim[2], delim[3]))`
 *  - Next `next`: is recursively parsed by calling this function with the next
 * 	brace groups as delimiters `((delim[2], delim[3]), END)`.
 *
 *  @param arg Word to perform splitting on
 *  @param delims Delimiters parsed by @ref
 *
 *  @returns The parsed brace group
 */
t_brace_group
brace_split(t_word *arg, const size_t delims[4]);
/**
 * @brief Parses brace groups like this: `[X..Y[..INCR]]`
 *
 * Currently, this does not handle ASCII-based ranges
 *
 * @param group Resulting brace group
 * @param inner Content to parse and expand
 *
 * @returns 0 if no range matched, 1 if a range matched.
 */
int
brace_parse_range(t_brace_group *group, t_word *inner);
/**
 * @brief Parses a brace group
 *
 * This function calls itself recursively in order to parse the subsequent brace
 * group
 *
 * @param word Word to parse braces in
 * @param group Resulting brace group parsed
 *
 * @returns 1 on success, 0 if no valid brace groups are found in `arg`
 */
int
brace_parse(t_word *word, t_brace_group *group);

/******************************************************************************/
/* Individual expanders                                                       */
/******************************************************************************/

/**
 * @brief Performs braces expansion
 *
 * @param word Word to expand
 * @param list Resulting @ref t_wordlist
 *
 * Invalid brace expansions are left unchanged.
 */
void
expand_braces(t_word *word, t_wordlist *list);
/**
 * @brief Resolves a parameter in the current context
 *
 * Resolution is done in the following order
 *  - Check for special parameter: $?, $# $@, etc.
 *  - Check for positional parameters: $0, $1, $2, etc. Positional parameters
 *  are look for in the current evaluation stack frame
 *  - Check for local variables in the current evaluation stack frame
 *  - Check for global variables in the environment
 *
 * @param shell The shell session
 * @param list Fragment list to insert into
 * @param param Word to expand
 * @param ifs The shell's IFS variables, fallback to ` \t\n` if unset or invalid
 *
 * @returns 1 On success, 0 on unmatched. Or -1 In `experr` mode and expansion
 * fails. An error will be reported, further processing should stop.
 */
int
expand_param(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param,
	const char *ifs);
/**
 * @brief Performs tilde `~` expansion
 *
 * @param shell The shell session
 * @param list Fragment list to insert into
 * @param param Sub expression to expand
 * @param ifs The shell's IFS variables, fallback to ` \t\n` if unset or invalid
 *
 * @returns 1 On success, 0 on unmatched. Or -1 In `experr` mode and expansion
 * fails. An error will be reported, further processing should stop.
 */
int
expand_tilde(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param,
	const char *ifs);
/**
 * @brief Expands sub expressions
 *
 * @param shell The shell session
 * @param list Fragment list to insert into
 * @param param Sub expression to expand
 * @param ifs The shell's IFS variables, fallback to ` \t\n` if unset or invalid
 *
 * @returns 1 On success, 0 on failure.
 */
int
expand_subexpr(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param);
/**
 * @brief Performs word splitting
 *
 * @param shell The shell session
 * @param list The original word list (will be consumed)
 * @param ifs The shell's IFS variables, fallback to ` \t\n` if unset or invalid
 *
 * @returns The list of words after performing splitting
 */
t_fragment_list
word_split(t_shell *shell, t_fragment_list *list, const char *ifs);

/**
 * @brief Performs filename expansion
 *
 * @param shell The shell session
 * @param list The list of fragments
 *
 * @returns The list of words after performing filename expansion
 */
t_fragment_list
expand_filename(t_shell *shell, t_fragment_list *list);

/**
 * @brief Checks if argument needs filename expansion
 *
 * @param shell The shell session
 * @param start Argument start
 * @param end Argument end
 *
 * @retruns 1 If argument [start, end) needs expansion]
 */
int
filename_needs_expansion(
	t_shell *shell,
	const t_fragment *start,
	const t_fragment *end);
/**
 * @brief Build filename expansion regex for argument
 *
 * @param shell The shell session
 * @param start Argument start
 * @param end Argument end
 * @param regex Resulting regex
 *
 * @retruns 1 on success, 0 on failure
 */
int
filename_make_regex(
	const t_globopts *opts,
	t_fragment *start,
	t_fragment *end,
	t_regex *regex);

#endif // EXPAND_H
