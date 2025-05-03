/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*make_cmd_node(void)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_COMMAND;
	node->cmd.args.list = NULL;
	node->cmd.args.size = 0;
	node->cmd.assigns = NULL;
	node->cmd.nassigns = 0;
	node->cmd.redirs.redirs = NULL;
	node->cmd.redirs.redirs_capacity = 0;
	node->cmd.redirs.redirs_size = 0;
	return (node);
}

void
	free_cmd_node(t_ast_node *node)
{
	size_t	i;

	i = 0;
	while (i < node->cmd.nassigns)
	{
		word_free(&node->cmd.assigns[i].value);
		stringbuf_free(&node->cmd.assigns[i].variable);
		++i;
	}
	free(node->cmd.assigns);
	wordlist_free(&node->cmd.args);
	redirs_free(&node->cmd.redirs);
}

void
	print_cmd_node(size_t depth, const t_ast_node *node)
{
	size_t	i;

	print_pad(" | ", depth);
	ft_dprintf(2, "COMMAND\n");
	if (node->cmd.nassigns)
	{
		print_pad(" | ", depth);
		ft_dprintf(2, "(ASSIGNS)\n");
		i = 0;
		while (i < node->cmd.nassigns)
		{
			print_pad(" | ", depth);
			ft_dprintf(2, "%.*s=", node->cmd.assigns[i].variable.len,
					node->cmd.assigns[i].variable.str);
			word_print(0, &node->cmd.assigns[i].value);
			++i;
		}
		print_pad(" | ", depth);
		ft_dprintf(2, "(ARGS)\n");
	}
	wordlist_print(depth + 1, &node->cmd.args);
	print_redir(&node->cmd.redirs, depth + 1);
}
