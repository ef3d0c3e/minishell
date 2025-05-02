/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPAND_H
# define EXPAND_H

#include <parser/parser.h>

typedef struct s_shell	t_shell;

/** @brief A fragment */
typedef struct s_fragment
{
	t_string_buffer	word;
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

/** @brief Performs expansion over literals */
void
expand_literal(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param,
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

/******************************************************************************/
/* Arg expansion                                                              */
/******************************************************************************/

/**
 * @brief Resolves command arguments to an array of strings while performing
 * argument expansion
 *
 * @param shell The shell session
 * @param list Word list to expand
 * @param size Size of the word list
 *
 * @returns A null-terminated array of strings to be passed to `execve`
 */
char
**arg_expansion(t_shell *shell, struct s_word *list, size_t size);
/**
 * @brief Expands a single argument and concatenate the resulting words into
 * a single string separated by `ifs[0]`.
 *
 * @param shell The shell session
 * @param arg Argument to expand and concatenate
 *
 * @returns The resulting expansion
 */
char
*arg_expansion_single(t_shell *shell, struct s_word *list);

/******************************************************************************/
/* Brace expansion                                                            */
/******************************************************************************/

/** @brief A candidate for brace-expansion, tree-like structure */
typedef struct s_brace_canditate
{
	/** @brief Prefix for this brace group */
	t_word						prefix;
	/** @brief All alternatives sub-group for this brace group */
	struct s_brace_canditate	*alternatives;
	/** @brief Number of alternative sub-groups */
	size_t						nalternatives;
	/** @brief Next brace sub-group */
	struct s_brace_canditate	*next;

	/** @brief Position for the iteration code, initially `0` */
	size_t						selector;
}	t_brace_candidate;

/**
 * @brief Frees a brace candidate
 *
 * @param cand Brace candidate to free
 * @param root Whether this is the root node
 */
void
brace_candidate_free(t_brace_candidate *cand, int root);
/**
 * @brief Prints a brace candidate
 *
 * @param depth Print pad depth
 * @param cand Brace candidate to print
 */
void
brace_candidate_print(size_t depth, const t_brace_candidate *cand);

//int
//candidate_parse();

/**
 * @brief Performs a single expansion of a @ref t_brace_candidate
 *
 * Expands the candidate once and store the resulting expansion into `out`.
 * The counter for `cand` is increased.
 *
 * @param cand Candidate to expand once
 * @param out Output word
 *
 * @returns 1 if further expansions can be made, 0 if no more expansions are
 * possible
 */
int
candidate_expand(t_brace_candidate *cand, t_word *out);

/******************************************************************************/
/* Individual expanders                                                       */
/******************************************************************************/

/**
 * @brief Performs braces expansion
 *
 * @param shell The shell session
 * @param wordlist Wordlist to expand
 * @param len Length of `wordlist`
 *
 * Invalid brace expansions are left unchanged.
 */
void
expand_braces(
	t_shell *shell,
	t_word **wordlist,
	size_t *len);
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
	struct s_atom *param,
	const char *ifs);
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

#endif // EXPAND_H
