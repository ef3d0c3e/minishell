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

typedef struct s_brace_canditate
{
	struct s_word			prefix;
	struct s_brace_canditate	*alternatives;
	size_t						nalternatives;
	struct s_brace_canditate	*next;

	size_t						selector;

}	t_brace_candidate;

static void
	print_cand(size_t depth, const t_brace_candidate *cand)
{
	size_t	i;

	print_pad(" | ", depth);
	ft_dprintf(2, "PREFIX ");
	if (cand->prefix.natoms)
		word_print(0, &cand->prefix);
	else
		ft_dprintf(2, "\n");
	i = 0;
	while (i < cand->nalternatives)
	{
		print_pad(" | ", depth);
		ft_dprintf(2, "ALTERNATIVES %zu\n", i);
		print_cand(depth + 1, &cand->alternatives[i]);
		++i;
	}
	if (cand->next)
	{
		print_pad(" | ", depth);
		ft_dprintf(2, "NEXT\n");
		print_cand(depth + 1, cand->next);
	}
}

static struct s_word
	arg_from_range(struct s_word *arg, const size_t range[4])
{
	t_string_buffer		*current;
	struct s_word	new;
	size_t				i;
	
	printf("range = {%zu %zu %zu %zu}\n", range[0], range[1], range[2], range[3]);
	new.natoms = (range[2] - range[0]);
	new.atoms = xmalloc(sizeof(struct s_atom) * new.natoms);
	i = range[0];
	while (i < range[2])
	{
		if (i == range[0] && arg->atoms[i].type == W_LITERAL)
		{
			current = &arg->atoms[i].text;
			new.atoms[i - range[0]] = (struct s_atom){
				.type = W_LITERAL,
				.flags = arg->atoms[i].flags,
			};
			if (i + 1 == range[2])
				new.atoms[i - range[0]].text = stringbuf_from_range(current->str + range[1],
						current->str + range[3]);
			else
				new.atoms[i - range[0]].text = stringbuf_from_range(current->str + range[1],
						current->str + current->len);
		}
		else if (i + 1 == range[2])
		{
			current = &arg->atoms[i].text;
			new.atoms[i - range[0]] = (struct s_atom){
				.type = W_LITERAL,
				.flags = arg->atoms[i].flags,
				.text = stringbuf_from_range(current->str,
						current->str + range[3])
			};
		}
		else
		{
			// TODO: Deep copy
			new.atoms[i - range[0]] = arg->atoms[i];
		}
		++i;
	}
	return (new);
}

//static void
//	cand_push(t_brace_candidate *cand, struct s_argument *arg)
//{
//	cand->alternatives = ft_realloc(cand->alternatives,
//			sizeof(struct s_argument) * cand->nalternatives,
//			sizeof(struct s_argument) * (cand->nalternatives + 1));
//	cand->alternatives[cand->nalternatives++] = *arg;
//}

int
	parse_candidate(struct s_word *arg, t_brace_candidate *cand);

static void
	split_inner(t_brace_candidate *cand, struct s_word *inner)
{
	size_t	count;
	size_t	i;
	size_t	j;
	size_t	last[2];
	int		balance;
	struct s_word	arg;

	count = 1;
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
			if (inner->atoms[i].text.str[j] == ',' && !balance)
				++count;
			++j;
		}
		++i;
	}
	// TODO: if count == 1, do not recurse
	printf("ncands: %zu\n", count);

	cand->alternatives = xmalloc(sizeof(t_brace_candidate) * count);
	cand->nalternatives = 0;
	i = 0;
	balance = 0;
	last[0] = 0;
	last[1] = 0;
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
			if (inner->atoms[i].text.str[j] == ',' && !balance)
			{
				arg = arg_from_range(inner, (const size_t[4]){last[0], last[1], i + 1, j});
				word_print(2, &arg);
				if (!parse_candidate(&arg, &cand->alternatives[cand->nalternatives++]))
				{
					cand->alternatives[cand->nalternatives - 1].alternatives = NULL;
					cand->alternatives[cand->nalternatives - 1].nalternatives = 0;
					cand->alternatives[cand->nalternatives - 1].selector = 0;
					cand->alternatives[cand->nalternatives - 1].prefix = arg;
					cand->alternatives[cand->nalternatives - 1].next = NULL;
				}
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
	arg = arg_from_range(inner, (const size_t[4]){last[0], last[1], i, end});
	word_print(2, &arg);
	if (!parse_candidate(&arg, &cand->alternatives[cand->nalternatives++]))
	{
		cand->alternatives[cand->nalternatives - 1].alternatives = NULL;
		cand->alternatives[cand->nalternatives - 1].nalternatives = 0;
		cand->alternatives[cand->nalternatives - 1].selector = 0;
		cand->alternatives[cand->nalternatives - 1].prefix = arg;
		cand->alternatives[cand->nalternatives - 1].next = NULL;
	}
}

static t_brace_candidate
	cand_split(struct s_word *arg, const size_t delims[4])
{
	t_brace_candidate	cand;
	size_t				end;
	struct s_word	suffix;
	struct s_word	inner;
	
	cand.selector = 0;
	cand.prefix = arg_from_range(arg, (const size_t[4]){0, 0, delims[0] + 1, delims[1]});
	end = 0;
	if (arg->atoms[arg->natoms - 1].type == W_LITERAL)
		end = arg->atoms[arg->natoms - 1].text.len;
	inner = arg_from_range(arg, (const size_t[4]){delims[0], delims[1] + 1, delims[2] + 1, delims[3]});
	ft_dprintf(2, "inner\n");
	word_print(1, &inner);
	split_inner(&cand, &inner);
	cand.next = NULL;
	if (delims[2] < arg->natoms)
	{
		suffix = arg_from_range(arg, (const size_t[4]){delims[2], delims[3] + 1, arg->natoms, end});
		ft_dprintf(2, "Suffix\n");
		word_print(0, &suffix);
		cand.next = xmalloc(sizeof(t_brace_candidate));
		if (!parse_candidate(&suffix, cand.next))
		{
			cand.next->alternatives = NULL;
			cand.next->nalternatives = 0;
			cand.next->selector = 0;
			cand.next->prefix = suffix;
			cand.next->next = NULL;
		}
	}
	return (cand);
}

int
	parse_candidate(struct s_word *arg, t_brace_candidate *cand)
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
				--balance;
				if (!balance)
				{
					delims[2] = i;
					delims[3] = j;
				}
			}
			if (delims[0] != (size_t) - 1 && delims[2] != (size_t) - 1)
			{
				printf("delims = {%zu %zu %zu %zu}\n", delims[0], delims[1], delims[2], delims[3]);
				*cand = cand_split(arg, delims);
				return (1);
			}
			++j;
		}
		++i;
	}
	return (0);
}

static void
	merge_segments(struct s_word *result, struct s_word *source)
{
	size_t	i;

	result->atoms = ft_realloc(result->atoms,
		sizeof(struct s_atom) * result->natoms,
		sizeof(struct s_atom) * (result->natoms + source->natoms));
	i = 0;
	while (i < source->natoms)
		result->atoms[result->natoms++] = source->atoms[i++];
}

static int
	expand_impl(t_brace_candidate *cand, struct s_word *out)
{
	if (!cand)
		return (0);
	merge_segments(out, &cand->prefix);

	if (cand->nalternatives)
		cand->selector %= cand->nalternatives;
	if (cand->selector < cand->nalternatives)
	{
		if (!expand_impl(&cand->alternatives[cand->selector], out))
			++cand->selector;
		if (expand_impl(cand->next, out))
			--cand->selector;
		return (cand->selector < cand->nalternatives);
	}
	else
		return (expand_impl(cand->next, out));
}

int
	expand_candidate(t_brace_candidate *cand, struct s_word *out)
{
	out->atoms = NULL;
	out->natoms = 0;
	return (expand_impl(cand, out));
}

int
	expand_braces(
	t_shell *shell,
	struct s_word *arg)
{
	t_brace_candidate cand;
	struct s_word out;

	out.atoms = NULL;
	out.natoms = 0;
	if (parse_candidate(arg, &cand))
	{
		print_cand(0, &cand);
		expand_candidate(&cand, &out);
		ft_dprintf(2, "result=\n");
		word_print(0, &out);

		expand_candidate(&cand, &out);
		ft_dprintf(2, "result=\n");
		word_print(0, &out);

		expand_candidate(&cand, &out);
		ft_dprintf(2, "result=\n");
		word_print(0, &out);
	}
	// TODO...
	return (0);
}
