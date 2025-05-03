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

static void
	merge_parameters(t_word *result, const t_atom *left)
{
	t_atom *const	param = &result->atoms[result->natoms - 1];
	char			*joined;
	size_t			len;

	len = 0;
	while (len < left->text.len && is_param_ident(left->text.str[len]))
		++len;
	ft_asprintf(&joined, "%s%.*s", param->param.name, (int)len, left->text.str);
	free(param->param.name);
	param->param.name = joined;
	if (len >= left->text.len)
		return ;
	result->atoms[result->natoms] = *left;
	result->atoms[result->natoms].text.str += len;
	result->atoms[result->natoms++].text.len -= len;
}

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

int
	parse_candidate(t_word *arg, t_brace_candidate *cand);

static void
	split_inner_1(t_brace_candidate *cand, t_word *inner)
{
	cand->alternatives = NULL;
	cand->nalternatives = 0;
	merge_prefix(&cand->prefix, inner);
}

static void
	expand_incr(t_brace_candidate *cand, int start, int end, int incr)
{
	int		i;
	size_t	size;

	if (!incr)
		incr = 1;
	if (incr < 0)
		incr *= -1;
	size = (end - start) / incr + 1;
	if (end <= start)
		size = (start - end) / incr + 1;
	cand->nalternatives = 0;
	cand->alternatives = xmalloc(sizeof(t_brace_candidate)
			* size);
	i = start;
	while (1)
	{
		//printf("i=%d\n", i);
		cand->alternatives[cand->nalternatives++] = (t_brace_candidate){
			.prefix = word_from_int(i, 0),
			.nalternatives = 0,
			.alternatives = NULL,
			.next = NULL,
			.selector = 0,
		};
		if (start <= end)
		{
			i += incr;
			if (i > end)
				break ;
			continue ;
		}
		i -= incr;
		if (i < end)
			break ;
	}
}

/** @brief Parses brace groups like this: `[X..Y[..INCR]]` */
static int
	parse_incr(t_brace_candidate *cand, t_word *inner)
{
	t_string	s;
	int			v[3];
	int			incr;
	size_t		i;

	if (inner->natoms != 1)
		return (0);
	s = (t_string){inner->atoms[0].text.str, inner->atoms[0].text.len};
	i = str_find(s, "..");
	if (i == (size_t ) - 1 || !atoi_range_checked(s.str, s.str + i, &v[0]))
		return (0);
	s.str += i + 2;
	s.len -= i + 2;
	i = str_find(s, "..");
	incr = 1;
	if (i == (size_t ) - 1)
	{
		incr = 0;
		i = s.len;
	}
	if (!atoi_range_checked(s.str, s.str + i, &v[1]))
		return (0);
	v[2] = 1;
	if (incr)
	{
		s.str += i + 2;
		s.len -= i + 2;
		if (!atoi_range_checked(s.str, s.str + s.len, &v[2]))
			return (0);
	}
	expand_incr(cand, v[0], v[1], v[2]);
	return (1);
}

/** @brief Counts the number of `,` separated groups */
static size_t
	count_groups(t_word *inner)
{
	size_t	i;
	size_t	j;
	size_t	count;
	int		balance;

	i = 0;
	balance = 0;
	count = 1;
	while (i < inner->natoms)
	{
		if ((inner->atoms[i].type != W_LITERAL
			|| inner->atoms[i].flags & (FL_SQUOTED | FL_SQUOTED))
			&& ++i)
			continue ;
		j = 0;
		while (j < inner->atoms[i].text.len)
		{
			if (inner->atoms[i].text.str[j] == '{')
				++balance;
			if (inner->atoms[i].text.str[j] == '}')
				--balance;
			if (inner->atoms[i].text.str[j] == ',' && balance == 0)
				++count;
			++j;
		}
		++i;
	}
	return (count);
}

static void
	split_inner(t_brace_candidate *cand, t_word *inner)
{
	size_t	count;
	size_t	i;
	size_t	j;
	size_t	last[2];
	int		balance;
	t_word	arg;

	count = count_groups(inner);
	/*
	i = 0;
	balance = 0;
	while (i < inner->natoms)
	{
		if ((inner->atoms[i].type != W_LITERAL
			|| inner->atoms[i].flags & (FL_SQUOTED | FL_SQUOTED))
			&& ++i)
			continue ;
		j = 0;
		while (j < inner->atoms[i].text.len)
		{
			if (inner->atoms[i].text.str[j] == '{')
				++balance;
			if (inner->atoms[i].text.str[j] == '}')
				--balance;
			if (inner->atoms[i].text.str[j] == ',' && balance == 0)
				++count;
			++j;
		}
		++i;
	}
	*/
	cand->alternatives = xmalloc(sizeof(t_brace_candidate) * count);
	cand->nalternatives = 0;
	i = 0;
	balance = 0;
	last[0] = 0;
	last[1] = 0;
	while (i < inner->natoms)
	{
		if ((inner->atoms[i].type != W_LITERAL
			|| inner->atoms[i].flags & (FL_SQUOTED | FL_DQUOTED))
			&& ++i)
			continue ;
		j = 0;
		while (j < inner->atoms[i].text.len)
		{
			if (inner->atoms[i].text.str[j] == '{')
				++balance;
			if (inner->atoms[i].text.str[j] == '}')
				--balance;
			if (inner->atoms[i].text.str[j] == ',' && !balance)
			{
				arg = word_sub(inner, (const size_t[4]){last[0], last[1], i + 1, j});
				//word_print(2, &arg);
				if (!parse_candidate(&arg, &cand->alternatives[cand->nalternatives++]))
				{
					cand->alternatives[cand->nalternatives - 1].alternatives = NULL;
					cand->alternatives[cand->nalternatives - 1].nalternatives = 0;
					cand->alternatives[cand->nalternatives - 1].selector = 0;
					cand->alternatives[cand->nalternatives - 1].prefix = arg;
					cand->alternatives[cand->nalternatives - 1].next = NULL;
				}
				else
					word_free(&arg);
				last[0] = i;
				last[1] = j + 1;
			}
			++j;
		}
		++i;
	}

	size_t end = 0;
	if (inner->atoms[inner->natoms - 1].type == W_LITERAL)
		end = inner->atoms[inner->natoms - 1].text.len;
	arg = word_sub(inner, (const size_t[4]){last[0], last[1], i, end});
	if (!parse_candidate(&arg, &cand->alternatives[cand->nalternatives++]))
	{
		cand->alternatives[cand->nalternatives - 1].alternatives = NULL;
		cand->alternatives[cand->nalternatives - 1].nalternatives = 0;
		cand->alternatives[cand->nalternatives - 1].selector = 0;
		cand->alternatives[cand->nalternatives - 1].prefix = arg;
		cand->alternatives[cand->nalternatives - 1].next = NULL;
	}
	else
		word_free(&arg);
}

static t_brace_candidate
	cand_split(t_word *arg, const size_t delims[4])
{
	t_brace_candidate	cand;
	size_t				end;
	t_word				suffix;
	t_word				inner;
	size_t				count;
	
	cand.selector = 0;
	inner = word_sub(arg, (const size_t[4]){delims[0], delims[1] + 1, delims[2] + 1, delims[3]});
	count = count_groups(&inner);
	if (parse_incr(&cand, &inner))
		count = 0;
	cand.prefix = word_sub(arg, (const size_t[4]){0, 0, delims[0] + 1, delims[1] + (count == 1)});
	end = 0;
	if (arg->atoms[arg->natoms - 1].type == W_LITERAL)
		end = arg->atoms[arg->natoms - 1].text.len;
	//ft_dprintf(2, "inner\n");
	//word_print(1, &inner);
	if (count)
		split_inner(&cand, &inner);
	cand.next = NULL;
	if (delims[2] < arg->natoms)
	{
		suffix = word_sub(arg, (const size_t[4]){delims[2], delims[3] + (count != 1), arg->natoms, end});
		//ft_dprintf(2, "Suffix\n");
		//word_print(0, &suffix);
		cand.next = xmalloc(sizeof(t_brace_candidate));
		if (!parse_candidate(&suffix, cand.next))
		{
			cand.next->alternatives = NULL;
			cand.next->nalternatives = 0;
			cand.next->selector = 0;
			cand.next->prefix = suffix;
			cand.next->next = NULL;
		}
		else
			word_free(&suffix);
	}
	word_free(&inner);
	return (cand);
}

int
	parse_candidate(t_word *arg, t_brace_candidate *cand)
{
	size_t	i;
	size_t	j;
	size_t	delims[4];
	int		balance;

	i = 0;
	delims[0] = -1;
	delims[2] = -1;
	balance = 0;
	while (i < arg->natoms)
	{
		if ((arg->atoms[i].type != W_LITERAL
			|| arg->atoms[i].flags & (FL_SQUOTED | FL_SQUOTED))
			&& ++i)
			continue ;
		j = 0;
		while (j < arg->atoms[i].text.len)	
		{
			if (arg->atoms[i].text.str[j] == '{')
			{
				if (!balance)
				{
					delims[0] = i;
					delims[1] = j;
				}
				++balance;
			}
			if (arg->atoms[i].text.str[j] == '}')
			{
				if (balance == 1)
				{
					delims[2] = i;
					delims[3] = j;
				}
				--balance;
				if (balance < 0)
					balance = 0;
			}
			if (delims[0] != (size_t) - 1 && delims[2] != (size_t) - 1)
			{
				*cand = cand_split(arg, delims);
				return (1);
			}
			++j;
		}
		++i;
	}
	return (0);
}

/** @brief Gather prefixes recursively, according to the current selectors */
static void
	gather_prefixes(t_brace_candidate *node, t_word *out)
{
	if (!node)
		return ;
	merge_prefix(out, &node->prefix);
	if (node->nalternatives > 0 && node->selector < node->nalternatives)
		gather_prefixes(&node->alternatives[node->selector], out);
	gather_prefixes(node->next, out);
}

/** @brief Increases current selectors, from the most nested to least nested */
static int
	bump_selector(t_brace_candidate *node)
{
	if (!node)
		return (0);
	if (bump_selector(node->next))
		return (1);
	if (node->nalternatives <= 0)
		return (0);
	if (bump_selector(&node->alternatives[node->selector]))
		return (1);
	++node->selector;
	if (node->selector < node->nalternatives)
		return (1);
	node->selector = 0;
	return (0);
}

/** @brief Performs a single expansion of a @ref t_brace_candidate */
int
	candidate_expand(t_brace_candidate *cand, t_word *out)
{
	out->atoms = NULL;
	out->natoms = 0;
    gather_prefixes(cand, out);
    return bump_selector(cand);
}

void
	expand_braces(
	t_shell *shell,
	t_word **wordlist,
	size_t *len)
{
	size_t				i;
	t_word				*result;
	size_t				result_len;
	t_brace_candidate	cand;

	result = NULL;
	result_len = 0;
	i = 0;
	while (i < *len)
	{
		if (parse_candidate(wordlist[i], &cand))
		{
			//brace_candidate_print(0, &cand);
			while (1)
			{
				result = ft_realloc(result, sizeof(t_word) * result_len,
						sizeof(t_word) * (result_len + 1));
				if (!candidate_expand(&cand, &result[result_len++]))
					break ;
				//word_print(0, &result[result_len - 1]);
			}
			brace_candidate_free(&cand, 1);
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
