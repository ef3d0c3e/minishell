/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_for.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*make_for_node(char *ident)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_FOR;
	node->st_for.ident = ident;
	node->st_for.args.list = NULL;
	node->st_for.args.size = 0;
	node->st_for.body = NULL;
	return (node);
}

void
	free_for_node(t_ast_node *node)
{
	wordlist_free(&node->st_for.args);
	free(node->st_for.ident);
	ast_free(node->st_for.body, 0);
}

void
	print_for_node(size_t depth, const t_ast_node *node)
{
	print_pad(" | ", depth);
	ft_dprintf(2, "FOR '%s'\n", node->st_for.ident);
	print_pad(" | ", depth);
	ft_dprintf(2, "(WORDLIST)\n");
	wordlist_print(depth + 1, &node->st_for.args);
	print_pad(" | ", depth);
	ft_dprintf(2, "(BODY)\n");
	ast_print(depth + 1, node->st_for.body);
}
