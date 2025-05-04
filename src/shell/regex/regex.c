/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/regex/regex.h"
#include "parser/ast/ast.h"
#include <shell/shell.h>

void
	regex_free(t_regex_ast *ast)
{
	size_t	i;

	i = 0;
	if (!ast)
		return ;
	if (ast->type == M_LITERAL)
		free(ast->literal);
	else if (ast->type == M_CHARCLASS)
		free(ast->range.list);
	else if (ast->type == M_EXTGLOB)
	{
		while (i < ast->glob.ngroups)
			regex_free(ast->glob.groups[i++]);
		free(ast->glob.groups);
	}
	else if (ast->type == M_SEQ || ast->type == M_ALT)
	{
		while (i < ast->compound.ngroups)
			regex_free(ast->compound.groups[i++]);
		free(ast->compound.groups);
	}
	free(ast);
}
