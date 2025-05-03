/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/words/words.h"
#include "shell/expand/expand.h"
#include <shell/shell.h>

/** @brief Merge parameter (result) with a @ref W_LITERAL @ref t_atom */
static void
	merge_parameters(t_word *result, const t_atom *atom)
{
	t_atom *const	param = &result->atoms[result->natoms - 1];
	char			*joined;
	size_t			len;

	len = 0;
	while (len < atom->text.len && is_param_ident(atom->text.str[len]))
		++len;
	ft_asprintf(&joined, "%s%.*s", param->param.name, (int)len, atom->text.str);
	free(param->param.name);
	param->param.name = joined;
	if (len >= atom->text.len)
		return ;
	result->atoms[result->natoms] = *atom;
	result->atoms[result->natoms++].text = stringbuf_from_range(
			atom->text.str + len, atom->text.str + atom->text.len - len);
}

/** @brief Gather prefixes recursively, according to the current selectors */
static void
	gather_prefixes(t_brace_group *node, t_word *out)
{
	size_t	i;

	if (!node)
		return ;
	out->atoms = ft_realloc(out->atoms, sizeof(struct s_atom) * out->natoms,
			sizeof(struct s_atom) * (out->natoms + node->prefix.natoms));
	i = 0;
	while (i < node->prefix.natoms)
	{
		if (node->prefix.atoms[i].type == W_LITERAL
			&& !node->prefix.atoms[i].text.len && (++i))
			continue ;
		if (out->natoms
			&& node->prefix.atoms[i].type == W_LITERAL
			&& !(node->prefix.atoms[i].flags & (FL_SQUOTED | FL_DQUOTED))
			&& out->atoms[out->natoms - 1].type == W_PARAMETER
			&& out->atoms[out->natoms - 1].param.simple_param)
			merge_parameters(out, &node->prefix.atoms[i++]);
		else
			out->atoms[out->natoms++] = atom_copy(&node->prefix.atoms[i++]);
	}
	if (node->nalternatives > 0 && node->selector < node->nalternatives)
		gather_prefixes(&node->alternatives[node->selector], out);
	gather_prefixes(node->next, out);
}

/**
 * @brief Performs a single expansion of a @ref t_brace_group
 *
 * Expands the brace group once and store the resulting expansion into `out`.
 * The counter for `group` is increased.
 *
 * @param group Brace group to expand once
 * @param out Output word
 *
 * @returns 1 if further expansions can be made, 0 if no more expansions are
 * possible
 */
static int
	brace_expand(t_brace_group *group, t_word *out)
{
	if (out)
	{
		out->atoms = NULL;
		out->natoms = 0;
		gather_prefixes(group, out);
		return (brace_expand(group, NULL));
	}
	if (!group)
		return (0);
	if (brace_expand(group->next, NULL))
		return (1);
	if (group->nalternatives <= 0)
		return (0);
	if (brace_expand(&group->alternatives[group->selector], NULL))
		return (1);
	++group->selector;
	if (group->selector < group->nalternatives)
		return (1);
	group->selector = 0;
	return (0);
}

/** @brief Attempts to expand word */
static int
	try_expand(
	t_word *word,
	t_brace_group *group,
	t_word **result,
	size_t *result_len)
{
	if (!brace_parse(word, group))
		return (0);
	while (1)
	{
		*result = ft_realloc(*result, sizeof(t_word) * *result_len,
				sizeof(t_word) * (*result_len + 1));
		if (!brace_expand(group, &(*result)[(*result_len)++]))
			break ;
	}
	brace_group_free(group, 1);
	return (1);
}

void
	expand_braces(
	t_word **wordlist,
	size_t *len)
{
	size_t			i;
	t_word			*result;
	size_t			result_len;
	t_brace_group	group;

	result = NULL;
	result_len = 0;
	i = 0;
	while (i < *len)
	{
		if (!try_expand(wordlist[i], &group, &result, &result_len))
		{
			result = ft_realloc(result, sizeof(t_word) * result_len,
					sizeof(t_word) * (result_len + 1));
			result[result_len++] = word_copy(wordlist[i]);
		}
		++i;
	}
	*wordlist = result;
	*len = result_len;
}
