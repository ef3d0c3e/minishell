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
#include "parser/redir.h"
#include <parser/parser.h>

void
	ast_free(t_ast_node *head)
{
	if (!head)
		return ;
	if (head->type == NODE_LOGIC)
	{
		ast_free(head->logic.left);
		ast_free(head->logic.right);
	}
	else if (head->type == NODE_UNARY)
	{
		// TODO
	}
	else if (head->type == NODE_ATOM || head->type == NODE_PARAMETER)
	{
		stringbuf_free(&head->atom);
		return ;
	}
	else if (head->type == NODE_SUBEXPR)
	{
		ast_free(head->expr.head);
		return ;
	}
	else if (head->type == NODE_SUBSHELL)
	{
		ast_free(head->expr.head);
		redirs_free(&head->expr.redirs);
	}
	else if (head->type == NODE_COMMAND)
	{
		for (size_t i = 0; i < head->cmd.nargs; ++i)
		{
			for (size_t j = 0; j < head->cmd.args[i].nitems; ++j)
				ast_free(&head->cmd.args[i].items[j]);
			free(head->cmd.args[i].items);
		}
		free(head->cmd.args);
		redirs_free(&head->cmd.redirs);
	}
	free(head);
}

void
	ast_print_debug(
		t_string input,
		t_ast_node *head,
		size_t depth)
{
	if (!head)
		return ;
	for (size_t i = 0; i < depth; ++i)
		write(2, " | ", 3);
	if (head->type == NODE_ATOM)
		ft_dprintf(2, "ATOM(`%.*s`)\n", (int)head->atom.len, head->atom.str);
	else if (head->type == NODE_PARAMETER)
		ft_dprintf(2, "PARAMETER(`%.*s`)\n", (int)head->atom.len, head->atom.str);
	else if (head->type == NODE_SUBSHELL)
	{
		ft_dprintf(2, "SUBSHELL\n");
		ast_print_debug(head->expr.input, head->expr.head, depth + 1);
		print_redir(&head->expr.redirs, depth + 2);
	}
	else if (head->type == NODE_SUBEXPR)
	{
		ft_dprintf(2, "SUBEXPR\n");
		ast_print_debug(head->expr.input, head->expr.head, depth + 1);
	}
	else if (head->type == NODE_COMMAND)
	{
		ft_dprintf(2, "COMMAND [%zu]\n", head->cmd.nargs);
		for (size_t i = 0; i < head->cmd.nargs; ++i)
		{
			for (size_t k = 0; k < depth + 1; ++k)
				write(2, " | ", 3);
			write(2, "COMPOUND\n", 10);
			for (size_t j = 0; j < head->cmd.args[i].nitems; ++j)
				ast_print_debug(input, &head->cmd.args[i].items[j], depth + 2);
		}
		print_redir(&head->cmd.redirs, depth + 2);
	}
	else if (head->type == NODE_LOGIC)
	{
		ft_dprintf(2, "LOGIC `%s`\n", head->logic.token.reserved_word);
		ast_print_debug(input, head->logic.left, depth + 1);
		ast_print_debug(input, head->logic.right, depth + 1);
	}
}
