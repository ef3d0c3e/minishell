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
#include "ft_printf.h"
#include "parser/args.h"
#include "util/util.h"
#include <shell/shell.h>
#include <stddef.h>
#include <stdio.h>

typedef struct s_brace_canditate
{
	struct s_argument	prefix;
	struct s_argument	suffix;
	struct s_argument	*alternatives;
	size_t				nalternatives;

}	t_brace_candidate;

static void
	print_cand(const t_brace_candidate *cand)
{
	ft_dprintf(2, "|prefix|");
	if (cand->prefix.nitems)
		arg_print(0, &cand->prefix);
	else
		ft_dprintf(2, "\n");
	ft_dprintf(2, "|suffix|");
	if (cand->suffix.nitems)
		arg_print(0, &cand->suffix);
	else
		ft_dprintf(2, "\n");
	//ft_dprintf(2, "|alternatives|");
	//arglist_print(0, cand->alternatives, cand->nalternatives);
}

static struct s_argument
	arg_from_range(struct s_argument *arg, const size_t range[4])
{
	t_string_buffer		*current;
	struct s_argument	new;
	size_t				i;
	
	printf("range = {%zu %zu %zu %zu}\n", range[0], range[1], range[2], range[3]);
	new.nitems = (range[2] - range[0]);
	new.items = xmalloc(sizeof(struct s_arg_item) * new.nitems);
	i = range[0];
	while (i < range[2])
	{
		if (i == range[0])
		{
			current = &arg->items[i].text;
			new.items[i - range[0]] = (struct s_arg_item){
				.type = ARG_LITERAL,
				.flags = arg->items[i].flags,
			};
			if (i + 1 == range[2])
				new.items[i - range[0]].text = stringbuf_from_range(current->str + range[1],
						current->str + range[3]);
			else
				new.items[i - range[0]].text = stringbuf_from_range(current->str + range[1],
						current->str + current->len);
		}
		else if (i + 1 == range[2])
		{
			current = &arg->items[i].text;
			new.items[i - range[0]] = (struct s_arg_item){
				.type = ARG_LITERAL,
				.flags = arg->items[i].flags,
				.text = stringbuf_from_range(current->str,
						current->str + range[3])
			};
		}
		else
		{
			// TODO: Deep copy
			new.items[i - range[0]] = arg->items[i];
		}
		++i;
	}
	return (new);
}

static void
	cand_push(t_brace_candidate *cand, struct s_argument *arg)
{
	cand->alternatives = ft_realloc(cand->alternatives,
			sizeof(struct s_argument) * cand->nalternatives,
			sizeof(struct s_argument) * (cand->nalternatives + 1));
	cand->alternatives[cand->nalternatives++] = *arg;
}

static t_brace_candidate
	cand_split(struct s_argument *arg, const size_t delims[4])
{
	t_brace_candidate	cand;
	size_t				end;
	size_t				start[2];
	
	cand.prefix = arg_from_range(arg, (const size_t[4]){0, 0, delims[0], delims[1]});
	end = 0;
	if (arg->items[arg->nitems - 1].type == ARG_LITERAL)
		end = arg->items[arg->nitems - 1].text.len;
	cand.suffix = arg_from_range(arg, (const size_t[4]){delims[2], delims[3], arg->nitems, end});
	cand.nalternatives = 0;
	cand.alternatives = NULL;

	start[0] = delims[0];
	start[1] = delims[1];

	
	return (cand);
}


int
	parse_candidate(struct s_argument *arg, t_brace_candidate *cand)
{
	size_t	i;
	size_t	j;
	size_t	delims[4];

	i = 0;
	delims[0] = -1;
	delims[2] = -1;
	while (i < arg->nitems)
	{
		if ((arg->items[i].type != ARG_LITERAL
			|| arg->items[i].flags & (FL_SQUOTED | FL_SQUOTED))
			&& ++i)
			continue ;
		j = 0;
		while (j < arg->items[i].text.len)	
		{
			if (delims[0] == (size_t) - 1 && arg->items[i].text.str[j] == '{')
			{
				delims[0] = i;
				delims[1] = j;
			}
			if (arg->items[i].text.str[j] == '}')
			{
				delims[2] = i;
				delims[3] = j + 1;
			}
			++j;
		}
		++i;
	}
	if (delims[0] == (size_t) - 1 || delims[2] == (size_t) - 1)
		return (0);
	printf("delims = {%zu %zu %zu %zu}\n", delims[0], delims[1], delims[2], delims[3]);
	*cand = cand_split(arg, delims);
	return (1);
}

int
	expand_braces(
	t_shell *shell,
	struct s_argument *arg)
{
	t_brace_candidate cand;

	if (parse_candidate(arg, &cand))
	{
		print_cand(&cand);
	}
	// TODO...
	return (0);
}
