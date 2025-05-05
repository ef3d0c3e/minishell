/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "shell/env/env.h"
#include "shell/expand/expand.h"
#include "shell/regex/regex.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <shell/shell.h>
#include <stdio.h>

/** @brief Checks if a fragment range needs filename expansion by scanning for
 * reserved words */
static int
	needs_expansion(
	t_shell *shell,
	const t_fragment *start,
	const t_fragment *end)
{
	static const char	*alts[] = {"*", "?", NULL};
	static const char	*glob[] = {"@", "!", "+", NULL};
	const t_string_buffer	*word;

	while (start != end)
	{
		if (start->flags & (FL_SQUOTED | FL_DQUOTED) && ++start)
			continue ;
		word = &start->word;
		if (str_find_alternatives((t_string){word->str, word->len}, alts))
			return (1);
		else if (option_value(shell, "extglob")
			&& str_find_alternatives((t_string){word->str, word->len}, glob))
			return (1);
		++start;
	}
	return (0);
}

// TODO: Error handling for failglob
static int
	make_regex(
	const t_globopts *opts,
	t_fragment *start,
	t_fragment *end,
	t_regex *regex)
{
	t_regex_builder	builder;
	size_t			recurse;

	builder = regex_builder_new();
	while (start != end)
	{
		if (start->flags & (FL_SQUOTED | FL_DQUOTED))
		{
			if (!regex_builder_literal(opts, &builder,
						stringbuf_cstr(&start->word)))
				return (0);
		}
		else if (!regex_builder_expr(opts, &builder,
					stringbuf_cstr(&start->word)))
			return (0);
		++start;
	}
	*regex = builder.regex;
	return (1);
}

struct s_filename_traversal
{
	t_fragment_list	*list;
	t_globopts		opts;
	t_regex			regex;
};

static int
	collect_files(char *path, const struct stat *sb, void *ptr)
{
	struct s_filename_traversal *const	tr = ptr;

	if (!regex_match(&tr->opts, &tr->regex, path))
		return (0);
	ft_dprintf(2, "matching: `%s`\n", path);
	fraglist_push(tr->list, stringbuf_from(path), 0);
	tr->list->fragments[tr->list->size - 1].force_split = 1;
	return (0);
}

static int
	expand(
	t_shell *shell,
	t_fragment_list *list,
	t_fragment *start,
	t_fragment *end)
{
	t_string_buffer				s;
	size_t						recurse;
	struct s_filename_traversal	tr;

	if (!needs_expansion(shell, start, end))
	{
		stringbuf_init(&s, 16);
		while (start != end)
		{
			stringbuf_append(&s, (t_string){start->word.str, start->word.len});
			++start;
		}
		fraglist_push(list, s, 0);
		return (1);
	}
	tr.opts = regex_shellopt_get(shell);
	if (!make_regex(&tr.opts, start, end, &tr.regex))
		return (0);
	recurse = regex_recurse_depth(tr.regex.expr);
	if (recurse != (size_t)-1)
		recurse += 1;
	tr.list = list;
	regex_print(0, tr.regex.expr);
	file_tree_walk(".", recurse, collect_files, &tr);
	regex_free(tr.regex.expr);
	return (1);
}

t_fragment_list
	expand_filename(t_shell *shell, t_fragment_list *list)
{
	t_fragment_list new;
	size_t			i;
	size_t			start;

	i = 0;
	start = 0;
	fraglist_init(&new);
	while (i < list->size)
	{
		if (list->fragments[i].force_split)	
		{
			if (start != i)
			{
				expand(shell, &new, &list->fragments[start], &list->fragments[i]);
				new.fragments[new.size - 1].force_split = 1;
				start = i;
			}
		}
		++i;
	}
	if (start != i)
	{
		expand(shell, &new, &list->fragments[start], &list->fragments[i]);
		new.fragments[new.size - 1].force_split = 1;
	}
	fraglist_free(list);
	return (new);
}
