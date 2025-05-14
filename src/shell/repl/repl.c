/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	path_traversal(size_t depth, t_rbnode *node, void *cookie)
{
	t_path_tr *const	tr = cookie;
	
	tr->items[tr->index++] = (t_complete_item){
		.kind = COMPLETE_WORD,
		.name = ft_strdup(node->key),
		.desc = ft_strdup("Executable"),
	};
}

t_complete_item
	*repl_completer(t_getline *line)
{
	t_path_tr	tr;

	tr.index = 0;
	tr.items = xmalloc(sizeof(t_complete_item)
			* (line->shell->path_cache.size + 1));
	rb_apply(&line->shell->path_cache, path_traversal, &tr);
	tr.items[tr.index].name = NULL;
	return (tr.items);
}

t_getline
	repl_setup(t_shell *shell)
{
	t_getline	line;

	line = getline_setup(shell);
	line.highlighter_fn = repl_highlighter;
	line.comp_provider_fn = repl_completer;
	return (line);
}
