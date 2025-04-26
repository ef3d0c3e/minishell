/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"
#include "util/util.h"
#include <shell/shell.h>

void
	ast_free(t_ast_node *head)
{
	if (!head)
		return ;
	if (head->type == NODE_BLOCK)
	{
		ast_free(head->block.inner);
	}
	else if (head->type == NODE_SUBSHELL)
	{
		ast_free(head->expr.head);
		redirs_free(&head->expr.redirs);
	}
	else if (head->type == NODE_COMMAND)
	{
		arglist_free(head->cmd.args, head->cmd.nargs);
		redirs_free(&head->cmd.redirs);
	}
	else if (head->type == NODE_LOGIC)
	{
		ast_free(head->logic.left);
		ast_free(head->logic.right);
	}
	else if (head->type == NODE_FUNCTION)
	{
		stringbuf_free(&head->function.name);
		ast_free(head->function.body);
	}
	else if (head->type == NODE_FUNCTION)
	{
		stringbuf_free(&head->function.name);
		ast_free(head->function.body);
	}
	free(head);
}

static void
	pad(size_t depth)
{
	for (size_t i = 0; i < depth; ++i)
		write(2, " | ", 3);
}

void
	ast_print_debug(
		t_string input,
		t_ast_node *head,
		size_t depth)
{
	size_t	i;

	if (!head)
		return ;
	pad(depth);
	if (head->type == NODE_BLOCK)
	{
		ft_dprintf(2, "BLOCK\n");
		ast_print_debug(input, head->block.inner, depth + 1);
	}
	else if (head->type == NODE_SUBSHELL)
	{
		ft_dprintf(2, "SUBSHELL\n");
		ast_print_debug(input, head->expr.head, depth + 1);
		print_redir(&head->expr.redirs, depth + 2);
	}
	else if (head->type == NODE_COMMAND)
	{
		ft_dprintf(2, "COMMAND [%zu]\n", head->cmd.nargs);
		arglist_print(depth + 1, head->cmd.args, head->cmd.nargs);
		print_redir(&head->cmd.redirs, depth + 2);
	}
	else if (head->type == NODE_LOGIC)
	{
		ft_dprintf(2, "LOGIC `%s`\n", head->logic.token.reserved_word);
		ast_print_debug(input, head->logic.left, depth + 1);
		ast_print_debug(input, head->logic.right, depth + 1);
	}
	else if (head->type == NODE_FUNCTION)
	{
		ft_dprintf(2, "FUNCTION `%.*s`\n", head->function.name.len, head->function.name.str);
		ast_print_debug(input, head->function.body, depth + 1);
	}
	else if (head->type == NODE_IF)
	{
		i = 0;
		while (i < head->st_if.nconds)
		{
			ft_dprintf(2, "%s\n", &"IF\0ELIF"[!!i * 3]);
			pad(depth + 1);
			ft_dprintf(2, "(COND)\n");
			ast_print_debug(input, head->st_if.conds[i], depth + 1);
			pad(depth + 1);
			ft_dprintf(2, "(BODY)\n");
			ast_print_debug(input, head->st_if.bodies[i], depth + 1);
			++i;
		}
		if (i < head->st_if.nbodies)
		{
			ft_dprintf(2, "ELSE\n");
			ast_print_debug(input, head->st_if.bodies[i], depth + 1);
		}
	}
}
