/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
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

/** @brief Merges prefix (result) with expanded word (source) */
static void
	merge_prefix(t_word *result, t_word *source)
{
	size_t	i;

	result->atoms = ft_realloc(result->atoms,
		sizeof(struct s_atom) * result->natoms,
		sizeof(struct s_atom) * (result->natoms + source->natoms));
	i = 0;
	while (i < source->natoms)
	{
		if (source->atoms[i].type == W_LITERAL && !source->atoms[i].text.len
			&& (++i))
			continue ;
		if (result->natoms
			&& source->atoms[i].type == W_LITERAL
			&& !(source->atoms[i].flags & (FL_SQUOTED | FL_DQUOTED))
			&& result->atoms[result->natoms - 1].type == W_PARAMETER
			&& result->atoms[result->natoms - 1].param.simple_param)
			merge_parameters(result, &source->atoms[i++]);
		else
			result->atoms[result->natoms++] = atom_copy(&source->atoms[i++]);
	}
}

/** @brief Gather prefixes recursively, according to the current selectors */
static void
	gather_prefixes(t_brace_group *node, t_word *out)
{
	if (!node)
		return ;
	merge_prefix(out, &node->prefix);
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

void
	expand_braces(
	t_shell *shell,
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
		if (brace_parse(wordlist[i], &group))
		{
			while (1)
			{
				result = ft_realloc(result, sizeof(t_word) * result_len,
						sizeof(t_word) * (result_len + 1));
				if (!brace_expand(&group, &result[result_len++]))
					break ;
			}
			brace_group_free(&group, 1);
		}
		else
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
