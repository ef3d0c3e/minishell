/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_if.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*make_if_node(void)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_IF;
	node->st_if.conds = NULL;
	node->st_if.nconds = 0;
	node->st_if.bodies = NULL;
	node->st_if.nbodies = 0;
	return (node);
}

void
	free_if_node(t_ast_node *node)
{
	size_t	i;

	i = 0;
	while (i < node->st_if.nconds)
	{
		ast_free(node->st_if.bodies[i], 0);
		ast_free(node->st_if.conds[i], 0);
		++i;
	}
	if (i < node->st_if.nbodies)
		ast_free(node->st_if.bodies[i], 0);
	free(node->st_if.bodies);
	free(node->st_if.conds);
}

void
	print_if_node(size_t depth, const t_ast_node *node)
{
	size_t	i;

	i = 0;
	while (i < node->st_if.nconds)
	{
		print_pad(" | ", depth);
		ft_dprintf(2, "%s\n", &"IF\0ELIF"[!!i * 3]);
		print_pad(" | ", depth);
		ft_dprintf(2, "(COND)\n");
		ast_print(depth + 1, node->st_if.conds[i]);
		print_pad(" | ", depth);
		ft_dprintf(2, "(BODY)\n");
		ast_print(depth + 1, node->st_if.bodies[i]);
		++i;
	}
	if (i < node->st_if.nbodies)
	{
		print_pad(" | ", depth);
		ft_dprintf(2, "ELSE\n");
		ast_print(depth + 1, node->st_if.bodies[i]);
	}
}
