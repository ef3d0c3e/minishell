/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 14:31:37 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static int
	collect_files(char *path, const struct stat *sb, void *ptr)
{
	struct s_filename_traversal *const	tr = ptr;
	int									r;

	(void)sb;
	r = regex_match(&tr->opts, &tr->regex, path);
	if (r == 0)
		return (1);
	if (r == 2)
	{
		fraglist_push(tr->list, stringbuf_from(path), 0);
		tr->list->fragments[tr->list->size - 1].force_split = 1;
	}
	return (0);
}

/** @brief Expand by walking files using FTW. If no files are matched
 * 0 is returned, and the caller must expand the regex's pattern literally */
static int
	expand_files(
			t_shell *shell,
			t_fragment_list *list,
			t_fragment *start,
			t_fragment *end
			)
{
	struct s_filename_traversal	tr;
	size_t						recurse;
	size_t						oldsz;
	char						*path;

	tr.opts = regex_shellopt_get(shell);
	if (!filename_make_regex(&tr.opts, start, end, &tr.regex))
		return (0);
	recurse = regex_recurse_depth(tr.regex.expr);
	tr.list = list;
	oldsz = tr.list->size;
	path = regex_path(&tr.regex);
	file_tree_walk(path, recurse + (recurse != (size_t)-1), collect_files, &tr);
	free(path);
	regex_free(tr.regex.expr);
	return (tr.list->size != oldsz);
}

// TODO: If no match, use globerr/globfail to determine what to do
static int
	expand(
	t_shell *shell,
	t_fragment_list *list,
	t_fragment *start,
	t_fragment *end)
{
	t_string_buffer	s;

	if (!filename_needs_expansion(shell, start, end)
		|| !expand_files(shell, list, start, end))
	{
		stringbuf_init(&s, 16);
		while (start != end)
		{
			stringbuf_append(&s, (t_string){start->word.str, start->word.len});
			++start;
		}
		fraglist_push(list, s, 0);
	}
	return (1);
}

static void
	expand_filename_(
			t_shell *shell,
			t_fragment_list *list,
			t_fragment_list *new,
			size_t *start
			)
{
	size_t			i;

	i = 0;
	while (i < list->size)
	{
		if (list->fragments[i].force_split)
		{
			if (*start != i)
			{
				expand(shell, new, &list->fragments[*start],
					&list->fragments[i]);
				new->fragments[new->size - 1].force_split = 1;
				*start = i;
			}
		}
		++i;
	}
	if (*start != i)
	{
		expand(shell, new, &list->fragments[*start], &list->fragments[i]);
		if (new->size)
			new->fragments[new->size - 1].force_split = 1;
	}
}

t_fragment_list
	expand_filename(t_shell *shell, t_fragment_list *list)
{
	t_fragment_list	new;
	size_t			start;

	start = 0;
	fraglist_init(&new);
	expand_filename_(shell, list, &new, &start);
	fraglist_free(list);
	return (new);
}
